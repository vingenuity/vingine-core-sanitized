#pragma once
#ifndef INCLUDED_OPENGL_ES2_RENDERER_INTERFACE_HPP
#define INCLUDED_OPENGL_ES2_RENDERER_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include "../EngineMacros.hpp"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "../AssertionError.hpp"
#include "RendererInterface.hpp"


//-----------------------------------------------------------------------------------------------
STATIC class OGLES2RendererInterface : public RendererInterface
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
	OGLES2RendererInterface() : RendererInterface() { }

	//Copy and assign are not allowed
	OGLES2RendererInterface( const OGLES2RendererInterface& );
	void operator=( const OGLES2RendererInterface& );

	GLenum ConvertFramebufferTargetToOpenGLEnum( Framebuffer::Target target );

	void Initialize() { }
};



#pragma region Feature Enabling
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Feature Enabling +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoEnableArrayType( ArrayType /*type*/ ) const { /*glEnableClientState( type );*/ }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoDisableArrayType( ArrayType /*type*/ ) const { /*glDisableClientState( type );*/ }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoEnableFeature( Feature feature ) const { glEnable( feature ); }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoDisableFeature( Feature feature ) const { glDisable( feature ); }
#pragma endregion

#pragma region Color and Depth Buffers
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Color and Depth Buffers +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoClearColorBuffer() const { glClear( GL_COLOR_BUFFER_BIT ); }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoClearDepthBuffer() const { glClear( GL_DEPTH_BUFFER_BIT ); }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetColorBufferClearValue( float red, float green, float blue, float alpha )
{
	glClearColor( red, green, blue, alpha );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetDepthBufferClearValue( float depthBetweenZeroAndOne )
{
	glClearDepthf( depthBetweenZeroAndOne );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoDisableDepthBufferWriting() const { glDepthMask( GL_FALSE ); }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoEnableDepthBufferWriting() const { glDepthMask( GL_TRUE ); }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetViewport( int lowerLeftX, int lowerLeftY, unsigned int viewportWidth, unsigned int viewportHeight )
{
	glViewport( lowerLeftX, lowerLeftY, viewportWidth, viewportHeight );
}
#pragma endregion

#pragma region Framebuffers
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Framebuffers +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoAttachTextureToFramebufferColorOutputSlot( Texture* colorTexture, Framebuffer& framebuffer, unsigned int colorSlot )
{
	assert( colorSlot < Framebuffer::MAXIMUM_COLOR_OUTPUT_TEXTURES );

	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
		GL_COLOR_ATTACHMENT0 + colorSlot, GL_TEXTURE_2D, colorTexture->textureIDOnCard, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedColorTextures[ colorSlot ] = colorTexture;
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoAttachTextureToFramebufferDepthOutput( Texture* depthTexture, Framebuffer& framebuffer )
{
	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
		GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->textureIDOnCard, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedDepthTexture = depthTexture;
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoAttachTextureToFramebufferStencilOutput( Texture* stencilTexture, Framebuffer& framebuffer )
{
	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
		GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilTexture->textureIDOnCard, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedStencilTexture = stencilTexture;
}

//-----------------------------------------------------------------------------------------------
inline bool OGLES2RendererInterface::DoCheckIfFramebufferIsReady( const Framebuffer& framebuffer )
{
	UseFrameBuffer( framebuffer );
	unsigned int framebufferStatus = glCheckFramebufferStatus( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ) );

	if( framebufferStatus != GL_FRAMEBUFFER_COMPLETE )
	{
		switch( framebufferStatus )
		{
// 		case GL_FRAMEBUFFER_UNDEFINED:
// 			RECOVERABLE_ERROR( "Framebuffer Error", "The default framebuffer is not ready for rendering.\nThe default framebuffer does not exist.\n" );
// 			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			RECOVERABLE_ERROR( "Framebuffer Error", "The framebuffer is not ready for rendering.\nOne or more of the framebuffer attachments are incomplete.\n" );
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			RECOVERABLE_ERROR( "Framebuffer Error", "The framebuffer is not ready for rendering.\nThe framebuffer does not have enough attachments to render.\n" );
			break;
// 		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
// 			RECOVERABLE_ERROR( "Framebuffer Error", "The draw framebuffer is not ready for rendering.\nOne or more of the framebuffer attachments has object type GL_NONE.\n" );
// 			break;
// 		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
// 			RECOVERABLE_ERROR( "Framebuffer Error", "The read framebuffer is not ready for rendering.\nOne or more of the framebuffer attachments has object type GL_NONE.\n" );
// 			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			RECOVERABLE_ERROR( "Framebuffer Error", "The framebuffer is not ready for rendering.\nOne or more of the framebuffer attachments' imageformats is against platform specifications.\n" );
			break;
// 		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
// 			RECOVERABLE_ERROR( "Framebuffer Error", "The framebuffer is not ready for rendering.\nThe texture or renderbuffer sample settings for the attachments are not set the same.\n" );
// 			break;
// 		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
// 			RECOVERABLE_ERROR( "Framebuffer Error", "This framebuffer is not ready for rendering.\nA layered framebuffer is being used, but one or more attachments are not layered!.\n" );
// 			break;
		default:
			RECOVERABLE_ERROR( "Framebuffer Error", "An unknown issue is preventing the framebuffer from being ready.\n" );
		}
	}

	return true;
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoClearFramebufferColorOutputSlot( Framebuffer& framebuffer, unsigned int colorSlot )
{
	assert( colorSlot < Framebuffer::MAXIMUM_COLOR_OUTPUT_TEXTURES );

	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
		GL_COLOR_ATTACHMENT0 + colorSlot, GL_TEXTURE_2D, 0, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedColorTextures[ colorSlot ] = nullptr;
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoClearFramebufferDepthOutput( Framebuffer& framebuffer )
{
	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
		GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedDepthTexture = nullptr;
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoClearFramebufferStencilOutput( Framebuffer& framebuffer )
{
	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );

	static const int DEFAULT_MIPMAP_LEVEL = 0;
	glFramebufferTexture2D( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), 
		GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, DEFAULT_MIPMAP_LEVEL );
	framebuffer.m_attachedStencilTexture = nullptr;
}

//-----------------------------------------------------------------------------------------------
inline Framebuffer OGLES2RendererInterface::DoCreateFramebufferObject( Framebuffer::Target targetForReadingOrWriting )
{
	unsigned int framebufferID = 0;
	glGenFramebuffers( 1, &framebufferID );
	return Framebuffer( framebufferID, targetForReadingOrWriting );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoDeleteFramebuffer( Framebuffer& framebuffer )
{
	unsigned int framebufferID = framebuffer.GetID();
	glDeleteFramebuffers( 1, &framebufferID );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoUseDefaultFramebuffer()
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoUseFrameBuffer( const Framebuffer& framebuffer )
{
	glBindFramebuffer( ConvertFramebufferTargetToOpenGLEnum( framebuffer.GetOutputTarget() ), framebuffer.GetID() );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
#pragma endregion

#pragma region Draw Modification
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Draw Modification +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetAlphaBlendingFunction( ColorBlendingMode sourceBlendingFactor, ColorBlendingMode destinationBlendingFactor ) const
{
	glBlendFunc( sourceBlendingFactor, destinationBlendingFactor );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetColor( float red, float green, float blue, float alpha ) const
{
	RECOVERABLE_ERROR( "OpenGL ES2 Interface Error",
		"This function is unsupported by OpenGL ES2. Use a different rendering interface to use this functionality." );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetLineWidth( float widthPixels ) const { glLineWidth( widthPixels ); }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetPointSize( float pointSize ) const
{
	RECOVERABLE_ERROR( "OpenGL ES2 Interface Error",
		"This function is unsupported by OpenGL ES2. Use a different rendering interface to use this functionality." );
}
#pragma endregion

#pragma region Mipmaps
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Mipmaps +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoGenerateMipmaps( Feature textureType ) { glGenerateMipmap( textureType ); }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetMaximumMipmapLevel( Feature textureType, unsigned int maxLevel ) 
{
	RECOVERABLE_ERROR( "OpenGL ES2 Interface Error",
		"This function is unsupported by OpenGL ES2. Use a different rendering interface to use this functionality." );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetMipmapQuality( QualityLevel qualityLevel ) { glHint( GL_GENERATE_MIPMAP_HINT, qualityLevel ); }
#pragma endregion



#pragma region Textures
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Textures +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoBindTexture( Feature textureType, const Texture* texture ) const { glBindTexture( textureType, texture->textureIDOnCard ); }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoCreateTextureFrom2DImage( Feature textureType, unsigned int mipmapLevel, ColorComponents cardColorComponentFormat, 
	unsigned int imageWidth, unsigned int imageHeight, ColorComponents inputColorComponentFormat, 
	CoordinateType pixelDataType, const void* imageData )
{
	static const unsigned int NO_IMAGE_BORDERS = 0;
	glTexImage2D( textureType, mipmapLevel, cardColorComponentFormat, imageWidth, imageHeight, NO_IMAGE_BORDERS, inputColorComponentFormat, pixelDataType, imageData );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoDeleteTextureDataOnCard( Texture* texture )
{ 
	unsigned int textureID = texture->textureIDOnCard;
	glDeleteTextures(1, (GLuint*) &textureID ); 
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoGenerateTextureIDs( unsigned int numberOfTextureIDs, unsigned int *arrayOfTextureIDs )
{
	glGenTextures( numberOfTextureIDs, (GLuint*) arrayOfTextureIDs );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetActiveTextureUnit( unsigned int textureUnitNumber )
{
	glActiveTexture( GL_TEXTURE0 + textureUnitNumber );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetTextureInputImageAlignment( unsigned int bytePackingOneTwoFourOrEight )
{
	glPixelStorei( GL_UNPACK_ALIGNMENT, bytePackingOneTwoFourOrEight );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetTextureMagnificationMode( Feature textureType, TextureFilteringMethod magnificationMethod )
{
	glTexParameteri( textureType, GL_TEXTURE_MAG_FILTER, magnificationMethod );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetTextureMinificationMode( Feature textureType, TextureFilteringMethod minificationMethod )
{
	glTexParameteri( textureType, GL_TEXTURE_MIN_FILTER, minificationMethod );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetTextureWrappingMode( Feature textureType, TextureWrapMode wrapMode )
{
	glTexParameteri( textureType, GL_TEXTURE_WRAP_S, wrapMode );
	glTexParameteri( textureType, GL_TEXTURE_WRAP_T, wrapMode );
}
#pragma endregion

#pragma region Vertex Arrays
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Vertex Arrays +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoRenderPartOfArray( Shape drawingShape, unsigned int numberPointsToDraw, CoordinateType indexType, const void* firstIndexToRender ) const
{
	//glLoadMatrixf( m_matrixStack.top().GetRawBuffer() );
	glDrawElements( drawingShape, numberPointsToDraw, indexType, firstIndexToRender );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoRenderVertexArray( Shape drawingShape, unsigned int startingArrayIndex, unsigned int numberPointsInArray ) const
{
	//glLoadMatrixf( m_matrixStack.top().GetRawBuffer() );
	glDrawArrays( drawingShape, startingArrayIndex, numberPointsInArray );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetPointerToColorArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const
{
	RECOVERABLE_ERROR( "OpenGL ES2 Interface Error",
		"This function is unsupported by OpenGL ES2. Use a different rendering interface to use this functionality." );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetPointerToGenericArray( unsigned int variableLocation, int numberOfVertexCoordinates, CoordinateType coordinateType, bool normalizeData, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const
{
	glVertexAttribPointer( variableLocation, numberOfVertexCoordinates, coordinateType, normalizeData, gapBetweenVertices, firstVertexInArray );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetPointerToTextureCoordinateArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const
{
	RECOVERABLE_ERROR( "OpenGL ES2 Interface Error",
		"This function is unsupported by OpenGL ES2. Use a different rendering interface to use this functionality." );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetPointerToVertexArray( unsigned int coordinatesPerVertex, CoordinateType coordinateType, unsigned int gapBetweenVertices, const void* firstVertexInArray ) const
{
	RECOVERABLE_ERROR( "OpenGL ES2 Interface Error",
		"This function is unsupported by OpenGL ES2. Use a different rendering interface to use this functionality." );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSetShapeRestartIndex( unsigned int index )
{
	RECOVERABLE_ERROR( "OpenGL ES2 Interface Error",
		"This function is unsupported by OpenGL ES2. Use a different rendering interface to use this functionality." );
}
#pragma endregion

#pragma region Vertex Buffer Objects
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Vertex Buffer Objects +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoBindBufferObject( BufferType bufferType, unsigned int bufferID ) { glBindBuffer( bufferType, bufferID ); }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoDeleteBufferObject( unsigned int bufferID ) { glDeleteBuffers( 1, &bufferID ); }

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoGenerateBuffer( unsigned int numberOfBuffersToGenerate, unsigned int* arrayOfBufferIDs )
{
	glGenBuffers( numberOfBuffersToGenerate, arrayOfBufferIDs );
}

//-----------------------------------------------------------------------------------------------
inline void OGLES2RendererInterface::DoSendDataToBuffer( BufferType bufferType, unsigned int sizeOfBufferBytes, const void* dataToSendToBuffer )
{
	glBufferData( bufferType, sizeOfBufferBytes, dataToSendToBuffer, GL_STATIC_DRAW );
}
#pragma endregion

#pragma region Converters
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Converters +++++++++++++++++++++++++++++++++++++++++++++++++++
inline GLenum OGLES2RendererInterface::ConvertFramebufferTargetToOpenGLEnum( Framebuffer::Target target )
{
	// GL_READ_FRAMEBUFFER and GL_DRAW_FRAMEBUFFER are unsupported by OpenGL ES2.
	return GL_FRAMEBUFFER;
}
#pragma endregion

#endif //INCLUDED_OPENGL_ES2_RENDERER_INTERFACE_HPP