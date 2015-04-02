#ifndef INCLUDED_GLYPH_HPP
#define INCLUDED_GLYPH_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "../Math/FloatVector2.hpp"

/*
For reference, many of the names used for data members here come from FreeType:
	http://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html
*/
//-----------------------------------------------------------------------------------------------
struct Glyph
{
	Glyph()
		: textureAtlasID( 0 )
		, minNormalizedUVCoord()
		, maxNormalizedUVCoord()
		, xPenAdvanceEm( 0.f )
		, xBearingEm( 0.f )
		, xWidthEm( 0.f )
	{ }

	Glyph( unsigned int textureSheetID,
		const FloatVector2& lowerLeftUVCoord, const FloatVector2& upperRightUVCoord,
		float xPenPosAdvanceInEm, float xBearingInEm, float xWidthInEm )
		   : textureAtlasID( textureSheetID )
		   , minNormalizedUVCoord( lowerLeftUVCoord )
		   , maxNormalizedUVCoord( upperRightUVCoord )
		   , xPenAdvanceEm( xPenPosAdvanceInEm )
		   , xBearingEm( xBearingInEm )
		   , xWidthEm( xWidthInEm )
	{ }


	// Texture Data
	unsigned int textureAtlasID;
	FloatVector2 minNormalizedUVCoord;
	FloatVector2 maxNormalizedUVCoord;

	// Horizontal Typesetting Data
	float xPenAdvanceEm;
	float xBearingEm;
	float xWidthEm;

	// Vertical Type data isn't stored for now... but we certainly could later.
};

#endif //INCLUDED_GLYPH_HPP
