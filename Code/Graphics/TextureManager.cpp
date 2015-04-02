#include "TextureManager.hpp"

#include <stdlib.h>

#include "RendererInterface.hpp"
#include "Texture.hpp"


//-----------------------------------------------------------------------------------------------
VIRTUAL TextureManager::~TextureManager()
{
	for( CachedTextureRegistry::iterator pair = m_cachedTextures.begin();
		pair != m_cachedTextures.end();
		++pair )
	{
		Texture*& texture = pair->second;
		RendererInterface::DeleteTextureDataOnCard( texture );
		delete texture;
	}
	m_cachedTextures.clear();
}

//-----------------------------------------------------------------------------------------------
Texture* TextureManager::CreateFramebufferColorTexture( unsigned int windowXWidth, unsigned int windowYHeight, unsigned short colorComponentType )
{
	Texture* newTexture = new Texture();
	newTexture->widthPixels = windowXWidth;
	newTexture->heightPixels = windowYHeight;
	RendererInterface::GenerateTextureIDs( 1, &newTexture->textureIDOnCard );
	RendererInterface::BindTexture( RendererInterface::TEXTURES_2D, newTexture );

	RendererInterface::SetTextureWrappingMode( RendererInterface::TEXTURES_2D, RendererInterface::CLAMP_TO_EDGE );
	RendererInterface::SetTextureMagnificationMode( RendererInterface::TEXTURES_2D, RendererInterface::LINEAR_INTERPOLATION );
	RendererInterface::SetTextureMinificationMode( RendererInterface::TEXTURES_2D, RendererInterface::LINEAR_INTERPOLATION );

	RendererInterface::CreateTextureFrom2DImage( RendererInterface::TEXTURES_2D, TextureManager::NO_MIPMAPS, colorComponentType, 
										newTexture->widthPixels, newTexture->heightPixels, 
										colorComponentType, RendererInterface::TYPE_UNSIGNED_BYTE, nullptr );
	return newTexture;
}

//-----------------------------------------------------------------------------------------------
Texture* TextureManager::CreateFramebufferDepthTexture( unsigned int windowXWidth, unsigned int windowYHeight )
{
	Texture* newTexture = new Texture();
	newTexture->widthPixels = windowXWidth;
	newTexture->heightPixels = windowYHeight;
	RendererInterface::GenerateTextureIDs( 1, &newTexture->textureIDOnCard );
	RendererInterface::BindTexture( RendererInterface::TEXTURES_2D, newTexture );

	RendererInterface::SetTextureWrappingMode( RendererInterface::TEXTURES_2D, RendererInterface::CLAMP_TO_EDGE );
	RendererInterface::SetTextureMagnificationMode( RendererInterface::TEXTURES_2D, RendererInterface::NEAREST_NEIGHBOR );
	RendererInterface::SetTextureMinificationMode( RendererInterface::TEXTURES_2D, RendererInterface::NEAREST_NEIGHBOR );

	RendererInterface::CreateTextureFrom2DImage( RendererInterface::TEXTURES_2D, TextureManager::NO_MIPMAPS, RendererInterface::COMPONENTS_DEPTH, 
										newTexture->widthPixels, newTexture->heightPixels, 
										RendererInterface::COMPONENTS_DEPTH, RendererInterface::TYPE_UNSIGNED_INTEGER, nullptr );

	return newTexture;
}

//-----------------------------------------------------------------------------------------------
Texture* TextureManager::CreateTextureOfSizeWithColor( unsigned int width, unsigned int height, const Color& color )
{
	static const unsigned int NUMBER_OF_COLOR_COMPONENTS = 4;

	unsigned char redComponentAsChar   = static_cast< unsigned char >( color.r * 255.f );
	unsigned char greenComponentAsChar = static_cast< unsigned char >( color.g * 255.f );
	unsigned char blueComponentAsChar  = static_cast< unsigned char >( color.b * 255.f );
	unsigned char alphaComponentAsChar = static_cast< unsigned char >( color.a * 255.f );

	unsigned int totalNumberOfColorComponents = width * height * NUMBER_OF_COLOR_COMPONENTS;
	unsigned char* textureData = new unsigned char[ totalNumberOfColorComponents ];

	for( unsigned int i = 0; i < totalNumberOfColorComponents; i += NUMBER_OF_COLOR_COMPONENTS )
	{
		textureData[ i     ] = redComponentAsChar;
		textureData[ i + 1 ] = greenComponentAsChar;
		textureData[ i + 2 ] = blueComponentAsChar;
		textureData[ i + 3 ] = alphaComponentAsChar;
	}

	Texture* newTexture = new Texture();

	static const unsigned int BYTE_ALIGNED = 1;
	RendererInterface::SetTextureInputImageAlignment( BYTE_ALIGNED );

	RendererInterface::GenerateTextureIDs( 1, &newTexture->textureIDOnCard );
	RendererInterface::BindTexture( RendererInterface::TEXTURES_2D, newTexture );

	RendererInterface::SetTextureWrappingMode( RendererInterface::TEXTURES_2D, RendererInterface::CLAMP_TO_EDGE );
	RendererInterface::SetTextureMagnificationMode( RendererInterface::TEXTURES_2D, RendererInterface::LINEAR_INTERPOLATION );

	RendererInterface::CreateTextureFrom2DImage( RendererInterface::TEXTURES_2D, 
		TextureManager::NO_MIPMAPS,
		RendererInterface::RGBA,
		newTexture->widthPixels,
		newTexture->heightPixels,
		RendererInterface::RGBA,
		RendererInterface::TYPE_UNSIGNED_BYTE,
		textureData );

	return newTexture;
}

//-----------------------------------------------------------------------------------------------
//This code is based off of the code and suggestions given here: 
//	http://codereview.stackexchange.com/questions/29618/code-review-for-image-flip-algorithm-in-c
void TextureManager::FlipImageBufferVertically( unsigned char* imageBuffer, unsigned int widthPixels, 
												unsigned int heightPixels, unsigned int bytesPerPixel )
{
	size_t sizeOfRowBytes = widthPixels * bytesPerPixel;
	void* temporaryRow = malloc( sizeOfRowBytes );
	//FIX: should start checking for success on all memory allocations

	unsigned char* lowerRow = imageBuffer;
	unsigned char* upperRow = &imageBuffer[ ( heightPixels - 1 ) * sizeOfRowBytes ];
	for ( ; lowerRow < upperRow; lowerRow += sizeOfRowBytes, upperRow -= sizeOfRowBytes )
	{
		memcpy( temporaryRow, lowerRow, sizeOfRowBytes );
		memcpy( lowerRow, upperRow,		sizeOfRowBytes );
		memcpy( upperRow, temporaryRow, sizeOfRowBytes );
	}

	free( temporaryRow );
	temporaryRow = nullptr;
}