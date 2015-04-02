#include "STBTextureManager.hpp"

#include "../AssertionError.hpp"
#include "../AssetInterface.hpp"
#include "RendererInterface.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//-----------------------------------------------------------------------------------------------
Texture* STBTextureManager::CreateOrGetTexture( const char* textureFileLocation, 
												Texture::FilteringMethod filterMethod, 
												Texture::WrappingMode wrapMode,
												bool flipTexture )
{
	CachedTextureRegistry::iterator registeredTexture = m_cachedTextures.find( textureFileLocation );
	if( registeredTexture != m_cachedTextures.end() )
		return registeredTexture->second;

	Texture* newTexture = new Texture();

	FILE* textureFile = AssetInterface::OpenAssetAsFile( textureFileLocation, "rb" );
	AssetInterface::SeekInAssetFile( textureFile, 0, SEEK_END );
	size_t textureFileSize = AssetInterface::GetCurrentPositionInAssetFile( textureFile );
	AssetInterface::SeekInAssetFile( textureFile, 0, SEEK_SET );

	unsigned char* rawTextureData = new unsigned char[ textureFileSize ];
	AssetInterface::ReadFromAssetFile( rawTextureData, sizeof( unsigned char ), textureFileSize, textureFile );
	AssetInterface::CloseAssetFile( textureFile );
	RECOVERABLE_ASSERTION( rawTextureData != nullptr, "Texture Loading Error", "Unable to load a texture from file." );

	int numberOfColorComponents, requestedNumberOfColorComponents = 0;
	unsigned char* textureData = stbi_load_from_memory( rawTextureData, textureFileSize, &newTexture->widthPixels, &newTexture->heightPixels,
		&numberOfColorComponents, requestedNumberOfColorComponents );
	delete rawTextureData;

	if( flipTexture )
		FlipImageBufferVertically( textureData, newTexture->widthPixels, newTexture->heightPixels, numberOfColorComponents );
	
	RendererInterface::ColorComponents bufferTextureComponentFormat;
	RendererInterface::CoordinateType cardCoordinateType;
	if( numberOfColorComponents == 4 )
	{
		bufferTextureComponentFormat = RendererInterface::RGBA;
		cardCoordinateType = RendererInterface::TYPE_FOUR_BYTES_AS_INT;
	}
	else
	{
		bufferTextureComponentFormat = RendererInterface::RGB;
		cardCoordinateType = RendererInterface::TYPE_UNSIGNED_BYTE;
	}
	RendererInterface::ColorComponents cardTextureComponentFormat = bufferTextureComponentFormat;
	// FIX: This doesn't correctly handle exotic component formats

	static const unsigned int BYTE_ALIGNED = 1;
	RendererInterface::SetTextureInputImageAlignment( BYTE_ALIGNED );

	RendererInterface::GenerateTextureIDs( 1, &newTexture->textureIDOnCard );
	RendererInterface::BindTexture( RendererInterface::TEXTURES_2D, newTexture );

	RendererInterface::SetTextureWrappingMode( RendererInterface::TEXTURES_2D, wrapMode );

	RendererInterface::SetTextureMagnificationMode( RendererInterface::TEXTURES_2D, RendererInterface::NEAREST_NEIGHBOR );
	RendererInterface::SetTextureMinificationMode( RendererInterface::TEXTURES_2D, filterMethod );

	RendererInterface::CreateTextureFrom2DImage( RendererInterface::TEXTURES_2D, 
		TextureManager::NO_MIPMAPS,
		cardTextureComponentFormat,
		newTexture->widthPixels,
		newTexture->heightPixels,
		bufferTextureComponentFormat,
		cardCoordinateType,
		textureData );

	//RendererInterface::SetMipmapQuality( RendererInterface::FASTEST );
	//RendererInterface::GenerateMipmaps( RendererInterface::TEXTURES_2D );

	stbi_image_free( textureData );
	m_cachedTextures[ textureFileLocation ] = newTexture;
	return newTexture;
}
