#pragma once
#ifndef INCLUDED_RENDERER_INTERFACE_HPP
#define INCLUDED_RENDERER_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include <stack>
#include <vector>

#include "../Math/EngineMath.hpp"
#include "../Math/FloatVector2.hpp"
#include "../Math/FloatVector3.hpp"
#include "../Math/FloatVector4.hpp"
#include "../Math/IntVector2.hpp"
#include "../Math/Float4x4Matrix.hpp"
#include "../Camera.hpp"
#include "../Color.hpp"
#include "Bone.hpp"
#include "Framebuffer.hpp"
#include "Light.hpp"
#include "TextureManager.hpp"

class CachingFontLoader;
class CachingShaderLoader;
struct Material;
struct ShaderPipeline;
struct Texture;
struct VertexData;

#pragma region Rendering Convenience Structures
//-----------------------------------------------------------------------------------------------
struct VertexAttribute;
struct VertexData;
#pragma endregion //Rendering Convenience Structures



//-----------------------------------------------------------------------------------------------
ABSTRACT STATIC class RendererInterface
{
public:
	typedef const char* DefaultAttributeName;
	static const DefaultAttributeName DEFAULT_NAME_Vertex;
	static const DefaultAttributeName DEFAULT_NAME_Color;
	static const DefaultAttributeName DEFAULT_NAME_TextureCoords;
	static const DefaultAttributeName DEFAULT_NAME_Normal;
	static const DefaultAttributeName DEFAULT_NAME_Tangent;
	static const DefaultAttributeName DEFAULT_NAME_Bitangent;
	static const DefaultAttributeName DEFAULT_NAME_BoneIndex0;
	static const DefaultAttributeName DEFAULT_NAME_BoneWeight0;

	typedef unsigned short ArrayType;
	static const ArrayType COLOR_ARRAYS;
	static const ArrayType TEXTURE_COORD_ARRAYS;
	static const ArrayType VERTEX_ARRAYS;

	typedef unsigned short BufferType;
	static const BufferType ARRAY_BUFFER;
	static const BufferType INDEX_BUFFER;

	typedef unsigned short ColorComponents;
	static const ColorComponents COMPONENTS_DEPTH;
	static const ColorComponents COMPONENTS_STENCIL;
	static const ColorComponents ARGB;
	static const ColorComponents RGB;
	static const ColorComponents RGBA;
	static const ColorComponents RGB_16_BIT;
	static const ColorComponents RGBA_16_BIT;

	typedef unsigned short ColorBlendingMode;
	static const ColorBlendingMode NO_COLOR;
	static const ColorBlendingMode CONSTANT_ONE;
	static const ColorBlendingMode ONE_MINUS_DESTINATION_COLOR;
	static const ColorBlendingMode ONE_MINUS_SOURCE_ALPHA;
	static const ColorBlendingMode SOURCE_ALPHA;
	static const ColorBlendingMode ONE_MINUS_SOURCE_COLOR;
	static const ColorBlendingMode SOURCE_COLOR;

	typedef unsigned short CoordinateType;
	static const CoordinateType TYPE_SHORT;
	static const CoordinateType TYPE_INT;
	static const CoordinateType TYPE_FLOAT;
	static const CoordinateType TYPE_DOUBLE;
	static const CoordinateType TYPE_UNSIGNED_BYTE;
	static const CoordinateType TYPE_UNSIGNED_INTEGER;
	static const CoordinateType TYPE_UNSIGNED_SHORT;
	static const CoordinateType TYPE_FOUR_BYTES_AS_INT;

	typedef unsigned short Feature;
	static const Feature COLOR_BLENDING;
	static const Feature DEPTH_TESTING;
	static const Feature FACE_CULLING;
	static const Feature SHAPE_RESTART_INDEXING;
	static const Feature TEXTURES_2D;

	typedef unsigned int QualityLevel;
	static const QualityLevel FASTEST;

	typedef unsigned short Shader;
	static const Shader GEOMETRY_SHADER;
	static const Shader PIXEL_FRAGMENT_SHADER;
	static const Shader VERTEX_SHADER;

	typedef unsigned short Shape;
	static const Shape POINTS;
	static const Shape LINES;
	static const Shape LINE_LOOP;
	static const Shape LINE_STRIP;
	static const Shape TRIANGLES;
	static const Shape TRIANGLE_STRIP;
	static const Shape QUADS;

	typedef unsigned short TextureFilteringMethod;
	static const TextureFilteringMethod NEAREST_NEIGHBOR;
	static const TextureFilteringMethod LINEAR_INTERPOLATION;
	static const TextureFilteringMethod NEAREST_MIPMAP_NEAREST_TEXTURE;
	static const TextureFilteringMethod NEAREST_MIPMAP_INTERPOLATE_TEXTURES;
	static const TextureFilteringMethod INTERPOLATE_MIPMAPS_NEAREST_TEXTURE;
	static const TextureFilteringMethod INTERPOLATE_MIPMAPS_INTERPOLATE_TEXTURES;

	typedef unsigned short TextureWrapMode;
	static const TextureWrapMode CLAMP_TO_EDGE;
	static const TextureWrapMode REPEAT_OVER_GEOMETRY;
	static const TextureWrapMode REPEAT_FLIPPED;

	//Lifecycle
	static void Startup();
	static CachingFontLoader* GetFontLoader() { return s_activeRendererInterface->m_activeFontLoader; }
	static CachingShaderLoader* GetShaderLoader() { return s_activeRendererInterface->m_activeShaderLoader; }
	static TextureManager* GetTextureManager() { return s_activeRendererInterface->m_activeTextureManager; }
	static void Shutdown();

	//Matrix Operations
	static Float4x4Matrix& GetModelMatrix() { return s_activeRendererInterface->m_matrixStack.top(); }
	static const Float4x4Matrix& GetConstModelMatrix() { return s_activeRendererInterface->m_matrixStack.top(); }
	static const Float4x4Matrix& GetViewMatrix() { return s_activeRendererInterface->m_viewMatrix; }
	static const Float4x4Matrix& GetProjectionMatrix() { return s_activeRendererInterface->m_projectionMatrix; }
	static void PopMatrix();
	static void PushMatrix();
	static void RotateWorldAboutAxisDegrees( const FloatVector3& axis, float angleDegrees );
	static void ScaleWorld( float scaleX, float scaleY, float scaleZ );
	static void SetOrthographicProjection( double leftXCoord, double rightXCoord, double bottomYCoord, double topYCoord, double nearClippingPlaneDistance, double farClippingPlaneDistance );
	static void SetPerpectiveProjection( double horizontalFOVDegrees, double aspectRatio, double nearClippingPlaneDistance, double farClippingPlaneDistance );
	static void SetTopOfStackToIdentity();
	static void SetTopOfStackToMatrix( const Float4x4Matrix& matrix );
	static void SetViewMatrix( const Float4x4Matrix& matrix ) { s_activeRendererInterface->m_viewMatrix = matrix; }
	static void SetViewMatrixFromCamera( const Camera& camera );
	static void SetViewMatrixToIdentity() { s_activeRendererInterface->m_viewMatrix = F4X4_IDENTITY_MATRIX; }
	static void TranslateWorld( const FloatVector2& translationDirection );
	static void TranslateWorld( const FloatVector3& translationDirection );

	//Materials
	static Material* CreateOrGetNewMaterial( const std::wstring& materialName );
	static void ApplyMaterial( const Material* material );
	static void RemoveMaterial( const Material* material );

	//Bones
	static void UpdateSkeletonOnMaterial( const Float4x4Matrix& objectStartingTransform, const std::vector< Bone >& skeleton, Material* material );

	//Lights
	static void AddLight( const Light& light ) { s_activeRendererInterface->m_lights.push_back( light ); }
	static void UpdateLightsOnMaterial( Material* material );

	//Convenience Structures
	static void BindVertexDataToShader( const VertexData* vertData, const ShaderPipeline* pipeline );
	static void BufferVertexData( const VertexData* vertData );
	static void UnbindVertexDataFromShader( const VertexData* vertData, const ShaderPipeline* pipeline );



#pragma region Public Static Interface
	//Feature Enabling
	static void EnableArrayType( ArrayType type );
	static void DisableArrayType( ArrayType type );
	static void EnableFeature( Feature feature );
	static void DisableFeature( Feature feature );

	//Color and Depth Buffers
	static void ClearColorBuffer();
	static void ClearDepthBuffer();
	static void SetColorBufferClearValue( float red, float green, float blue, float alpha );
	static void SetDepthBufferClearValue( float depthBetweenZeroAndOne );
	static void DisableDepthBufferWriting();
	static void EnableDepthBufferWriting();
	static void SetViewport( int lowerLeftX, int lowerLeftY, unsigned int viewportWidth, unsigned int viewportHeight );

	//Frame Buffers
	static void AttachTextureToFramebufferColorOutputSlot( Texture* colorTexture, Framebuffer& framebuffer, unsigned int colorSlot );
	static void AttachTextureToFramebufferDepthOutput( Texture* depthTexture, Framebuffer& framebuffer );
	static void AttachTextureToFramebufferStencilOutput( Texture* stencilTexture, Framebuffer& framebuffer );
	static bool CheckIfFramebufferIsReady( const Framebuffer& framebuffer );
	static void ClearFramebufferColorOutputSlot( Framebuffer& framebuffer, unsigned int colorSlot );
	static void ClearFramebufferDepthOutput( Framebuffer& framebuffer );
	static void ClearFramebufferStencilOutput( Framebuffer& framebuffer );
	static Framebuffer CreateFramebufferObject( Framebuffer::Target targetForReadingOrWriting );
	static void DeleteFramebuffer( Framebuffer& framebuffer );
	static void UseDefaultFramebuffer();
	static void UseFrameBuffer( const Framebuffer& framebuffer );

	//Draw modification
	static void SetAlphaBlendingFunction( ColorBlendingMode sourceBlendingFactor, ColorBlendingMode destinationBlendingFactor );
	static void SetColor( float red, float green, float blue, float alpha );
	static void SetLineWidth( float widthPixels );
	static void SetPointSize( float pointSize );

	//Mipmaps
	static void GenerateMipmaps( Feature textureType );
	static void SetMaximumMipmapLevel( Feature textureType, unsigned int maxLevel );
	static void SetMipmapQuality( QualityLevel qualityLevel );

	//Textures
	static void BindTexture( Feature textureType, const Texture* texture );
	static void CreateTextureFrom2DImage( Feature textureType, unsigned int mipmapLevel, ColorComponents cardColorComponentFormat, 
										   unsigned int imageWidth, unsigned int imageHeight, ColorComponents inputColorComponentFormat, 
										   CoordinateType pixelDataType, const void* imageData );
	static void DeleteTextureDataOnCard( Texture* texture );
	static void GenerateTextureIDs( unsigned int numberOfTextureIDs, unsigned int *arrayOfTextureIDs );
	static void SetTextureInputImageAlignment( unsigned int bytePackingOneTwoFourOrEight );
	static void SetActiveTextureUnit( unsigned int textureUnitNumber );
	static void SetTextureMagnificationMode( Feature textureType, TextureFilteringMethod magnificationMethod );
	static void SetTextureMinificationMode( Feature textureType, TextureFilteringMethod minificationMethod );
	static void SetTextureWrappingMode( Feature textureType, TextureWrapMode wrapMode );

	//Vertex/Index Arrays
	static void RenderPartOfArray( Shape drawingShape, unsigned int numberPointsToDraw, CoordinateType indexType, const void* firstIndexToRender );
	static void RenderVertexArray( Shape drawingShape, unsigned int startingArrayIndex, unsigned int numberPointsInArray );
	static void SetPointerToColorArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray );
	static void SetPointerToGenericArray( unsigned int variableLocation, int numberOfVertexCoordinates, CoordinateType coordinateType, bool normalizeData, unsigned int gapBetweenVertices, const void* firstVertexInArray );
	static void SetPointerToTextureCoordinateArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray );
	static void SetPointerToVertexArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray );
	static void SetShapeRestartIndex( unsigned int index );

	//Vertex Buffer Objects
	static void BindBufferObject( BufferType bufferType, unsigned int bufferID );
	static void DeleteBufferObject( unsigned int bufferID );
	static void GenerateBuffer( unsigned int numberOfBuffersToGenerate, unsigned int* arrayOfBufferIDs );
	static void SendDataToBuffer( BufferType bufferType, unsigned int sizeOfBufferBytes, const void* dataToSendToBuffer );
#pragma endregion //Public Static Interface



protected:
	//Rendering Matrices
	std::stack< Float4x4Matrix > m_matrixStack;
	Float4x4Matrix m_viewMatrix;
	Float4x4Matrix m_projectionMatrix;

	//Asset Management Structures
	CachingFontLoader* m_activeFontLoader;
	CachingShaderLoader* m_activeShaderLoader;
	TextureManager* m_activeTextureManager;
	std::map< std::wstring, Material* > m_materials;

	static const unsigned int MAX_LIGHTS_IN_SHADER = 16;
	std::vector< Light > m_lights;

	static const unsigned int MAX_BONES_IN_SHADER = 250;

	RendererInterface();
	virtual ~RendererInterface();


private:
	// Static Member
	static RendererInterface* s_activeRendererInterface;


#pragma region Internal Interface Declarations
	virtual void Initialize() = 0;

	//Feature Enabling
	virtual void DoEnableArrayType( ArrayType type ) const = 0;
	virtual void DoDisableArrayType( ArrayType type ) const = 0;
	virtual void DoEnableFeature( Feature feature ) const = 0;
	virtual void DoDisableFeature( Feature feature ) const = 0;

	//Color and Depth Buffers
	virtual void DoClearColorBuffer() const = 0;
	virtual void DoClearDepthBuffer() const = 0;
	virtual void DoSetColorBufferClearValue( float red, float green, float blue, float alpha ) = 0;
	virtual void DoSetDepthBufferClearValue( float depthBetweenZeroAndOne ) = 0;
	virtual void DoDisableDepthBufferWriting() const = 0;
	virtual void DoEnableDepthBufferWriting() const = 0;
	virtual void DoSetViewport( int lowerLeftX, int lowerLeftY, unsigned int viewportWidth, unsigned int viewportHeight ) = 0;

	//Frame Buffers
	virtual void DoAttachTextureToFramebufferColorOutputSlot( Texture* colorTexture, Framebuffer& framebuffer, unsigned int colorSlot ) = 0;
	virtual void DoAttachTextureToFramebufferDepthOutput( Texture* depthTexture, Framebuffer& framebuffer ) = 0;
	virtual void DoAttachTextureToFramebufferStencilOutput( Texture* stencilTexture, Framebuffer& framebuffer ) = 0;
	virtual bool DoCheckIfFramebufferIsReady( const Framebuffer& framebuffer ) = 0;
	virtual void DoClearFramebufferColorOutputSlot( Framebuffer& framebuffer, unsigned int colorSlot ) = 0;
	virtual void DoClearFramebufferDepthOutput( Framebuffer& framebuffer ) = 0;
	virtual void DoClearFramebufferStencilOutput( Framebuffer& framebuffer ) = 0;
	virtual Framebuffer DoCreateFramebufferObject( Framebuffer::Target targetForReadingOrWriting ) = 0;
	virtual void DoDeleteFramebuffer( Framebuffer& framebuffer ) = 0;
	virtual void DoUseDefaultFramebuffer() = 0;
	virtual void DoUseFrameBuffer( const Framebuffer& framebuffer ) = 0;

	//Draw modification
	virtual void DoSetAlphaBlendingFunction( ColorBlendingMode sourceBlendingFactor, ColorBlendingMode destinationBlendingFactor ) const = 0;
	virtual void DoSetColor( float red, float green, float blue, float alpha ) const = 0;
	virtual void DoSetLineWidth( float widthPixels ) const = 0;
	virtual void DoSetPointSize( float pointSize ) const = 0;

	//Mipmaps
	virtual void DoGenerateMipmaps( Feature textureType ) = 0;
	virtual void DoSetMaximumMipmapLevel( Feature textureType, unsigned int maxLevel ) = 0;
	virtual void DoSetMipmapQuality( QualityLevel qualityLevel ) = 0;

	//Textures
	virtual void DoBindTexture( Feature textureType, const Texture* texture ) const = 0;
	virtual void DoCreateTextureFrom2DImage( Feature textureType, unsigned int mipmapLevel, ColorComponents cardColorComponentFormat, 
		unsigned int imageWidth, unsigned int imageHeight, ColorComponents inputColorComponentFormat, 
		CoordinateType pixelDataType, const void* imageData ) = 0;
	virtual void DoDeleteTextureDataOnCard( Texture* texture ) = 0;
	virtual void DoGenerateTextureIDs( unsigned int numberOfTextureIDs, unsigned int *arrayOfTextureIDs ) = 0;
	virtual void DoSetTextureInputImageAlignment( unsigned int bytePackingOneTwoFourOrEight ) = 0;
	virtual void DoSetActiveTextureUnit( unsigned int textureUnitNumber ) = 0;
	virtual void DoSetTextureMagnificationMode( Feature textureType, TextureFilteringMethod magnificationMethod ) = 0;
	virtual void DoSetTextureMinificationMode( Feature textureType, TextureFilteringMethod minificationMethod ) = 0;
	virtual void DoSetTextureWrappingMode( Feature textureType, TextureWrapMode wrapMode ) = 0;

	//Vertex/Index Arrays
	virtual void DoRenderPartOfArray( Shape drawingShape, unsigned int numberPointsToDraw, CoordinateType indexType, const void* firstIndexToRender ) const = 0;
	virtual void DoRenderVertexArray( Shape drawingShape, unsigned int startingArrayIndex, unsigned int numberPointsInArray ) const = 0;
	virtual void DoSetPointerToColorArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const = 0;
	virtual void DoSetPointerToGenericArray( unsigned int variableLocation, int numberOfVertexCoordinates, CoordinateType coordinateType, bool normalizeData, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const = 0;
	virtual void DoSetPointerToTextureCoordinateArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const = 0;
	virtual void DoSetPointerToVertexArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const = 0;
	virtual void DoSetShapeRestartIndex( unsigned int index ) = 0;

	//Vertex Buffer Objects
	virtual void DoBindBufferObject( BufferType bufferType, unsigned int bufferID ) = 0;
	virtual void DoDeleteBufferObject( unsigned int bufferID ) = 0;
	virtual void DoGenerateBuffer( unsigned int numberOfBuffersToGenerate, unsigned int* arrayOfBufferIDs ) = 0;
	virtual void DoSendDataToBuffer( BufferType bufferType, unsigned int sizeOfBufferBytes, const void* dataToSendToBuffer ) = 0;
#pragma endregion //Internal Interface Declarations
};


//-----------------------------------------------------------------------------------------------
inline RendererInterface::RendererInterface()
	: m_viewMatrix( F4X4_IDENTITY_MATRIX )
	, m_projectionMatrix( F4X4_IDENTITY_MATRIX )
	, m_activeFontLoader( nullptr )
	, m_activeShaderLoader( nullptr )
	, m_activeTextureManager( nullptr )
{
	m_matrixStack.push( F4X4_IDENTITY_MATRIX );
}

#pragma region +++++Renderer Matrix Operations+++++
//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::PopMatrix()
{
	assert( !s_activeRendererInterface->m_matrixStack.empty() );
	s_activeRendererInterface->m_matrixStack.pop();
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::PushMatrix()
{
	s_activeRendererInterface->m_matrixStack.push( s_activeRendererInterface->m_matrixStack.top() );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void GetRotationMatrixForAxisAndAngleDegrees( Float4x4Matrix& out_rotationMatrix, const FloatVector3& axis, float angleDegrees )
{
	float angleRadians = ConvertDegreesToRadians( angleDegrees );
	float cosineAngle = cos( angleRadians );
	float oneMinusCosineAngle = 1.f - cosineAngle;
	float sineAngle = sin( angleRadians );

	//Using the conversion from the Wikipedia rotation matrix page:
	//http://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	out_rotationMatrix( 1, 1 ) = cosineAngle + axis.x * axis.x * oneMinusCosineAngle;
	out_rotationMatrix( 2, 1 ) = axis.x * axis.y * oneMinusCosineAngle - axis.z * sineAngle;
	out_rotationMatrix( 3, 1 ) = axis.x * axis.z * oneMinusCosineAngle + axis.y * sineAngle;
	out_rotationMatrix( 1, 2 ) = axis.y * axis.x * oneMinusCosineAngle + axis.z * sineAngle;
	out_rotationMatrix( 2, 2 ) = cosineAngle + axis.y * axis.y * oneMinusCosineAngle;
	out_rotationMatrix( 3, 2 ) = axis.y * axis.z * oneMinusCosineAngle - axis.x * sineAngle;
	out_rotationMatrix( 1, 3 ) = axis.z * axis.x * oneMinusCosineAngle - axis.y * sineAngle;
	out_rotationMatrix( 2, 3 ) = axis.z * axis.y * oneMinusCosineAngle + axis.x * sineAngle;
	out_rotationMatrix( 3, 3 ) = cosineAngle + axis.z * axis.z * oneMinusCosineAngle;
	out_rotationMatrix( 4, 4 ) = 1.f;
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::RotateWorldAboutAxisDegrees( const FloatVector3& axis, float angleDegrees )
{
	Float4x4Matrix& currentMatrix = GetModelMatrix();

	Float4x4Matrix rotationMatrix = F4X4_IDENTITY_MATRIX;
	GetRotationMatrixForAxisAndAngleDegrees( rotationMatrix, axis, angleDegrees );

	currentMatrix = rotationMatrix * currentMatrix;
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::ScaleWorld( float scaleX, float scaleY, float scaleZ )
{
	Float4x4Matrix& currentMatrix = GetModelMatrix();

	Float4x4Matrix scaleMatrix = F4X4_IDENTITY_MATRIX;
	scaleMatrix( 1, 1 ) = scaleX;
	scaleMatrix( 2, 2 ) = scaleY;
	scaleMatrix( 3, 3 ) = scaleZ;

	currentMatrix = scaleMatrix * currentMatrix;
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetOrthographicProjection( double leftXCoord, double rightXCoord, double bottomYCoord, double topYCoord, double nearClippingPlaneDistance, double farClippingPlaneDistance )
{
	float inverseScreenWidth = 1.f / static_cast< float >( rightXCoord - leftXCoord );
	float inverseScreenHeight = 1.f / static_cast< float >( topYCoord - bottomYCoord );
	float inverseFrustumDepth = 1.f / static_cast< float >( farClippingPlaneDistance - nearClippingPlaneDistance );

	Float4x4Matrix orthographicMatrix = F4X4_IDENTITY_MATRIX;
	orthographicMatrix( 1, 1 ) =  2.f * inverseScreenWidth;
	orthographicMatrix( 2, 2 ) =  2.f * inverseScreenHeight;
	orthographicMatrix( 3, 3 ) = -2.f * inverseFrustumDepth;
	orthographicMatrix( 4, 1 ) = -static_cast< float >( leftXCoord + rightXCoord ) * inverseScreenWidth;
	orthographicMatrix( 4, 2 ) = -static_cast< float >( topYCoord + bottomYCoord ) * inverseScreenHeight;
	orthographicMatrix( 4, 3 ) = -static_cast< float >( farClippingPlaneDistance + nearClippingPlaneDistance ) * inverseFrustumDepth;

	s_activeRendererInterface->m_projectionMatrix = orthographicMatrix;
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetPerpectiveProjection( double horizontalFOVDegrees, double aspectRatio, double nearClippingPlaneDistance, double farClippingPlaneDistance )
{
	float perspectiveScalingFactor = 1.f / tan( ConvertDegreesToRadians( static_cast< float >( horizontalFOVDegrees ) ) * 0.5f );
	float inverseFrustumDepth = 1.f / static_cast< float >( nearClippingPlaneDistance - farClippingPlaneDistance );

	Float4x4Matrix perspectiveMatrix = F4X4_IDENTITY_MATRIX;
	perspectiveMatrix( 1, 1 ) = perspectiveScalingFactor;
	perspectiveMatrix( 2, 2 ) = perspectiveScalingFactor * static_cast< float >( aspectRatio );
	perspectiveMatrix( 3, 3 ) = static_cast< float >( farClippingPlaneDistance + nearClippingPlaneDistance ) * inverseFrustumDepth;
	perspectiveMatrix( 3, 4 ) = -1.f;
	perspectiveMatrix( 4, 3 ) = static_cast< float >( 2.f * farClippingPlaneDistance * nearClippingPlaneDistance ) * inverseFrustumDepth;
	perspectiveMatrix( 4, 4 ) = 0.f;

	s_activeRendererInterface->m_projectionMatrix = perspectiveMatrix;
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetTopOfStackToIdentity()
{
	s_activeRendererInterface->m_matrixStack.top() = F4X4_IDENTITY_MATRIX;
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetTopOfStackToMatrix( const Float4x4Matrix& matrix )
{
	s_activeRendererInterface->m_matrixStack.top() = matrix;
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetViewMatrixFromCamera( const Camera& camera )
{
 	Float4x4Matrix rotationMatrix = F4X4_IDENTITY_MATRIX;
	const EulerAngles& cameraOrientation = camera.GetHeading();
	Float4x4Matrix xRotation, yRotation, zRotation, x2, z2; 
	GetRotationMatrixForAxisAndAngleDegrees( xRotation, FloatVector3( 1.f, 0.f, 0.f ), -cameraOrientation.rollDegreesAboutX );
	GetRotationMatrixForAxisAndAngleDegrees( yRotation, FloatVector3( 0.f, 1.f, 0.f ), -cameraOrientation.pitchDegreesAboutY );
	GetRotationMatrixForAxisAndAngleDegrees( zRotation, FloatVector3( 0.f, 0.f, 1.f ), -cameraOrientation.yawDegreesAboutZ );
	GetRotationMatrixForAxisAndAngleDegrees( x2, FloatVector3( 1.f, 0.f, 0.f ), -90.f );
	GetRotationMatrixForAxisAndAngleDegrees( z2, FloatVector3( 0.f, 0.f, 1.f ),  90.f );
	rotationMatrix = zRotation * yRotation * xRotation * z2 * x2;
 
	Float4x4Matrix translationMatrix = F4X4_IDENTITY_MATRIX;
	const FloatVector3& cameraPosition = camera.GetPosition();
	translationMatrix[ 12 ] = -cameraPosition.x;
	translationMatrix[ 13 ] = -cameraPosition.y;
	translationMatrix[ 14 ] = -cameraPosition.z;
	translationMatrix[ 15 ] = 1.f;

 	s_activeRendererInterface->m_viewMatrix = translationMatrix * rotationMatrix;
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::TranslateWorld( const FloatVector2& translationDirection )
{
	Float4x4Matrix& currentMatrix = GetModelMatrix();

	Float4x4Matrix translationMatrix = F4X4_IDENTITY_MATRIX;
	translationMatrix[ 12 ] = translationDirection.x;
	translationMatrix[ 13 ] = translationDirection.y;
	translationMatrix[ 14 ] = 0.f;
	translationMatrix[ 15 ] = 1.f;

	currentMatrix = translationMatrix * currentMatrix;
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::TranslateWorld( const FloatVector3& translationDirection )
{
	Float4x4Matrix& currentMatrix = GetModelMatrix();

	Float4x4Matrix translationMatrix = F4X4_IDENTITY_MATRIX;
	translationMatrix[ 12 ] = translationDirection.x;
	translationMatrix[ 13 ] = translationDirection.y;
	translationMatrix[ 14 ] = translationDirection.z;
	translationMatrix[ 15 ] = 1.f;

	currentMatrix = translationMatrix * currentMatrix;
}
#pragma endregion


#pragma region Public Static Interface
//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::EnableArrayType( ArrayType type )
{
	s_activeRendererInterface->DoEnableArrayType( type );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::DisableArrayType( ArrayType type )
{
	s_activeRendererInterface->DoDisableArrayType( type );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::EnableFeature( Feature feature )
{
	s_activeRendererInterface->DoEnableFeature( feature );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::DisableFeature( Feature feature )
{
	s_activeRendererInterface->DoDisableFeature( feature );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::ClearColorBuffer()
{
	s_activeRendererInterface->DoClearColorBuffer();
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::ClearDepthBuffer()
{
	s_activeRendererInterface->DoClearDepthBuffer();
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetColorBufferClearValue( float red, float green, float blue, float alpha )
{
	s_activeRendererInterface->DoSetColorBufferClearValue( red, green, blue, alpha );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetDepthBufferClearValue( float depthBetweenZeroAndOne )
{
	s_activeRendererInterface->DoSetDepthBufferClearValue( depthBetweenZeroAndOne );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::DisableDepthBufferWriting()
{
	s_activeRendererInterface->DoDisableDepthBufferWriting();
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::EnableDepthBufferWriting()
{
	s_activeRendererInterface->DoEnableDepthBufferWriting();
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetViewport( int lowerLeftX, int lowerLeftY, unsigned int viewportWidth, unsigned int viewportHeight )
{
	s_activeRendererInterface->DoSetViewport( lowerLeftX, lowerLeftY, viewportWidth, viewportHeight );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::AttachTextureToFramebufferColorOutputSlot( Texture* colorTexture, Framebuffer& framebuffer, unsigned int colorSlot )
{
	s_activeRendererInterface->DoAttachTextureToFramebufferColorOutputSlot( colorTexture, framebuffer, colorSlot );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::AttachTextureToFramebufferDepthOutput( Texture* depthTexture, Framebuffer& framebuffer )
{
	s_activeRendererInterface->DoAttachTextureToFramebufferDepthOutput( depthTexture, framebuffer );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::AttachTextureToFramebufferStencilOutput( Texture* stencilTexture, Framebuffer& framebuffer )
{
	s_activeRendererInterface->DoAttachTextureToFramebufferStencilOutput( stencilTexture, framebuffer );
}

//-----------------------------------------------------------------------------------------------
STATIC inline bool RendererInterface::CheckIfFramebufferIsReady( const Framebuffer& framebuffer )
{
	return s_activeRendererInterface->DoCheckIfFramebufferIsReady( framebuffer );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::ClearFramebufferColorOutputSlot( Framebuffer& framebuffer, unsigned int colorSlot )
{
	s_activeRendererInterface->DoClearFramebufferColorOutputSlot( framebuffer, colorSlot );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::ClearFramebufferDepthOutput( Framebuffer& framebuffer )
{
	s_activeRendererInterface->DoClearFramebufferDepthOutput( framebuffer );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::ClearFramebufferStencilOutput( Framebuffer& framebuffer )
{
	s_activeRendererInterface->DoClearFramebufferStencilOutput( framebuffer );
}

//-----------------------------------------------------------------------------------------------
STATIC inline Framebuffer RendererInterface::CreateFramebufferObject( Framebuffer::Target targetForReadingOrWriting )
{
	return s_activeRendererInterface->DoCreateFramebufferObject( targetForReadingOrWriting );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::DeleteFramebuffer( Framebuffer& framebuffer )
{
	s_activeRendererInterface->DoDeleteFramebuffer( framebuffer );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::UseDefaultFramebuffer()
{
	s_activeRendererInterface->DoUseDefaultFramebuffer();
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::UseFrameBuffer( const Framebuffer& framebuffer )
{
	s_activeRendererInterface->DoUseFrameBuffer( framebuffer );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetAlphaBlendingFunction( ColorBlendingMode sourceBlendingFactor, ColorBlendingMode destinationBlendingFactor )
{
	s_activeRendererInterface->DoSetAlphaBlendingFunction( sourceBlendingFactor, destinationBlendingFactor );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetColor( float red, float green, float blue, float alpha )
{
	s_activeRendererInterface->DoSetColor( red, green, blue, alpha );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetLineWidth( float widthPixels )
{
	s_activeRendererInterface->DoSetLineWidth( widthPixels );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetPointSize( float pointSize )
{
	s_activeRendererInterface->DoSetPointSize( pointSize );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::GenerateMipmaps( Feature textureType )
{
	s_activeRendererInterface->DoGenerateMipmaps( textureType );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetMaximumMipmapLevel( Feature textureType, unsigned int maxLevel )
{
	s_activeRendererInterface->DoSetMaximumMipmapLevel( textureType, maxLevel );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetMipmapQuality( QualityLevel qualityLevel )
{
	s_activeRendererInterface->DoSetMipmapQuality( qualityLevel );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::BindTexture( Feature textureType, const Texture* texture )
{
	s_activeRendererInterface->DoBindTexture( textureType, texture );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::CreateTextureFrom2DImage( Feature textureType, unsigned int mipmapLevel, ColorComponents cardColorComponentFormat, 
	unsigned int imageWidth, unsigned int imageHeight, ColorComponents inputColorComponentFormat, 
	CoordinateType pixelDataType, const void* imageData )
{
	s_activeRendererInterface->DoCreateTextureFrom2DImage( textureType, mipmapLevel, cardColorComponentFormat,
		imageWidth, imageHeight, inputColorComponentFormat, pixelDataType, imageData );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::DeleteTextureDataOnCard( Texture* texture )
{
	s_activeRendererInterface->DoDeleteTextureDataOnCard( texture );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::GenerateTextureIDs( unsigned int numberOfTextureIDs, unsigned int *arrayOfTextureIDs )
{
	s_activeRendererInterface->DoGenerateTextureIDs( numberOfTextureIDs, arrayOfTextureIDs );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetTextureInputImageAlignment( unsigned int bytePackingOneTwoFourOrEight )
{
	s_activeRendererInterface->DoSetTextureInputImageAlignment( bytePackingOneTwoFourOrEight );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetActiveTextureUnit( unsigned int textureUnitNumber )
{
	s_activeRendererInterface->DoSetActiveTextureUnit( textureUnitNumber );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetTextureMagnificationMode( Feature textureType, TextureFilteringMethod magnificationMethod )
{
	s_activeRendererInterface->DoSetTextureMagnificationMode( textureType, magnificationMethod );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetTextureMinificationMode( Feature textureType, TextureFilteringMethod minificationMethod )
{
	s_activeRendererInterface->DoSetTextureMinificationMode( textureType, minificationMethod );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetTextureWrappingMode( Feature textureType, TextureWrapMode wrapMode )
{
	s_activeRendererInterface->DoSetTextureWrappingMode( textureType, wrapMode );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::RenderPartOfArray( Shape drawingShape, unsigned int numberPointsToDraw, CoordinateType indexType, const void* firstIndexToRender )
{
	s_activeRendererInterface->DoRenderPartOfArray( drawingShape, numberPointsToDraw, indexType, firstIndexToRender );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::RenderVertexArray( Shape drawingShape, unsigned int startingArrayIndex, unsigned int numberPointsInArray )
{
	s_activeRendererInterface->DoRenderVertexArray( drawingShape, startingArrayIndex, numberPointsInArray );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetPointerToColorArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray )
{
	s_activeRendererInterface->DoSetPointerToColorArray( coordinatesPerVertex, coordinateType, gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetPointerToGenericArray( unsigned int variableLocation, int numberOfVertexCoordinates, CoordinateType coordinateType, bool normalizeData, unsigned int gapBetweenVertices, const void* firstVertexInArray )
{
	s_activeRendererInterface->DoSetPointerToGenericArray( variableLocation, 
		numberOfVertexCoordinates, coordinateType, normalizeData, gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetPointerToTextureCoordinateArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray )
{
	s_activeRendererInterface->DoSetPointerToTextureCoordinateArray( coordinatesPerVertex, 
		coordinateType, gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetPointerToVertexArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray )
{
	s_activeRendererInterface->DoSetPointerToVertexArray( coordinatesPerVertex, coordinateType, 
		gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SetShapeRestartIndex( unsigned int index )
{
	s_activeRendererInterface->DoSetShapeRestartIndex( index );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::BindBufferObject( BufferType bufferType, unsigned int bufferID )
{
	s_activeRendererInterface->DoBindBufferObject( bufferType, bufferID );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::DeleteBufferObject( unsigned int bufferID )
{
	s_activeRendererInterface->DoDeleteBufferObject( bufferID );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::GenerateBuffer( unsigned int numberOfBuffersToGenerate, unsigned int* arrayOfBufferIDs )
{
	s_activeRendererInterface->DoGenerateBuffer( numberOfBuffersToGenerate, arrayOfBufferIDs );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void RendererInterface::SendDataToBuffer( BufferType bufferType, unsigned int sizeOfBufferBytes, const void* dataToSendToBuffer )
{
	s_activeRendererInterface->DoSendDataToBuffer( bufferType, sizeOfBufferBytes, dataToSendToBuffer );
}
#pragma endregion //Public Static Interface

#endif //INCLUDED_RENDERER_INTERFACE_HPP
