#pragma once
#ifndef INCLUDED_STB_TEXTURE_MANAGER_HPP
#define INCLUDED_STB_TEXTURE_MANAGER_HPP

//-----------------------------------------------------------------------------------------------
#include "TextureManager.hpp"


/************************************************************************************************
STB Texture Manager uses stb_image as the loader for textures.
************************************************************************************************/
//-----------------------------------------------------------------------------------------------
class STBTextureManager : public TextureManager
{
	friend class RendererInterface;

public:
	Texture* CreateOrGetTexture( const char* textureFileLocation, Texture::FilteringMethod filterMethod, Texture::WrappingMode wrapMode, bool flipTexture = true );


protected:
	STBTextureManager() { }
	~STBTextureManager() { }
};
#endif //INCLUDED_STB_TEXTURE_MANAGER_HPP
