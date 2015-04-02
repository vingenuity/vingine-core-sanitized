#ifndef INCLUDED_OPENGL_RENDERER_INTERFACE_HPP
#define INCLUDED_OPENGL_RENDERER_INTERFACE_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "../PlatformSpecificHeaders.hpp"

#include <gl/gl.h>
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#include "glext.h"

#include "RendererInterface.hpp"


#pragma region OpenGL_Function_Pointer_Declarations
//-----------------------------------------------------------------------------------------------
extern PFNGLCLEARDEPTHFPROC				glClearDepthf;

//Vertex Buffers
extern PFNGLBINDBUFFERPROC				 glBindBuffer;
extern PFNGLBUFFERDATAPROC				 glBufferData;
extern PFNGLDELETEBUFFERSPROC			 glDeleteBuffers;
extern PFNGLGENBUFFERSPROC				 glGenBuffers;
extern PFNGLPRIMITIVERESTARTINDEXPROC	 glPrimitiveRestartIndex;
extern PFNGLVERTEXATTRIBPOINTERPROC		 glVertexAttribPointer;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;

//Framebuffers
extern PFNGLBINDFRAMEBUFFERPROC			glBindFramebuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC	glCheckFramebufferStatus;
extern PFNGLDELETEFRAMEBUFFERSPROC		glDeleteFramebuffers;
extern PFNGLDRAWBUFFERSPROC				glDrawBuffers;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC	glFramebufferRenderbuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC	glFramebufferTexture2D;
extern PFNGLGENFRAMEBUFFERSPROC			glGenFramebuffers;

//Mipmaps
extern PFNGLGENERATEMIPMAPPROC	glGenerateMipmap;

//Shaders
extern PFNGLCOMPILESHADERPROC		glCompileShader;
extern PFNGLCREATESHADERPROC		glCreateShader;
extern PFNGLGETSHADERINFOLOGPROC	glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC			glGetShaderiv;
extern PFNGLSHADERSOURCEPROC		glShaderSource;

//Shader Programs
extern PFNGLATTACHSHADERPROC		glAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC  glBindAttribLocation;
extern PFNGLCREATEPROGRAMPROC		glCreateProgram;
extern PFNGLDELETEPROGRAMPROC		glDeleteProgram;
extern PFNGLDELETESHADERPROC		glDeleteShader;
extern PFNGLDETACHSHADERPROC		glDetachShader;
extern PFNGLGETACTIVEATTRIBPROC		glGetActiveAttrib;
extern PFNGLGETACTIVEUNIFORMPROC	glGetActiveUniform;
extern PFNGLGETATTRIBLOCATIONPROC	glGetAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC	glGetUniformLocation;
extern PFNGLGETPROGRAMIVPROC		glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC	glGetProgramInfoLog;
extern PFNGLLINKPROGRAMPROC			glLinkProgram;
extern PFNGLUSEPROGRAMPROC			glUseProgram;

extern PFNGLUNIFORM1FPROC			glUniform1f;
extern PFNGLUNIFORM2FPROC			glUniform2f;
extern PFNGLUNIFORM3FPROC			glUniform3f;
extern PFNGLUNIFORM4FPROC			glUniform4f;

extern PFNGLUNIFORM1FVPROC			glUniform1fv;
extern PFNGLUNIFORM2FVPROC			glUniform2fv;
extern PFNGLUNIFORM3FVPROC			glUniform3fv;
extern PFNGLUNIFORM4FVPROC			glUniform4fv;

extern PFNGLUNIFORM1IPROC			glUniform1i;
extern PFNGLUNIFORM2IPROC			glUniform2i;
extern PFNGLUNIFORM3IPROC			glUniform3i;
extern PFNGLUNIFORM4IPROC			glUniform4i;

extern PFNGLUNIFORM1IVPROC			glUniform1iv;
extern PFNGLUNIFORM2IVPROC			glUniform2iv;
extern PFNGLUNIFORM3IVPROC			glUniform3iv;
extern PFNGLUNIFORM4IVPROC			glUniform4iv;

extern PFNGLUNIFORMMATRIX4FVPROC	glUniformMatrix4fv;

//Textures
extern PFNGLACTIVETEXTUREPROC		glActiveTexture;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
#pragma endregion

//-----------------------------------------------------------------------------------------------
STATIC class OGLRendererInterface : RendererInterface
{
	friend class RendererInterface;

public:
	//Feature Enabling
	void DoEnableArrayType( ArrayType type ) const;
	void DoDisableArrayType( ArrayType type ) const;
	void DoEnableFeature( Feature feature ) const;
	void DoDisableFeature( Feature feature ) const;

	//Color and Depth Buffers
	void DoClearColorBuffer() const;
	void DoClearDepthBuffer() const;
	void DoSetColorBufferClearValue( float red, float green, float blue, float alpha );
	void DoSetDepthBufferClearValue( float depthBetweenZeroAndOne );
	void DoDisableDepthBufferWriting() const;
	void DoEnableDepthBufferWriting() const;
	void DoSetViewport( int lowerLeftX, int lowerLeftY, unsigned int viewportWidth, unsigned int viewportHeight );

	//Frame Buffers
	void DoAttachTextureToFramebufferColorOutputSlot( Texture* colorTexture, Framebuffer& framebuffer, unsigned int colorSlot );
	void DoAttachTextureToFramebufferDepthOutput( Texture* depthTexture, Framebuffer& framebuffer );
	void DoAttachTextureToFramebufferStencilOutput( Texture* stencilTexture, Framebuffer& framebuffer );
	bool DoCheckIfFramebufferIsReady( const Framebuffer& framebuffer );
	void DoClearFramebufferColorOutputSlot( Framebuffer& framebuffer, unsigned int colorSlot );
	void DoClearFramebufferDepthOutput( Framebuffer& framebuffer );
	void DoClearFramebufferStencilOutput( Framebuffer& framebuffer );
	Framebuffer DoCreateFramebufferObject( Framebuffer::Target targetForReadingOrWriting );
	void DoDeleteFramebuffer( Framebuffer& framebuffer );
	void DoUseDefaultFramebuffer();
	void DoUseFrameBuffer( const Framebuffer& framebuffer );

	//Draw Modification
	void DoSetAlphaBlendingFunction( ColorBlendingMode sourceBlendingFactor, ColorBlendingMode destinationBlendingFactor ) const;
	void DoSetColor( float red, float green, float blue, float alpha ) const;
	void DoSetLineWidth( float widthPixels ) const;
	void DoSetPointSize( float pointSize ) const;

	//Mipmaps
	void DoGenerateMipmaps( Feature textureType );
	void DoSetMaximumMipmapLevel( Feature textureType, unsigned int maxLevel );
	void DoSetMipmapQuality( QualityLevel qualityLevel );

	//Textures
	void DoBindTexture( Feature textureType, const Texture* texture ) const;
	void DoCreateTextureFrom2DImage( Feature textureType, unsigned int mipmapLevel, ColorComponents cardColorComponentFormat, 
								   unsigned int imageWidth, unsigned int imageHeight, ColorComponents inputColorComponentFormat, 
								   CoordinateType pixelDataType, const void* imageData );
	void DoDeleteTextureDataOnCard( Texture* texture );
	void DoGenerateTextureIDs( unsigned int numberOfTextureIDs, unsigned int *arrayOfTextureIDs );
	void DoSetActiveTextureUnit( unsigned int textureUnitNumber );
	void DoSetTextureInputImageAlignment( unsigned int bytePackingOneTwoFourOrEight );
	void DoSetTextureMagnificationMode( Feature textureType, TextureFilteringMethod magnificationMethod );
	void DoSetTextureMinificationMode( Feature textureType, TextureFilteringMethod minificationMethod );
	void DoSetTextureWrappingMode( Feature textureType, TextureWrapMode wrapMode );

	//Vertex Arrays
	void DoRenderPartOfArray( Shape drawingShape, unsigned int numberPointsToDraw, CoordinateType indexType, const void* firstIndexToRender ) const;
	void DoRenderVertexArray( Shape drawingShape, unsigned int startingArrayIndex, unsigned int numberPointsInArray ) const;
	void DoSetPointerToColorArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const;
	void DoSetPointerToGenericArray( unsigned int variableLocation, int numberOfVertexCoordinates, CoordinateType coordinateType, bool normalizeData, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const;
	void DoSetPointerToTextureCoordinateArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const;
	void DoSetPointerToVertexArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const;
	void DoSetShapeRestartIndex( unsigned int index );

	//Vertex Buffer Objects
	void DoBindBufferObject( BufferType bufferType, unsigned int bufferID );
	void DoDeleteBufferObject( unsigned int bufferID );
	void DoGenerateBuffer( unsigned int numberOfBuffersToGenerate, unsigned int* arrayOfBufferIDs );
	void DoSendDataToBuffer( BufferType bufferType, unsigned int sizeOfBufferBytes, const void* dataToSendToBuffer );

private:
	//Don't allow other Plebian programmers to call our singleton's constructor.
	OGLRendererInterface()
		: RendererInterface() 
	{ }

	//Copy and assign are not allowed
	OGLRendererInterface( const OGLRendererInterface& );
	void operator=( const OGLRendererInterface& );

	GLenum ConvertFramebufferTargetToOpenGLEnum( Framebuffer::Target target );

	void Initialize();
	void InitializeOpenGLFunctionPointers();
};

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::Initialize()
{
	InitializeOpenGLFunctionPointers();
	SetShapeRestartIndex( 0xFFFF );
}

#pragma region Feature Enabling
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Feature Enabling +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoEnableArrayType( ArrayType /*type*/ ) const { /*glEnableClientState( type );*/ }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoDisableArrayType( ArrayType /*type*/ ) const { /*glDisableClientState( type );*/ }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoEnableFeature( Feature feature ) const { glEnable( feature ); }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoDisableFeature( Feature feature ) const { glDisable( feature ); }
#pragma endregion

#pragma region Color and Depth Buffers
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Color and Depth Buffers +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoClearColorBuffer() const { glClear( GL_COLOR_BUFFER_BIT ); }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoClearDepthBuffer() const { glClear( GL_DEPTH_BUFFER_BIT ); }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetColorBufferClearValue( float red, float green, float blue, float alpha )
{
	glClearColor( red, green, blue, alpha );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetDepthBufferClearValue( float depthBetweenZeroAndOne )
{
	glClearDepthf( depthBetweenZeroAndOne );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoDisableDepthBufferWriting() const { glDepthMask( GL_FALSE ); }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoEnableDepthBufferWriting() const { glDepthMask( GL_TRUE ); }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetViewport( int lowerLeftX, int lowerLeftY, unsigned int viewportWidth, unsigned int viewportHeight )
{
	glViewport( lowerLeftX, lowerLeftY, viewportWidth, viewportHeight );
}
#pragma endregion

#pragma region Framebuffers
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Framebuffers +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoAttachTextureToFramebufferColorOutputSlot( Texture* colorTexture, Framebuffer& framebuffer, unsigned int colorSlot )
{
	assert( colorSlot < Framebuffer::MAXIMUM_COLOR_OUTPUT_TEXTURES );

	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
							GL_COLOR_ATTACHMENT0 + colorSlot, GL_TEXTURE_2D, colorTexture->textureIDOnCard, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedColorTextures[ colorSlot ] = colorTexture;
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoAttachTextureToFramebufferDepthOutput( Texture* depthTexture, Framebuffer& framebuffer )
{
	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
							GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->textureIDOnCard, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedDepthTexture = depthTexture;
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoAttachTextureToFramebufferStencilOutput( Texture* stencilTexture, Framebuffer& framebuffer )
{
	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
							GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilTexture->textureIDOnCard, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedStencilTexture = stencilTexture;
}

//-----------------------------------------------------------------------------------------------
inline bool OGLRendererInterface::DoCheckIfFramebufferIsReady( const Framebuffer& framebuffer )
{
	UseFrameBuffer( framebuffer );
	unsigned int framebufferStatus = glCheckFramebufferStatus( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ) );
	
	assert( framebufferStatus == GL_FRAMEBUFFER_COMPLETE );

	return ( framebufferStatus == GL_FRAMEBUFFER_COMPLETE );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoClearFramebufferColorOutputSlot( Framebuffer& framebuffer, unsigned int colorSlot )
{
	assert( colorSlot < Framebuffer::MAXIMUM_COLOR_OUTPUT_TEXTURES );

	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
							GL_COLOR_ATTACHMENT0 + colorSlot, GL_TEXTURE_2D, 0, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedColorTextures[ colorSlot ] = nullptr;
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoClearFramebufferDepthOutput( Framebuffer& framebuffer )
{
	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
							GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedDepthTexture = nullptr;
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoClearFramebufferStencilOutput( Framebuffer& framebuffer )
{
	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
							GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedStencilTexture = nullptr;
}

//-----------------------------------------------------------------------------------------------
inline Framebuffer OGLRendererInterface::DoCreateFramebufferObject( Framebuffer::Target targetForReadingOrWriting )
{
	unsigned int framebufferID = 0;
	glGenFramebuffers( 1, &framebufferID );
	return Framebuffer( framebufferID, targetForReadingOrWriting );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoDeleteFramebuffer( Framebuffer& framebuffer )
{
	unsigned int framebufferID = framebuffer.GetID();
	glDeleteFramebuffers( 1, &framebufferID );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoUseDefaultFramebuffer()
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoUseFrameBuffer( const Framebuffer& framebuffer )
{
	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
#pragma endregion

#pragma region Draw Modification
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Draw Modification +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetAlphaBlendingFunction( ColorBlendingMode sourceBlendingFactor, ColorBlendingMode destinationBlendingFactor ) const
{
	glBlendFunc( sourceBlendingFactor, destinationBlendingFactor );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetColor( float red, float green, float blue, float alpha ) const
{
	glColor4f( red, green, blue, alpha );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetLineWidth( float widthPixels ) const { glLineWidth( widthPixels ); }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetPointSize( float pointSize ) const
{
	glPointSize( pointSize );
}
#pragma endregion

#pragma region Mipmaps
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Mipmaps +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoGenerateMipmaps( Feature textureType ) { glGenerateMipmap( textureType ); }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetMaximumMipmapLevel( Feature textureType, unsigned int maxLevel ) 
{
	glTexParameteri( textureType, GL_TEXTURE_MAX_LEVEL, maxLevel );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetMipmapQuality( QualityLevel qualityLevel ) { glHint( GL_GENERATE_MIPMAP_HINT, qualityLevel ); }
#pragma endregion


#pragma region Textures
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Textures +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoBindTexture( Feature textureType, const Texture* texture ) const { glBindTexture( textureType, texture->textureIDOnCard ); }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoCreateTextureFrom2DImage( Feature textureType, unsigned int mipmapLevel, ColorComponents cardColorComponentFormat, 
													  unsigned int imageWidth, unsigned int imageHeight, ColorComponents inputColorComponentFormat, 
													  CoordinateType pixelDataType, const void* imageData )
{
	static const unsigned int NO_IMAGE_BORDERS = 0;
	glTexImage2D( textureType, mipmapLevel, cardColorComponentFormat, imageWidth, imageHeight, NO_IMAGE_BORDERS, inputColorComponentFormat, pixelDataType, imageData );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoDeleteTextureDataOnCard( Texture* texture )
{ 
	unsigned int textureID = texture->textureIDOnCard;
	glDeleteTextures(1, (GLuint*) &textureID ); 
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoGenerateTextureIDs( unsigned int numberOfTextureIDs, unsigned int *arrayOfTextureIDs )
{
	glGenTextures( numberOfTextureIDs, (GLuint*) arrayOfTextureIDs );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetActiveTextureUnit( unsigned int textureUnitNumber )
{
	glActiveTexture( GL_TEXTURE0 + textureUnitNumber );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetTextureInputImageAlignment( unsigned int bytePackingOneTwoFourOrEight )
{
	glPixelStorei( GL_UNPACK_ALIGNMENT, bytePackingOneTwoFourOrEight );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetTextureMagnificationMode( Feature textureType, TextureFilteringMethod magnificationMethod )
{
	glTexParameteri( textureType, GL_TEXTURE_MAG_FILTER, magnificationMethod );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetTextureMinificationMode( Feature textureType, TextureFilteringMethod minificationMethod )
{
	glTexParameteri( textureType, GL_TEXTURE_MIN_FILTER, minificationMethod );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetTextureWrappingMode( Feature textureType, TextureWrapMode wrapMode )
{
	glTexParameteri( textureType, GL_TEXTURE_WRAP_S, wrapMode );
	glTexParameteri( textureType, GL_TEXTURE_WRAP_T, wrapMode );
}
#pragma endregion

#pragma region Vertex Arrays
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Vertex Arrays +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoRenderPartOfArray( Shape drawingShape, unsigned int numberPointsToDraw, CoordinateType indexType, const void* firstIndexToRender ) const
{
	//glLoadMatrixf( m_matrixStack.top().GetRawBuffer() );
	glDrawElements( drawingShape, numberPointsToDraw, indexType, firstIndexToRender );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoRenderVertexArray( Shape drawingShape, unsigned int startingArrayIndex, unsigned int numberPointsInArray ) const
{
	//glLoadMatrixf( m_matrixStack.top().GetRawBuffer() );
	glDrawArrays( drawingShape, startingArrayIndex, numberPointsInArray );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetPointerToColorArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const
{
	glColorPointer( coordinatesPerVertex, coordinateType, gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetPointerToGenericArray( unsigned int variableLocation, int numberOfVertexCoordinates, CoordinateType coordinateType, bool normalizeData, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const
{
	glVertexAttribPointer( variableLocation, numberOfVertexCoordinates, coordinateType, normalizeData, gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetPointerToTextureCoordinateArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const
{
	glTexCoordPointer( coordinatesPerVertex, coordinateType, gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetPointerToVertexArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const
{
	glVertexPointer( coordinatesPerVertex, coordinateType, gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSetShapeRestartIndex( unsigned int index )
{
	glPrimitiveRestartIndex( index );
}
#pragma endregion

#pragma region Vertex Buffer Objects
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Vertex Buffer Objects +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoBindBufferObject( BufferType bufferType, unsigned int bufferID ) { glBindBuffer( bufferType, bufferID ); }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoDeleteBufferObject( unsigned int bufferID ) { glDeleteBuffers( 1, &bufferID ); }

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoGenerateBuffer( unsigned int numberOfBuffersToGenerate, unsigned int* arrayOfBufferIDs )
{
	glGenBuffers( numberOfBuffersToGenerate, arrayOfBufferIDs );
}

//-----------------------------------------------------------------------------------------------
inline void OGLRendererInterface::DoSendDataToBuffer( BufferType bufferType, unsigned int sizeOfBufferBytes, const void* dataToSendToBuffer )
{
	glBufferData( bufferType, sizeOfBufferBytes, dataToSendToBuffer, GL_STATIC_DRAW );
}
#pragma endregion

#pragma region Converters
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Converters +++++++++++++++++++++++++++++++++++++++++++++++++++
inline GLenum OGLRendererInterface::ConvertFramebufferTargetToOpenGLEnum( Framebuffer::Target target )
{
	switch( target )
	{
	case Framebuffer::TARGET_FOR_READING:
		return GL_READ_FRAMEBUFFER;
	case Framebuffer::TARGET_FOR_WRITING:
		return GL_DRAW_FRAMEBUFFER;
	case Framebuffer::TARGET_FOR_READING_AND_WRITING:
	default:
		return GL_FRAMEBUFFER;
	}
}
#pragma endregion

#endif //INCLUDED_OPENGL_RENDERER_INTERFACE_HPP
