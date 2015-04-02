#pragma once
#ifndef INCLUDED_CACHING_FONT_LOADER_HPP
#define INCLUDED_CACHING_FONT_LOADER_HPP

//-----------------------------------------------------------------------------------------------
#include <map>

#include "BitmapFont.hpp"


//-----------------------------------------------------------------------------------------------
class CachingFontLoader
{
public:
	CachingFontLoader() { }
	~CachingFontLoader() { }

	BitmapFont* GetOrLoadFontFromXML( const char* fontDefinitionFileLocation, const char* fontAtlasDirectory );

private:
	std::map< const char*, BitmapFont > m_fontRegistry;
};
#endif //INCLUDED_CACHING_FONT_LOADER_HPP