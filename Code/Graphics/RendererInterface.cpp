#include "RendererInterface.hpp"

#include "../BuildPreferences.hpp"
#include "../AssertionError.hpp"
#include "../StringConversion.hpp"
#include "Material.hpp"

#if defined( RENDERER_INTERFACE_USE_OPENGL )
	#include "OGLRendererInterface.hpp"
#elif defined( PLATFORM_PS3 )
	#include "PSGLRendererInterface.hpp"
#elif defined( RENDERER_INTERFACE_USE_OPENGL_ES2 )
	#include "OGLES2RendererInterface.hpp"
#elif defined( RENDERER_INTERFACE_USE_NULL )
	#include "NullRendererInterface.hpp"
#endif // defined( RENDERER_INTERFACE_USE_MACROS )

#include "NullTextureManager.hpp"
#include "STBTextureManager.hpp"

#include "CachingShaderLoader.hpp"
#if defined( SHADER_LOADER_USING_GLSL )
	#include "GLSLShaderLoader.hpp"
#elif defined( SHADER_LOADER_USING_CG ) && ( defined( RENDERER_INTERFACE_USE_OPENGL ) || defined( RENDERER_INTERFACE_USE_OPENGL_ES2 ) || defined( PLATFORM_PS3 ) )
	#include "CgGLShaderLoader.hpp"
#elif defined( SHADER_LOADER_USING_NULL )
	#include "NullShaderLoader.hpp"
#endif // defined( SHADER_LOADER_USING_MACROS )

#include "../Font/CachingFontLoader.hpp"

#include "Material.hpp"
#include "VertexAttribute.hpp"
#include "VertexData.hpp"


//-----------------------------------------------------------------------------------------------
STATIC const RendererInterface::DefaultAttributeName RendererInterface::DEFAULT_NAME_Vertex			= "i_vertexWorldPosition";
STATIC const RendererInterface::DefaultAttributeName RendererInterface::DEFAULT_NAME_Color			= "i_vertexColor";
STATIC const RendererInterface::DefaultAttributeName RendererInterface::DEFAULT_NAME_TextureCoords	= "i_textureCoordinates";
STATIC const RendererInterface::DefaultAttributeName RendererInterface::DEFAULT_NAME_Normal			= "i_textureNormal";
STATIC const RendererInterface::DefaultAttributeName RendererInterface::DEFAULT_NAME_Tangent		= "i_textureTangent";
STATIC const RendererInterface::DefaultAttributeName RendererInterface::DEFAULT_NAME_Bitangent		= "i_textureBitangent";
STATIC const RendererInterface::DefaultAttributeName RendererInterface::DEFAULT_NAME_BoneIndex0		= "i_tendons[0].boneIndex";
STATIC const RendererInterface::DefaultAttributeName RendererInterface::DEFAULT_NAME_BoneWeight0	= "i_tendons[0].boneWeight";

//-----------------------------------------------------------------------------------------------
STATIC RendererInterface* RendererInterface::s_activeRendererInterface = nullptr;


//-----------------------------------------------------------------------------------------------
VIRTUAL RendererInterface::~RendererInterface()
{
	std::map< std::wstring, Material* >::iterator materialIterator;
	for( materialIterator = m_materials.begin(); materialIterator != m_materials.end(); ++materialIterator )
	{
		delete materialIterator->second;
	}
	m_materials.clear();

	m_lights.clear();
}

//-----------------------------------------------------------------------------------------------
STATIC void RendererInterface::Startup()
{
	FATAL_ASSERTION( s_activeRendererInterface == nullptr, "Rendering Interface Error", "Cannot start up multiple rendering interfaces!" );

#if defined( RENDERER_INTERFACE_USE_WEBGL_1 )

	s_activeRendererInterface = new WebGL1RendererInterface();

#elif defined( RENDERER_INTERFACE_USE_OPENGL_ES2 )

	#if defined( PLATFORM_HTML5 )
		#pragma warning( "OpenGL ES2 calls are supported in Emscripten using emulation. Please add the emcc option '-s FULL_ES2=1' to the linking stage." )
		s_activeRendererInterface = new OGLES2RendererInterface();
	#else
		s_activeRendererInterface = new OGLES2RendererInterface();
	#endif // defined( PLATFORM_MACROS )


#elif defined( RENDERER_INTERFACE_USE_OPENGL )

	#if defined( PLATFORM_HTML5 )
		#pragma warning( "This rendering interface is unsupported in HTML5. Please use either the OpenGL ES or WebGL interfaces." )
	#else
		s_activeRendererInterface = new OGLRendererInterface();
	#endif // defined( PLATFORM_MACROS )

#elif defined( PLATFORM_PS3 )
	s_activeRendererInterface = new PSGLRendererInterface();
#else

	s_activeRendererInterface = new NullRendererInterface();

#endif // defined( RENDERER_INTERFACE_USE_MACROS )

	FATAL_ASSERTION( s_activeRendererInterface != nullptr, "Rendering Interface Error", "Unable to start up the rendering interface!" );
	s_activeRendererInterface->Initialize();

	s_activeRendererInterface->m_activeFontLoader = new CachingFontLoader();

#if defined( SHADER_LOADER_USING_GLSL )
	s_activeRendererInterface->m_activeShaderLoader = new GLSLShaderLoader();
#elif defined( SHADER_LOADER_USING_CG )
	s_activeRendererInterface->m_activeShaderLoader = new CgGLShaderLoader();
#elif defined( SHADER_LOADER_USING_NULL )
	s_activeRendererInterface->m_activeShaderLoader = new NullShaderLoader();
#endif // defined( SHADER_LOADER_USING_MACROS )

	s_activeRendererInterface->m_activeTextureManager = new STBTextureManager();
	//s_activeRendererInterface->m_activeTextureManager = new NullTextureManager();

	FATAL_ASSERTION( s_activeRendererInterface->m_activeTextureManager != nullptr, "Texture Manager Error", "Unable to create texture manager for the renderer." );
}

//-----------------------------------------------------------------------------------------------
STATIC void RendererInterface::Shutdown()
{
	//Internal managers need to be destroyed separately before the renderer's destructor.
	//   This must happen this way because manager destructors often call renderer functions for cleanup,
	//   and by the time renderer's destructor is called, the derived interface has been destroyed.
	//   For any virtually implmented renderer functions, the calls then become pure virtual, which causes a crash.
	delete s_activeRendererInterface->m_activeFontLoader;
	delete s_activeRendererInterface->m_activeShaderLoader;
	delete s_activeRendererInterface->m_activeTextureManager;

	delete s_activeRendererInterface;
}

//-----------------------------------------------------------------------------------------------
STATIC void RendererInterface::UpdateSkeletonOnMaterial( const Float4x4Matrix& /*objectStartingTransform*/, const std::vector< Bone >& /*skeleton*/, Material* /*material*/ )
{
// 	Float4x4Matrix vertexLocalToSkinnedWorldTransform;
// 	unsigned int numberOfBonesInSkeleton = skeleton.size();
// 	for( unsigned int i = 0; i < numberOfBonesInSkeleton; ++i )
// 	{
// 		vertexLocalToSkinnedWorldTransform = objectStartingTransform * skeleton[ i ].inverseRestTransform * skeleton[ i ].transformPointer->GetMatrix();
// 
// 		std::string boneUniformName( "u_boneTransformationMatrices[" + std::to_string( long long( i ) ) + "]" );
// 		material->SetMatrixUniform( boneUniformName, vertexLocalToSkinnedWorldTransform );
// 	}
}

//-----------------------------------------------------------------------------------------------
STATIC void RendererInterface::UpdateLightsOnMaterial( Material* material )
{
	unsigned int numberOfLights = s_activeRendererInterface->m_lights.size();
	for( unsigned int i = 0; i < numberOfLights; ++i )
	{
		std::string lightUniformName( "u_lights[" + ConvertIntegerToString( i ) + "]" );
		material->SetLightUniform( lightUniformName, s_activeRendererInterface->m_lights[ i ] );
	}

	Light defaultLight;
	for( unsigned int i = numberOfLights; i < MAX_LIGHTS_IN_SHADER; ++i )
	{
		std::string lightUniformName( "u_lights[" + ConvertIntegerToString( i ) + "]" );
		material->SetLightUniform( lightUniformName, defaultLight );
	}
}

//-----------------------------------------------------------------------------------------------
STATIC Material* RendererInterface::CreateOrGetNewMaterial( const std::wstring& materialName )
{
	Material* material = s_activeRendererInterface->m_materials[ materialName ];
	if( material == nullptr )
	{
		s_activeRendererInterface->m_materials[ materialName ] = new Material();
	}
	return s_activeRendererInterface->m_materials[ materialName ];
}

//-----------------------------------------------------------------------------------------------
STATIC void RendererInterface::ApplyMaterial( const Material* material )
{
	CachingShaderLoader* shaderLoader = s_activeRendererInterface->m_activeShaderLoader;
	const ShaderPipeline* const& pipeline = material->pipeline;

	shaderLoader->UseShaderPipeline( pipeline );

 	ShaderVariable* modelVariable = shaderLoader->GetUniformVariable( pipeline, "u_modelMatrix" );
 	shaderLoader->SetUniform( modelVariable, GetModelMatrix() );
 	delete modelVariable;
 
 	ShaderVariable* viewVariable = shaderLoader->GetUniformVariable( pipeline, "u_viewMatrix" );
 	shaderLoader->SetUniform( viewVariable, GetViewMatrix() );
 	delete viewVariable;

	ShaderVariable* projectionVariable = shaderLoader->GetUniformVariable( pipeline, "u_projectionMatrix" );
	shaderLoader->SetUniform( projectionVariable, GetProjectionMatrix() );
	delete projectionVariable;

	for( unsigned int i = 0; i < material->infoForTextures.size(); ++i )
	{
		const Material::TextureInfo& texInfo = material->infoForTextures[ i ];

		shaderLoader->SetTextureUnitUniform( texInfo.samplerUniformVariable, texInfo.textureUnitID );
		SetActiveTextureUnit( texInfo.textureUnitID );
		BindTexture( RendererInterface::TEXTURES_2D, texInfo.texture );
	}

	SetLineWidth( material->lineWidth );
}

//-----------------------------------------------------------------------------------------------
STATIC void RendererInterface::RemoveMaterial( const Material* material )
{
	for( unsigned int i = 0; i < material->infoForTextures.size(); ++i )
	{
		const Material::TextureInfo& texInfo = material->infoForTextures[ i ];
		SetActiveTextureUnit( texInfo.textureUnitID );
	}

	SetLineWidth( 1 );
}

#pragma region Convenience Structures
//-----------------------------------------------------------------------------------------------
STATIC void RendererInterface::BindVertexDataToShader( const VertexData* vertData, const ShaderPipeline* pipeline )
{
	CachingShaderLoader*& shaderLoader = s_activeRendererInterface->m_activeShaderLoader;

	BindBufferObject( ARRAY_BUFFER, vertData->bufferID );

	size_t vertDataLocation = !vertData->IsBuffered() * reinterpret_cast< size_t >( vertData->data );
	unsigned int numberOfAttributes = vertData->attributes.size();
	for( unsigned int i = 0; i < numberOfAttributes; ++i )
	{
		const VertexAttribute& attribute = vertData->attributes[ i ];
		ShaderVariable* attributeVariable = shaderLoader->GetAttributeVariable( pipeline, attribute.shaderVariableName );

		shaderLoader->BindVertexArrayToVariable( attributeVariable, attribute.numberOfComponents, attribute.coordinateType,
			attribute.normalizeFixedPointData, attribute.bytesUntilNextInstance, 
			reinterpret_cast<void*>( vertDataLocation + attribute.attributeOffsetInStructure ) );
		shaderLoader->EnableAttributeVariableInShader( attributeVariable );
		delete attributeVariable;
	}
}

//-----------------------------------------------------------------------------------------------
STATIC void RendererInterface::BufferVertexData( const VertexData* vertData )
{
	FATAL_ASSERTION( vertData->IsBuffered(), "Vertex Data Error", "Cannot buffer vertex data that has no buffer!" );

	BindBufferObject( RendererInterface::ARRAY_BUFFER, vertData->bufferID );
	SendDataToBuffer( RendererInterface::ARRAY_BUFFER, vertData->numberOfVertices * vertData->vertexSizeBytes, vertData->data );
}

//-----------------------------------------------------------------------------------------------
STATIC void RendererInterface::UnbindVertexDataFromShader( const VertexData* vertData, const ShaderPipeline* pipeline )
{
	CachingShaderLoader*& shaderLoader = s_activeRendererInterface->m_activeShaderLoader;

	BindBufferObject( ARRAY_BUFFER, vertData->bufferID );

	unsigned int numberOfAttributes = vertData->attributes.size();
	for( unsigned int i = 0; i < numberOfAttributes; ++i )
	{
		const VertexAttribute& attribute = vertData->attributes[ i ];
		ShaderVariable* attributeVariable = shaderLoader->GetAttributeVariable( pipeline, attribute.shaderVariableName );

		shaderLoader->DisableAttributeVariableInShader( attributeVariable );

		delete attributeVariable;
	}

	//No need to unbind the buffer; the following vertexes should take care of that.
}
#pragma endregion //Convenience Structures
