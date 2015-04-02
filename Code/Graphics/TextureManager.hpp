#pragma once
#ifndef INCLUDED_TEXTURE_MANAGER_HPP
#define INCLUDED_TEXTURE_MANAGER_HPP

//-----------------------------------------------------------------------------------------------
#include <map>

#include "../Color.hpp"
#include "../EngineMacros.hpp"
#include "Texture.hpp"


//-----------------------------------------------------------------------------------------------
ABSTRACT class TextureManager
{
	friend class RendererInterface;

protected:
	static const unsigned int NO_MIPMAPS = 0;
	typedef std::map< std::string, Texture* > CachedTextureRegistry;

public:
	virtual Texture* CreateOrGetTexture( const char* textureFileLocation, Texture::FilteringMethod filterMethod, Texture::WrappingMode wrapMode, bool flipTexture = true ) = 0;

	Texture* CreateFramebufferColorTexture( unsigned int windowXWidth, unsigned int windowYHeight, unsigned short colorComponentType );
	Texture* CreateFramebufferDepthTexture( unsigned int windowXWidth, unsigned int windowYHeight );

	Texture* CreateDefaultDiffuseTexture( unsigned int xWidth, unsigned int yHeight );
	Texture* CreateDefaultNormalTexture( unsigned int xWidth, unsigned int yHeight );
	Texture* CreateDefaultSpecularTexture( unsigned int xWidth, unsigned int yHeight );
	Texture* CreateDefaultEmissiveTexture( unsigned int xWidth, unsigned int yHeight );
	Texture* CreateDefaultParallaxTexture( unsigned int xWidth, unsigned int yHeight );

protected:
	//TextureManager();
	virtual ~TextureManager();

	Texture* CreateTextureOfSizeWithColor( unsigned int width, unsigned int height, const Color& color );
	void FlipImageBufferVertically( unsigned char* imageBuffer, unsigned int widthPixels, unsigned int heightPixels, unsigned int bytesPerPixel );

	//Data members
	CachedTextureRegistry m_cachedTextures;
};



//-----------------------------------------------------------------------------------------------
inline Texture* TextureManager::CreateDefaultDiffuseTexture( unsigned int xWidth, unsigned int yHeight )
{
	static const Color WHITE = Color( 255, 255, 255, 255 );

	return CreateTextureOfSizeWithColor( xWidth, yHeight, WHITE );
}

//-----------------------------------------------------------------------------------------------
inline Texture* TextureManager::CreateDefaultNormalTexture( unsigned int xWidth, unsigned int yHeight )
{
	static const Color Z_UP = Color( 127, 127, 255, 255 );

	return CreateTextureOfSizeWithColor( xWidth, yHeight, Z_UP );
}

//-----------------------------------------------------------------------------------------------
inline Texture* TextureManager::CreateDefaultSpecularTexture( unsigned int xWidth, unsigned int yHeight )
{
	static const Color WHITE = Color( 255, 255, 255, 255 );

	return CreateTextureOfSizeWithColor( xWidth, yHeight, WHITE );
}

//-----------------------------------------------------------------------------------------------
inline Texture* TextureManager::CreateDefaultEmissiveTexture( unsigned int xWidth, unsigned int yHeight )
{
	static const Color BLACK = Color( 0, 0, 0, 255 );

	return CreateTextureOfSizeWithColor( xWidth, yHeight, BLACK );
}

//-----------------------------------------------------------------------------------------------
inline Texture* TextureManager::CreateDefaultParallaxTexture( unsigned int xWidth, unsigned int yHeight )
{
	static const Color BLACK = Color( 0, 0, 0, 255 );

	return CreateTextureOfSizeWithColor( xWidth, yHeight, BLACK );
}

#endif //INCLUDED_TEXTURE_MANAGER_HPP
