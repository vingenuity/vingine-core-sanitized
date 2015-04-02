#ifndef INCLUDED_BITMAP_FONT_HPP
#define INCLUDED_BITMAP_FONT_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <map>

#include "Glyph.hpp"

struct Texture;


//-----------------------------------------------------------------------------------------------
struct BitmapFont
{
	typedef std::map< unsigned int, Glyph > GlyphRegistry;
public:
	BitmapFont();
	~BitmapFont();

	const Glyph& GetGlyphForCharacter( char character ) const;
	const Texture* GetTextureAtlas( unsigned int atlasIndex ) const;
	float GetWidthOfCharacterInPixels( char character, float fontHeightPixels ) const;


	//Data Members
	const char*		fontName;
	unsigned int	numberOfTextureAtlases;
	Texture**		textureAtlases;
	float			fontYDistanceFromBaselineEm;
	GlyphRegistry	glyphData;
};



#pragma region Construction/Destruction
//-----------------------------------------------------------------------------------------------
inline BitmapFont::BitmapFont()
	: fontName( nullptr )
	, numberOfTextureAtlases( 0 )
	, textureAtlases( nullptr )
	, fontYDistanceFromBaselineEm( 0.f )
{ }

//-----------------------------------------------------------------------------------------------
inline BitmapFont::~BitmapFont()
{
	delete[] textureAtlases;
}
#pragma endregion //Construction/Destruction

#endif //INCLUDED_BITMAP_FONT_HPP
