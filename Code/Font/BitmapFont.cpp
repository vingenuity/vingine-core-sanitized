#include "BitmapFont.hpp"

#include "../Graphics/RendererInterface.hpp"
#include "../XML/pugixml.hpp"
#include "../AssertionError.hpp"
#include "../AssetInterface.hpp"
#include "../StringConversion.hpp"


//-----------------------------------------------------------------------------------------------
const Glyph& BitmapFont::GetGlyphForCharacter( char character ) const
{
	GlyphRegistry::const_iterator fontIterator = glyphData.find( character );
	FATAL_ASSERTION( fontIterator != glyphData.end(), "Font Error", "Unable to find glyph for font." );

	return fontIterator->second;
}

//-----------------------------------------------------------------------------------------------
const Texture* BitmapFont::GetTextureAtlas( unsigned int atlasIndex ) const
{
	FATAL_ASSERTION( atlasIndex < numberOfTextureAtlases, "Font Error",
		"Attempted to access the texture atlas array with an out-of-bounds index." );

	return textureAtlases[ atlasIndex ];
}

//-----------------------------------------------------------------------------------------------
float BitmapFont::GetWidthOfCharacterInPixels( char character, float fontHeightPixels ) const
{
	const Glyph& characterGlyph = GetGlyphForCharacter( character );

	return characterGlyph.xWidthEm * fontHeightPixels;
}
