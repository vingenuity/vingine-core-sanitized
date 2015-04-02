#pragma once
#ifndef INCLUDED_NULL_RENDERER_INTERFACE_HPP
#define INCLUDED_NULL_RENDERER_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include "../EngineMacros.hpp"

#include "RendererInterface.hpp"


//-----------------------------------------------------------------------------------------------
STATIC class NullRendererInterface : public RendererInterface
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
	NullRendererInterface() : RendererInterface() { }

	//Copy and assign are not allowed
	NullRendererInterface( const NullRendererInterface& );
	void operator=( const NullRendererInterface& );

	void Initialize() { }
};



#pragma region Feature Enabling
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Feature Enabling +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoEnableArrayType( ArrayType /*type*/ ) const { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoDisableArrayType( ArrayType /*type*/ ) const { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoEnableFeature( Feature /*feature*/ ) const { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoDisableFeature( Feature /*feature*/ ) const { }
#pragma endregion

#pragma region Color and Depth Buffers
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Color and Depth Buffers +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoClearColorBuffer() const { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoClearDepthBuffer() const { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetColorBufferClearValue( float /*red*/, float /*green*/, float /*blue*/, float /*alpha*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetDepthBufferClearValue( float /*depthBetweenZeroAndOne*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoDisableDepthBufferWriting() const { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoEnableDepthBufferWriting() const { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetViewport( int /*lowerLeftX*/, int /*lowerLeftY*/,
	unsigned int /*viewportWidth*/, unsigned int /*viewportHeight*/ )
{
}
#pragma endregion

#pragma region Framebuffers
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Framebuffers +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoAttachTextureToFramebufferColorOutputSlot( Texture* /*colorTexture*/,
	Framebuffer& /*framebuffer*/, unsigned int /*colorSlot*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoAttachTextureToFramebufferDepthOutput( Texture* /*depthTexture*/, Framebuffer& /*framebuffer*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoAttachTextureToFramebufferStencilOutput( Texture* /*stencilTexture*/, Framebuffer& /*framebuffer*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline bool NullRendererInterface::DoCheckIfFramebufferIsReady( const Framebuffer& /*framebuffer*/ )
{
	return true;
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoClearFramebufferColorOutputSlot( Framebuffer& /*framebuffer*/, unsigned int /*colorSlot*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoClearFramebufferDepthOutput( Framebuffer& /*framebuffer*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoClearFramebufferStencilOutput( Framebuffer& /*framebuffer*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline Framebuffer NullRendererInterface::DoCreateFramebufferObject( Framebuffer::Target /*targetForReadingOrWriting*/ )
{
	return Framebuffer();
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoDeleteFramebuffer( Framebuffer& /*framebuffer*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoUseDefaultFramebuffer()
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoUseFrameBuffer( const Framebuffer& /*framebuffer*/ )
{
}
#pragma endregion

#pragma region Draw Modification
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Draw Modification +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetAlphaBlendingFunction( ColorBlendingMode /*sourceBlendingFactor*/, ColorBlendingMode /*destinationBlendingFactor*/ ) const
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetColor( float /*red*/, float /*green*/, float /*blue*/, float /*alpha*/ ) const
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetLineWidth( float /*widthPixels*/ ) const { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetPointSize( float /*pointSize*/ ) const
{
}
#pragma endregion

#pragma region Mipmaps
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Mipmaps +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoGenerateMipmaps( Feature /*textureType*/ ) { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetMaximumMipmapLevel( Feature /*textureType*/, unsigned int /*maxLevel*/ ) 
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetMipmapQuality( QualityLevel /*qualityLevel*/ ) { }
#pragma endregion



#pragma region Textures
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Textures +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoBindTexture( Feature /*textureType*/, const Texture* /*texture*/ ) const { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoCreateTextureFrom2DImage( Feature /*textureType*/, unsigned int /*mipmapLevel*/, ColorComponents /*cardColorComponentFormat*/, 
	unsigned int /*imageWidth*/, unsigned int /*imageHeight*/, ColorComponents /*inputColorComponentFormat*/, 
	CoordinateType /*pixelDataType*/, const void* /*imageData*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoDeleteTextureDataOnCard( Texture* /*texture*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoGenerateTextureIDs( unsigned int /*numberOfTextureIDs*/, unsigned int* /*arrayOfTextureIDs*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetActiveTextureUnit( unsigned int /*textureUnitNumber*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetTextureInputImageAlignment( unsigned int /*bytePackingOneTwoFourOrEight*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetTextureMagnificationMode( Feature /*textureType*/, TextureFilteringMethod /*magnificationMethod*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetTextureMinificationMode( Feature /*textureType*/, TextureFilteringMethod /*minificationMethod*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetTextureWrappingMode( Feature /*textureType*/, TextureWrapMode /*wrapMode*/ )
{
}
#pragma endregion

#pragma region Vertex Arrays
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Vertex Arrays +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoRenderPartOfArray( Shape /*drawingShape*/, unsigned int /*numberPointsToDraw*/, CoordinateType /*indexType*/, const void* /*firstIndexToRender*/ ) const
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoRenderVertexArray( Shape /*drawingShape*/, unsigned int /*startingArrayIndex*/, unsigned int /*numberPointsInArray*/ ) const
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetPointerToColorArray( unsigned int /*coordinatesPerVertex*/, CoordinateType /*coordinateType*/, unsigned int /*gapBetweenVertices*/, const void* /*firstVertexInArray*/ ) const
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetPointerToGenericArray( unsigned int /*variableLocation*/, int /*numberOfVertexCoordinates*/, CoordinateType /*coordinateType*/, bool /*normalizeData*/, unsigned int /*gapBetweenVertices*/, const void* /*firstVertexInArray*/ ) const
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetPointerToTextureCoordinateArray( unsigned int /*coordinatesPerVertex*/, CoordinateType /*coordinateType*/, unsigned int /*gapBetweenVertices*/, const void* /*firstVertexInArray*/ ) const
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetPointerToVertexArray( unsigned int /*coordinatesPerVertex*/, CoordinateType /*coordinateType*/, unsigned int /*gapBetweenVertices*/, const void* /*firstVertexInArray*/ ) const
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSetShapeRestartIndex( unsigned int /*index*/ )
{
}
#pragma endregion

#pragma region Vertex Buffer Objects
//+++++++++++++++++++++++++++++++++++++++++++++++++++ Vertex Buffer Objects +++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoBindBufferObject( BufferType /*bufferType*/, unsigned int /*bufferID*/ ) { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoDeleteBufferObject( unsigned int /*bufferID*/ ) { }

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoGenerateBuffer( unsigned int /*numberOfBuffersToGenerate*/, unsigned int* /*arrayOfBufferIDs*/ )
{
}

//-----------------------------------------------------------------------------------------------
inline void NullRendererInterface::DoSendDataToBuffer( BufferType /*bufferType*/, unsigned int /*sizeOfBufferBytes*/, const void* /*dataToSendToBuffer*/ )
{
}
#pragma endregion

#endif //INCLUDED_NULL_RENDERER_INTERFACE_HPP
