#pragma once
#ifndef INCLUDED_NULL_TEXTURE_MANAGER_HPP
#define INCLUDED_NULL_TEXTURE_MANAGER_HPP

//-----------------------------------------------------------------------------------------------
#include "TextureManager.hpp"


/************************************************************************************************
The null texture manager is the default texture manager for new/unsupported platforms.
All of its functions return a black 32x32 texture that should work on all platforms.
************************************************************************************************/
//-----------------------------------------------------------------------------------------------
class NullTextureManager : public TextureManager
{
	friend class RendererInterface;
	static const unsigned int NULL_TEXTURE_WIDTH = 32;
	static const unsigned int NULL_TEXTURE_HEIGHT = 32;

public:
	Texture* CreateOrGetTexture( const char* textureFileLocation, Texture::FilteringMethod filterMethod, Texture::WrappingMode wrapMode, bool flipTexture = true );


protected:
	NullTextureManager() { }
	~NullTextureManager() { }
};
#endif //INCLUDED_NULL_TEXTURE_MANAGER_HPP
