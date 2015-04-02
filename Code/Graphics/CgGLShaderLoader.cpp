#include "../BuildPreferences.hpp"

#if defined( SHADER_LOADER_USING_CG )
#include "CgGLShaderLoader.hpp"

#include "../AssertionError.hpp"
#include "../AssetInterface.hpp"
#include "../BuildPreferences.hpp"
#include "../DebuggerInterface.hpp"
#include "../StringConversion.hpp"

#if defined( PLATFORM_WINDOWS )
	#pragma comment( lib, "cg.lib" )
	#pragma comment( lib, "cgGL.lib" )

	//On windows, some GL definitions are necessary
	#include <gl/gl.h>
	#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
	#include "glext.h"

	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
	PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray = nullptr;
	PFNGLVERTEXATTRIBPOINTERPROC	  glVertexAttribPointer = nullptr;
#elif defined( PLATFORM_PS3 )
#endif // defined( PLATFORM_WINDOWS )


//-----------------------------------------------------------------------------------------------
const ShaderStage STAGE_Null = 0;
const ShaderStage STAGE_Domain = 0;//CG_GL_TESSELLATION_EVALUATION;
const ShaderStage STAGE_Fragment = CG_GL_FRAGMENT;
const ShaderStage STAGE_Geometry = 0;//CG_GL_GEOMETRY;
const ShaderStage STAGE_Hull = 0;//CG_GL_TESSELLATION_CONTROL;
const ShaderStage STAGE_Vertex = CG_GL_VERTEX;

//-----------------------------------------------------------------------------------------------
CgGLShaderLoader::CgGLShaderLoader()
	: m_shaderContainer( nullptr )
{
#if defined( PLATFORM_PS3 )
	cgRTCgcInit(); // Required call to initialize the CG compiler, else 'invalid enumerant' errors will occur.
#endif //defined( PLATFORM_PS3 )

	m_shaderContainer = cgCreateContext();

	// The windows platform needs these openGL functions to perform some functionality
#if defined( PLATFORM_WINDOWS )
	glDisableVertexAttribArray	= ( PFNGLDISABLEVERTEXATTRIBARRAYPROC ) wglGetProcAddress( "glDisableVertexAttribArray" );
	glEnableVertexAttribArray	= ( PFNGLENABLEVERTEXATTRIBARRAYPROC ) wglGetProcAddress( "glEnableVertexAttribArray" );
	glVertexAttribPointer		= ( PFNGLVERTEXATTRIBPOINTERPROC ) wglGetProcAddress( "glVertexAttribPointer" );
#endif
}

//-----------------------------------------------------------------------------------------------
CgGLShaderLoader::~CgGLShaderLoader()
{
	for( unsigned int i = 0; i < m_pipelineCache.size(); ++i )
	{
		delete m_pipelineCache[ i ];
	}
	m_pipelineCache.clear();
}

//-----------------------------------------------------------------------------------------------
bool CgGLShaderLoader::SupportsLanguage( ShaderLanguage language )
{
	switch( language )
	{
	case LANGUAGE_Cg:
		return true;
	case LANGUAGE_GLSL:
	case LANGUAGE_HLSL:
	case LANGUAGE_None:
	default:
		return false;
	}
}

#pragma region Interface
//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::BindVertexArrayToAttributeSlot( unsigned int slot, int numberOfVertexCoordinates,
	RendererInterface::CoordinateType coordinateType, bool normalizeData,
	unsigned int gapBetweenVertices, const void* firstVertexInArray )
{
#if defined( PLATFORM_PS3 )
	cgGLAttribPointer( slot, numberOfVertexCoordinates, coordinateType, normalizeData, gapBetweenVertices, firstVertexInArray );
#else
	glVertexAttribPointer( slot, numberOfVertexCoordinates, coordinateType, normalizeData, gapBetweenVertices, firstVertexInArray );
#endif
	VerifyNoCgErrorsHaveOccurredOrDie();
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::BindVertexArrayToVariable( const ShaderVariable* variable, int numberOfVertexCoordinates, RendererInterface::CoordinateType coordinateType,
	bool /*normalizeData*/, unsigned int gapBetweenVertices, const void* firstVertexInArray )
{
	cgGLSetParameterPointer( variable->parameter, numberOfVertexCoordinates, coordinateType, gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
Shader* CgGLShaderLoader::CompileShaderOrDie( ShaderStage shaderType, const char* sourceString )
{
	std::map< std::string, Shader >::iterator shaderInCache = m_shaderCache.find( sourceString );
	if( shaderInCache != m_shaderCache.end() )
	{
		return &shaderInCache->second;
	}

	static const char* ENTRY_POINT_IS_MAIN = nullptr;
	static const char** NO_ARGUMENTS = nullptr;
	CGprogram newShader = cgCreateProgram( m_shaderContainer, CG_SOURCE, sourceString, 
		cgGLGetLatestProfile( (CGGLenum)shaderType ), ENTRY_POINT_IS_MAIN, NO_ARGUMENTS );

	if( newShader == nullptr )
	{
		CGerror errorCode = GetCGErrorCode();
		std::string errorText( "Unable to create program from shader source.\nError Code: " );
		errorText.append( ConvertIntegerToString( errorCode ) );
		errorText.append( "\nError Text: " );
		errorText.append( GetCGErrorString( errorCode ) );
		FATAL_ERROR( "CgGL Shader Loader Error", errorText );
	}
	cgGLLoadProgram( newShader );
	VerifyNoCgErrorsHaveOccurredOrDie();

	m_shaderCache[ sourceString ] = newShader;
	return &m_shaderCache[ sourceString ];
}

//-----------------------------------------------------------------------------------------------
ShaderPipeline* CgGLShaderLoader::CreateOrGetShaderProgramFromFiles( const char* vertexSourceFileLocation, const char* fragmentSourceFileLocation )
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
ShaderPipeline* CgGLShaderLoader::CreateOrGetPipelineFromShaders( const Shader* vertexShader, const Shader* geometryShader, const Shader* fragmentShader )
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



	ShaderPipeline* newPipeline = new ShaderPipeline();
	newPipeline->vertexShader = vertexShader;
	newPipeline->geometryShader = geometryShader;
	newPipeline->fragmentShader = fragmentShader;

	if( geometryShader != nullptr )
	{
		CGprogram shaderArray[] = { vertexShader->shaderPointer, geometryShader->shaderPointer, fragmentShader->shaderPointer };
		newPipeline->combinedProgram = cgCombinePrograms( 3, shaderArray );
	}
	else
	{
		CGprogram shaderArray[] = { vertexShader->shaderPointer, fragmentShader->shaderPointer };
		newPipeline->combinedProgram = cgCombinePrograms( 2, shaderArray );
	}
	cgGLLoadProgram( newPipeline->combinedProgram );
	VerifyNoCgErrorsHaveOccurredOrDie();

	//Detach shaders so that they will be deleted automatically when the program is deleted
	DetachShaderFromPipeline( newPipeline->vertexShader, newPipeline );
	DetachShaderFromPipeline( newPipeline->fragmentShader, newPipeline );

	m_pipelineCache.push_back( newPipeline );
	return newPipeline;
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::DeletePipelineDataOnCard( ShaderPipeline* pipeline )
{
	cgDestroyProgram( pipeline->combinedProgram );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::DeleteShaderDataOnCard( Shader* shader )
{
	cgDestroyProgram( shader->shaderPointer );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::DetachShaderFromPipeline( const Shader* /*shader*/, ShaderPipeline* /*pipeline*/ )
{
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::DisableAttributeSlotInShader( unsigned int slot )
{
#if defined( PLATFORM_PS3 )
	cgGLDisableAttrib( slot );
#else
	glDisableVertexAttribArray( slot );
#endif // defined( PLATFORM_MACROS )
	VerifyNoCgErrorsHaveOccurredOrDie();
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::EnableAttributeSlotInShader( unsigned int slot )
{
#if defined( PLATFORM_PS3 )
	cgGLEnableAttrib( slot );
#else
	glEnableVertexAttribArray( slot );
#endif // defined( PLATFORM_MACROS )
	VerifyNoCgErrorsHaveOccurredOrDie();
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::DisableAttributeVariableInShader( const ShaderVariable* variable )
{
	cgGLDisableClientState( variable->parameter );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::EnableAttributeVariableInShader( const ShaderVariable* variable )
{
	cgGLEnableClientState( variable->parameter );
}

//-----------------------------------------------------------------------------------------------
ShaderVariable* CgGLShaderLoader::GetAttributeVariable( const ShaderPipeline* pipeline, const char* attributeName )
{
	ShaderVariable* newVariable = new ShaderVariable( cgGetNamedParameter( pipeline->vertexShader->shaderPointer, attributeName ) );

	if( newVariable->parameter == nullptr )
	{
		std::string errorText( "Unable to find attribute variable with name \"" );
		errorText.append( attributeName );
		errorText.append( "\" in the given shader.\n" );
		errorText.append( "The program may be able to continue, but graphics will likely be buggy!\n" );
		RECOVERABLE_ERROR( "CgGL Shader Loader Error", errorText );
	}
	return newVariable;
}

//-----------------------------------------------------------------------------------------------
int CgGLShaderLoader::GetNumberOfAttributesInPipeline( const ShaderPipeline* pipeline ) const
{
	unsigned int parameterCount = 0;
	CGparameter parameter = cgGetFirstParameter( pipeline->vertexShader->shaderPointer, CG_PROGRAM );
	while( parameter != nullptr )
	{
		parameter = cgGetNextParameter( parameter );
		++parameterCount;
	}
	return parameterCount;
}

//-----------------------------------------------------------------------------------------------
int CgGLShaderLoader::GetNumberOfUniformsInPipeline( const ShaderPipeline* pipeline ) const
{
	unsigned int parameterCount = 0;
	CGparameter parameter = cgGetFirstParameter( pipeline->vertexShader->shaderPointer, CG_PROGRAM );
	while( parameter != nullptr )
	{
		parameter = cgGetNextParameter( parameter );
		++parameterCount;
	}
	return parameterCount;
}

//-----------------------------------------------------------------------------------------------
ShaderVariable* CgGLShaderLoader::GetUniformVariable( const ShaderPipeline* pipeline, const char* uniformName )
{
	CGparameter parameterInShader = cgGetNamedParameter( pipeline->vertexShader->shaderPointer, uniformName );
	if( parameterInShader == nullptr )
		parameterInShader = cgGetNamedParameter( pipeline->fragmentShader->shaderPointer, uniformName );

	if( parameterInShader == nullptr )
	{
		std::string errorText( "Unable to find uniform variable with name \"" );
		errorText.append( uniformName );
		errorText.append( "\" in the given shader.\n" );
		errorText.append( "The program may be able to continue, but graphics will likely be buggy!\n" );
		RECOVERABLE_ERROR( "CgGL Shader Loader Error", errorText );
	}

	ShaderVariable* newVariable = new ShaderVariable( parameterInShader );
	return newVariable;
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::LoadSourceFromFileOrDie( char*& out_shaderData, const char* fileName )
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
void CgGLShaderLoader::SetTextureUnitUniform( ShaderVariable* variable, unsigned int samplerUnitNumber )
{
	cgGLSetTextureParameter( variable->parameter, samplerUnitNumber );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::SetUniform( ShaderVariable* variable, int integer )
{
	cgSetParameter1f( variable->parameter, (float)integer );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::SetUniform( ShaderVariable* variable, float floatingPointNumber )
{
	cgSetParameter1f( variable->parameter, floatingPointNumber );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::SetUniform( ShaderVariable* variable, const Color& color )
{
	cgSetParameter4f( variable->parameter, (float)color.r, (float)color.g, (float)color.b, (float)color.a );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::SetUniform( ShaderVariable* variable, const IntVector2& vector2 )
{
	cgSetParameter2f( variable->parameter, (float)vector2.x, (float)vector2.y );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::SetUniform( ShaderVariable* variable, const FloatVector2& vector2 )
{
	cgSetParameter2f( variable->parameter, vector2.x, vector2.y );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::SetUniform( ShaderVariable* variable, const FloatVector3& vector3 )
{
	cgSetParameter3f( variable->parameter, vector3.x, vector3.y, vector3.z );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::SetUniform( ShaderVariable* variable, const FloatVector4& vector4 )
{
	cgSetParameter4f( variable->parameter, vector4.x, vector4.y, vector4.z, vector4.w );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::SetUniform( ShaderVariable* variable, const Float4x4Matrix& matrix )
{
	cgGLSetMatrixParameterfr( variable->parameter, matrix.GetRawBuffer() );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::UseShaderPipeline( const ShaderPipeline* pipeline )
{
	cgGLEnableProfile(cgGLGetLatestProfile(CG_GL_VERTEX));
	cgGLEnableProfile(cgGLGetLatestProfile(CG_GL_FRAGMENT));

	cgGLBindProgram( pipeline->vertexShader->shaderPointer );
	cgGLBindProgram( pipeline->fragmentShader->shaderPointer );
	VerifyNoCgErrorsHaveOccurredOrDie();
}
#pragma endregion //Interface



#pragma region Helpers
//-----------------------------------------------------------------------------------------------
CGerror CgGLShaderLoader::GetCGErrorCode()
{
	return cgGetError();
}

//-----------------------------------------------------------------------------------------------
const char* CgGLShaderLoader::GetCGErrorString( CGerror errorCode )
{
	if( errorCode == CG_COMPILER_ERROR )
#if defined( PLATFORM_PS3 ) // The PS3 version of Cg does NOT implement cgGetLastListing().
		return "The shader failed to compile.\n";
#else
		return cgGetLastListing( m_shaderContainer );
#endif
	else
		return cgGetErrorString( errorCode );
}

//-----------------------------------------------------------------------------------------------
void CgGLShaderLoader::VerifyNoCgErrorsHaveOccurredOrDie()
{
	CGerror errorCode = GetCGErrorCode();
	if( errorCode != CG_NO_ERROR )
	{
		std::string errorText( "A Cg error has occurred.\nError Code: " );
		errorText.append( ConvertIntegerToString( errorCode ) );
		errorText.append( "\nError Text: " );
		errorText.append( GetCGErrorString( errorCode ) );
		FATAL_ERROR( "CgGL Shader Loader Error", errorText );
	}
}
#pragma endregion //Helpers

#endif //defined( SHADER_LOADER_USING_CG )