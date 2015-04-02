#include "../BuildPreferences.hpp"

#if defined( SHADER_LOADER_USING_GLSL )
#include "GLSLShaderLoader.hpp"

#include "../AssertionError.hpp"
#include "../AssetInterface.hpp"
#include "../StringConversion.hpp"
#include "RendererInterface.hpp"

#if defined( RENDERER_INTERFACE_USE_OPENGL_ES2 )
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#elif defined( RENDERER_INTERFACE_USE_OPENGL ) && defined( PLATFORM_WINDOWS )
#include "../PlatformSpecificHeaders.hpp"
	#include <gl/gl.h>
	#include "glext.h"

#pragma region OpenGL Function Declarations
PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray	= nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray	= nullptr;
PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer		= nullptr;
PFNGLCOMPILESHADERPROC				glCompileShader				= nullptr;
PFNGLCREATESHADERPROC				glCreateShader				= nullptr;
PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog			= nullptr;
PFNGLGETSHADERIVPROC				glGetShaderiv				= nullptr;
PFNGLSHADERSOURCEPROC				glShaderSource				= nullptr;
PFNGLATTACHSHADERPROC				glAttachShader				= nullptr;
PFNGLBINDATTRIBLOCATIONPROC			glBindAttribLocation		= nullptr;
PFNGLCREATEPROGRAMPROC				glCreateProgram				= nullptr;
PFNGLDELETEPROGRAMPROC				glDeleteProgram				= nullptr;
PFNGLDELETESHADERPROC				glDeleteShader				= nullptr;
PFNGLDETACHSHADERPROC				glDetachShader				= nullptr;
PFNGLGETACTIVEATTRIBPROC			glGetActiveAttrib			= nullptr;
PFNGLGETACTIVEUNIFORMPROC			glGetActiveUniform			= nullptr;
PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation			= nullptr;
PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation		= nullptr;
PFNGLGETPROGRAMIVPROC				glGetProgramiv				= nullptr;
PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog			= nullptr;
PFNGLLINKPROGRAMPROC				glLinkProgram				= nullptr;
PFNGLUSEPROGRAMPROC					glUseProgram				= nullptr;
PFNGLUNIFORM1FPROC					glUniform1f					= nullptr;
PFNGLUNIFORM2FPROC					glUniform2f					= nullptr;
PFNGLUNIFORM3FPROC					glUniform3f					= nullptr;
PFNGLUNIFORM4FPROC					glUniform4f					= nullptr;
PFNGLUNIFORM1FVPROC					glUniform1fv				= nullptr;
PFNGLUNIFORM2FVPROC					glUniform2fv				= nullptr;
PFNGLUNIFORM3FVPROC					glUniform3fv				= nullptr;
PFNGLUNIFORM4FVPROC					glUniform4fv				= nullptr;
PFNGLUNIFORM1IPROC					glUniform1i					= nullptr;
PFNGLUNIFORM2IPROC					glUniform2i					= nullptr;
PFNGLUNIFORM3IPROC					glUniform3i					= nullptr;
PFNGLUNIFORM4IPROC					glUniform4i					= nullptr;
PFNGLUNIFORM1IVPROC					glUniform1iv				= nullptr;
PFNGLUNIFORM2IVPROC					glUniform2iv				= nullptr;
PFNGLUNIFORM3IVPROC					glUniform3iv				= nullptr;
PFNGLUNIFORM4IVPROC					glUniform4iv				= nullptr;
PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv			= nullptr;
#pragma endregion // OpenGL Function Declarations
#endif // defined( PLATFORM_WINDOWS )

//-----------------------------------------------------------------------------------------------
GLSLShaderLoader::GLSLShaderLoader()
{
#if defined( PLATFORM_WINDOWS )
	glDisableVertexAttribArray	= ( PFNGLDISABLEVERTEXATTRIBARRAYPROC ) wglGetProcAddress( "glDisableVertexAttribArray" );
	glEnableVertexAttribArray	= ( PFNGLENABLEVERTEXATTRIBARRAYPROC ) wglGetProcAddress( "glEnableVertexAttribArray" );
	glVertexAttribPointer		= ( PFNGLVERTEXATTRIBPOINTERPROC ) wglGetProcAddress( "glVertexAttribPointer" );
	glCompileShader				= ( PFNGLCOMPILESHADERPROC ) wglGetProcAddress( "glCompileShader" );
	glCreateShader				= ( PFNGLCREATESHADERPROC ) wglGetProcAddress( "glCreateShader" );
	glGetShaderInfoLog			= ( PFNGLGETSHADERINFOLOGPROC ) wglGetProcAddress( "glGetShaderInfoLog" );
	glGetShaderiv				= ( PFNGLGETSHADERIVPROC ) wglGetProcAddress( "glGetShaderiv" );
	glShaderSource				= ( PFNGLSHADERSOURCEPROC ) wglGetProcAddress( "glShaderSource" );
	glAttachShader				= ( PFNGLATTACHSHADERPROC ) wglGetProcAddress( "glAttachShader" );
	glBindAttribLocation		= ( PFNGLBINDATTRIBLOCATIONPROC ) wglGetProcAddress( "glBindAttribLocation" );
	glCreateProgram				= ( PFNGLCREATEPROGRAMPROC ) wglGetProcAddress( "glCreateProgram" );
	glDeleteProgram				= ( PFNGLDELETEPROGRAMPROC ) wglGetProcAddress( "glDeleteProgram" );
	glDeleteShader				= ( PFNGLDELETESHADERPROC ) wglGetProcAddress( "glDeleteShader" );
	glDetachShader				= ( PFNGLDETACHSHADERPROC ) wglGetProcAddress( "glDetachShader" );
	glGetActiveAttrib			= ( PFNGLGETACTIVEATTRIBPROC ) wglGetProcAddress( "glGetActiveAttrib" );
	glGetActiveUniform			= ( PFNGLGETACTIVEUNIFORMPROC ) wglGetProcAddress( "glGetActiveUniform" );
	glGetAttribLocation			= ( PFNGLGETATTRIBLOCATIONPROC ) wglGetProcAddress( "glGetAttribLocation" );
	glGetUniformLocation		= ( PFNGLGETUNIFORMLOCATIONPROC ) wglGetProcAddress( "glGetUniformLocation" );
	glGetProgramiv				= ( PFNGLGETPROGRAMIVPROC ) wglGetProcAddress( "glGetProgramiv" );	   
	glGetProgramInfoLog			= ( PFNGLGETPROGRAMINFOLOGPROC ) wglGetProcAddress( "glGetProgramInfoLog" );
	glLinkProgram				= ( PFNGLLINKPROGRAMPROC ) wglGetProcAddress( "glLinkProgram" );
	glUseProgram				= ( PFNGLUSEPROGRAMPROC ) wglGetProcAddress( "glUseProgram" );
	glUniform1f					= ( PFNGLUNIFORM1FPROC ) wglGetProcAddress( "glUniform1f" );
	glUniform2f					= ( PFNGLUNIFORM2FPROC ) wglGetProcAddress( "glUniform2f" );
	glUniform3f					= ( PFNGLUNIFORM3FPROC ) wglGetProcAddress( "glUniform3f" );
	glUniform4f					= ( PFNGLUNIFORM4FPROC ) wglGetProcAddress( "glUniform4f" );
	glUniform1fv				= ( PFNGLUNIFORM1FVPROC ) wglGetProcAddress( "glUniform1fv" );
	glUniform2fv				= ( PFNGLUNIFORM2FVPROC ) wglGetProcAddress( "glUniform2fv" );
	glUniform3fv				= ( PFNGLUNIFORM3FVPROC ) wglGetProcAddress( "glUniform3fv" );
	glUniform4fv				= ( PFNGLUNIFORM4FVPROC ) wglGetProcAddress( "glUniform4fv" );
	glUniform1i					= ( PFNGLUNIFORM1IPROC ) wglGetProcAddress( "glUniform1i" );
	glUniform2i					= ( PFNGLUNIFORM2IPROC ) wglGetProcAddress( "glUniform2i" );
	glUniform3i					= ( PFNGLUNIFORM3IPROC ) wglGetProcAddress( "glUniform3i" );
	glUniform4i					= ( PFNGLUNIFORM4IPROC ) wglGetProcAddress( "glUniform4i" );
	glUniform1iv				= ( PFNGLUNIFORM1IVPROC ) wglGetProcAddress( "glUniform1iv" );
	glUniform2iv				= ( PFNGLUNIFORM2IVPROC ) wglGetProcAddress( "glUniform2iv" );
	glUniform3iv				= ( PFNGLUNIFORM3IVPROC ) wglGetProcAddress( "glUniform3iv" );
	glUniform4iv				= ( PFNGLUNIFORM4IVPROC ) wglGetProcAddress( "glUniform4iv" );
	glUniformMatrix4fv			= ( PFNGLUNIFORMMATRIX4FVPROC ) wglGetProcAddress( "glUniformMatrix4fv" );
#endif // defined( PLATFORM_WINDOWS )
}

//-----------------------------------------------------------------------------------------------
const ShaderStage STAGE_Null = 0;

#if defined( PLATFORM_HTML5 ) || defined( PLATFORM_ANDROID )
	const ShaderStage STAGE_Domain = GL_FALSE;
	const ShaderStage STAGE_Geometry = GL_FALSE;
	const ShaderStage STAGE_Hull = GL_FALSE;
#elif defined( PLATFORM_WINDOWS ) 
	const ShaderStage STAGE_Domain = GL_TESS_EVALUATION_SHADER;
	const ShaderStage STAGE_Geometry = GL_GEOMETRY_SHADER;
	const ShaderStage STAGE_Hull = GL_TESS_CONTROL_SHADER;
#endif
const ShaderStage STAGE_Fragment = GL_FRAGMENT_SHADER;
const ShaderStage STAGE_Vertex = GL_VERTEX_SHADER;

//-----------------------------------------------------------------------------------------------
GLSLShaderLoader::~GLSLShaderLoader()
{
	for( unsigned int i = 0; i < m_pipelineCache.size(); ++i )
	{
		delete m_pipelineCache[ i ];
	}
	m_pipelineCache.clear();
}

//-----------------------------------------------------------------------------------------------
bool GLSLShaderLoader::SupportsLanguage( ShaderLanguage language )
{
	switch( language )
	{
	case LANGUAGE_GLSL:
		return true;
	case LANGUAGE_Cg:
	case LANGUAGE_HLSL:
	case LANGUAGE_None:
	default:
		return false;
	}
}

#pragma region Interface
//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::BindVertexArrayToAttributeSlot( unsigned int slot, int numberOfVertexCoordinates,
	RendererInterface::CoordinateType coordinateType, bool normalizeData,
	unsigned int gapBetweenVertices, const void* firstVertexInArray )
{
	glVertexAttribPointer( slot, numberOfVertexCoordinates, coordinateType, normalizeData, gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::BindVertexArrayToVariable( const ShaderVariable* variable, int numberOfVertexCoordinates, RendererInterface::CoordinateType coordinateType,
	bool normalizeData, unsigned int gapBetweenVertices, const void* firstVertexInArray )
{
	glVertexAttribPointer( variable->location, numberOfVertexCoordinates, coordinateType, normalizeData, gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
Shader* GLSLShaderLoader::CompileShaderOrDie( ShaderStage shaderType, const char* sourceString )
{
	std::map< std::string, Shader >::iterator shaderInCache = m_shaderCache.find( sourceString );
	if( shaderInCache != m_shaderCache.end() )
	{
		return &shaderInCache->second;
	}

	glUseProgram( 0 );
	GLuint newShaderID = glCreateShader( shaderType );

	static const GLsizei NUMBER_OF_STRINGS_TO_LOAD = 1;
	static const GLint* LENGTHS_OF_EACH_SOURCE_STRING = nullptr;
	glShaderSource( newShaderID, NUMBER_OF_STRINGS_TO_LOAD, ( const GLchar** )&sourceString, LENGTHS_OF_EACH_SOURCE_STRING );

	GLint compilationResult = GL_FALSE;
	glCompileShader( newShaderID );
	glGetShaderiv( newShaderID, GL_COMPILE_STATUS, &compilationResult );

	if( compilationResult == GL_FALSE )
	{
		std::string errorText( "An error has occurred while compiling a GLSL shader.\nError Details:\n" );

		char* errorString = nullptr;
		int errorStringLength = -1;
		GetInfoLogForShader( errorString, errorStringLength, newShaderID );
		errorText.append( errorString, errorStringLength );
		delete[] errorString;

		FATAL_ERROR( "GLSL Shader Loader Error", errorText );
	}

	m_shaderCache[ sourceString ] = newShaderID;
	return &m_shaderCache[ sourceString ];
}

//-----------------------------------------------------------------------------------------------
ShaderPipeline* GLSLShaderLoader::CreateOrGetShaderProgramFromFiles( const char* vertexSourceFileLocation, const char* fragmentSourceFileLocation )
{
	char* vertexShaderSource = nullptr;
	char* fragmentShaderSource = nullptr;

	LoadSourceFromFileOrDie( vertexShaderSource, vertexSourceFileLocation );
	LoadSourceFromFileOrDie( fragmentShaderSource, fragmentSourceFileLocation );

	Shader* vertexShader = CompileShaderOrDie( STAGE_Vertex, vertexShaderSource );
	Shader* fragmentShader = CompileShaderOrDie( STAGE_Fragment, fragmentShaderSource );

	delete vertexShaderSource;
	delete fragmentShaderSource;
	return CreateOrGetPipelineFromShaders( vertexShader, nullptr, fragmentShader );
}

//-----------------------------------------------------------------------------------------------
ShaderPipeline* GLSLShaderLoader::CreateOrGetPipelineFromShaders( const Shader* vertexShader, const Shader* geometryShader, const Shader* fragmentShader )
{
	//Find and return a previously created shader pipeline if it exists
	ShaderPipeline pipelineToFind;
	pipelineToFind.vertexShader = vertexShader;
	pipelineToFind.geometryShader = geometryShader;
	pipelineToFind.fragmentShader = fragmentShader;
	for( unsigned int i = 0; i < m_pipelineCache.size(); ++i )
	{
		ShaderPipeline& pipelineInCache = *m_pipelineCache[ i ];
		if( pipelineInCache == pipelineToFind )
			return m_pipelineCache[ i ];
	}


	//Or Create a new pipeline
	GLuint programID = glCreateProgram();

	glAttachShader( programID, vertexShader->glID );

	if( geometryShader != nullptr )
		glAttachShader( programID, geometryShader->glID );

	glAttachShader( programID, fragmentShader->glID );

	GLint linkingResult = GL_FALSE;
	glLinkProgram( programID );
	glGetProgramiv( programID, GL_LINK_STATUS, &linkingResult );

	if( linkingResult == GL_FALSE )
	{
		std::string errorText( "An error has occurred while linking the GLSL shaders into a pipeline.\nError Details:\n" );

		char* errorString = nullptr;
		int errorStringLength = -1;
		GetInfoLogForProgram( errorString, errorStringLength, programID );
		errorText.append( errorString, errorStringLength );
		delete[] errorString;

		FATAL_ERROR( "GLSL Shader Loader Error", errorText );
	}

	ShaderPipeline* newPipeline = new ShaderPipeline();
	newPipeline->vertexShader = vertexShader;
	newPipeline->geometryShader = geometryShader;
	newPipeline->fragmentShader = fragmentShader;
	newPipeline->programID = programID;

	//Detach shaders so that they will be deleted automatically when the program is deleted
	DetachShaderFromPipeline( newPipeline->vertexShader, newPipeline );
	DetachShaderFromPipeline( newPipeline->fragmentShader, newPipeline );

	m_pipelineCache.push_back( newPipeline );
	return newPipeline;
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::DeletePipelineDataOnCard( ShaderPipeline* pipeline )
{
	glDeleteProgram( pipeline->programID );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::DeleteShaderDataOnCard( Shader* shader )
{
	glDeleteShader( shader->glID );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::DetachShaderFromPipeline( const Shader* shader, ShaderPipeline* pipeline )
{
	glDetachShader( pipeline->programID, shader->glID );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::DisableAttributeSlotInShader( unsigned int slot )
{
	glDisableVertexAttribArray( slot );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::EnableAttributeSlotInShader( unsigned int slot )
{
	glEnableVertexAttribArray( slot );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::DisableAttributeVariableInShader( const ShaderVariable* variable )
{
	glDisableVertexAttribArray( variable->location );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::EnableAttributeVariableInShader( const ShaderVariable* variable )
{
	glEnableVertexAttribArray( variable->location );
}

//-----------------------------------------------------------------------------------------------
ShaderVariable* GLSLShaderLoader::GetAttributeVariable( const ShaderPipeline* pipeline, const char* attributeName )
{
	return new ShaderVariable( glGetAttribLocation( pipeline->programID, attributeName ) );
}

//-----------------------------------------------------------------------------------------------
int GLSLShaderLoader::GetNumberOfAttributesInPipeline( const ShaderPipeline* pipeline ) const
{
	int numberOfAttributes = 0;
	glGetProgramiv( pipeline->programID, GL_ACTIVE_ATTRIBUTES, &numberOfAttributes );
	return numberOfAttributes;
}

//-----------------------------------------------------------------------------------------------
int GLSLShaderLoader::GetNumberOfUniformsInPipeline( const ShaderPipeline* pipeline ) const
{
	int numberOfUniforms = 0;
	glGetProgramiv( pipeline->programID, GL_ACTIVE_UNIFORMS, &numberOfUniforms );
	return numberOfUniforms;
}

//-----------------------------------------------------------------------------------------------
ShaderVariable* GLSLShaderLoader::GetUniformVariable( const ShaderPipeline* pipeline, const char* uniformName )
{
	return new ShaderVariable( glGetUniformLocation( pipeline->programID, uniformName ) );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::LoadSourceFromFileOrDie( char*& out_shaderData, const char* fileName )
{
	FILE* shaderFile = nullptr;
	shaderFile = AssetInterface::OpenAssetAsFile( fileName, "rb" );
	if( shaderFile == nullptr )
	{
		std::string errorMessage;
		errorMessage.append( "Failed to load shader " );
		errorMessage.append( fileName );
		errorMessage.append( ".\nError Code: " );
		errorMessage.append( ConvertIntegerToString( AssetInterface::GetErrorCode() ) );
		errorMessage.append( "\nError Message: " );
		errorMessage.append( AssetInterface::GetErrorString() );
		FATAL_ERROR( "Vingine Shader Error", errorMessage.c_str() );
	}

	AssetInterface::SeekInAssetFile( shaderFile, 0, SEEK_END );
	long fileSize = AssetInterface::GetCurrentPositionInAssetFile( shaderFile );
	AssetInterface::SeekInAssetFile( shaderFile, 0, SEEK_SET );

	out_shaderData = new char[ fileSize + 1 ]; //Leave room for a null terminator.
	AssetInterface::ReadFromAssetFile( out_shaderData, sizeof( char ), fileSize, shaderFile );
	AssetInterface::CloseAssetFile( shaderFile );
	out_shaderData[ fileSize ] = '\0';
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::SetTextureUnitUniform( ShaderVariable* variable, unsigned int samplerUnitNumber )
{
	glUniform1i( variable->location, samplerUnitNumber ); 
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::SetUniform( ShaderVariable* variable, int integer )
{
	glUniform1i( variable->location, integer ); 
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::SetUniform( ShaderVariable* variable, float floatingPointNumber )
{
	glUniform1f( variable->location, floatingPointNumber );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::SetUniform( ShaderVariable* variable, const Color& color )
{
	glUniform4i( variable->location, color.r, color.g, color.b, color.a );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::SetUniform( ShaderVariable* variable, const IntVector2& vector2 )
{
	glUniform2i( variable->location, vector2.x, vector2.y );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::SetUniform( ShaderVariable* variable, const FloatVector2& vector2 )
{
	glUniform2f( variable->location, vector2.x, vector2.y );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::SetUniform( ShaderVariable* variable, const FloatVector3& vector3 )
{
	glUniform3f( variable->location, vector3.x, vector3.y, vector3.z );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::SetUniform( ShaderVariable* variable, const FloatVector4& vector4 )
{
	glUniform4f( variable->location, vector4.x, vector4.y, vector4.z, vector4.w );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::SetUniform( ShaderVariable* variable, const Float4x4Matrix& matrix )
{
	glUniformMatrix4fv( variable->location, 1, false, matrix.GetRawBuffer() );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::SetUniformVariable( const ShaderVariable* const variable, const Float4x4Matrix& matrix )
{
	glUniformMatrix4fv( variable->location, 1, false, matrix.GetRawBuffer() );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::UseShaderPipeline( const ShaderPipeline* pipeline )
{
	glUseProgram( pipeline->programID );
}
#pragma endregion //Interface



//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::GetInfoLogForProgram( char*& out_infoLog, int& out_infoLogSize, int programID )
{
	static GLsizei* DO_NOT_WANT_NUMBER_CHARS_RETURNED = nullptr;
	glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &out_infoLogSize );
	out_infoLog = new char[ out_infoLogSize ];

	glGetProgramInfoLog( programID, out_infoLogSize, DO_NOT_WANT_NUMBER_CHARS_RETURNED, out_infoLog );
}

//-----------------------------------------------------------------------------------------------
void GLSLShaderLoader::GetInfoLogForShader( char*& out_infoLog, int& out_infoLogSize, int shaderID )
{
	static GLsizei* DO_NOT_WANT_NUMBER_CHARS_RETURNED = nullptr;
	glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &out_infoLogSize );
	out_infoLog = new char[ out_infoLogSize ];

	glGetShaderInfoLog( shaderID, out_infoLogSize, DO_NOT_WANT_NUMBER_CHARS_RETURNED, out_infoLog );
}

#endif //defined( SHADER_LOADER_USING_GLSL )
