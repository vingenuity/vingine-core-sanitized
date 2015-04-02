#include "MeshGenerationText.hpp"

#include "../Font/BitmapFont.hpp"
#include "../AssertionError.hpp"
#include "VertexData.hpp"

struct SimpleTextVertex
{
	FloatVector2 position;
	Color textColor;
	FloatVector2 textureUV;

	//Everything should initialize itself correctly automatically
	SimpleTextVertex() { }

	SimpleTextVertex( const FloatVector2& vertPos, const Color& textColors, const FloatVector2& vertUV )
		: position( vertPos )
		, textColor( textColors )
		, textureUV( vertUV )
	{ }
};


//-----------------------------------------------------------------------------------------------
float CalculateTextWidth( const std::string& textString, const BitmapFont* font, float textHeight )
{
	return CalculateTextWidthFrom( textString, font, textHeight, 0, textString.length() );
}

//-----------------------------------------------------------------------------------------------
float CalculateTextWidthFrom( const std::string& textString, const BitmapFont* font, float textHeight,
	unsigned int startIndex, unsigned int endIndex )
{
	FATAL_ASSERTION( ( startIndex < textString.length() ) && ( endIndex <= textString.length() ),
		"Text Generation Error", "Index given to calculate text width is out-of-bounds." );

	RECOVERABLE_ASSERTION( startIndex < endIndex, "Text Generation Error",
		"The start index given to calculate text width is less than or equal to the end index.\nDid you mean to do that?" );

	float textWidth = 0.f;
	for( unsigned int i = startIndex; i < endIndex; ++i )
	{
		const Glyph& glyph = font->GetGlyphForCharacter( textString[ i ] );

		textWidth += glyph.xPenAdvanceEm * textHeight;
	}
	return textWidth;
}

//-----------------------------------------------------------------------------------------------
void GenerateTextMesh( VertexData& out_vertices, const std::string& textString, const FloatVector2& baselineOriginPosition,
					   const Color& textColor, const BitmapFont* font, float fontHeightPixels )
{
	static const unsigned int VERTICES_PER_GLYPH = 6; // four vertices for the glyph, two for sewing strips together.
	unsigned int totalTextVertices = ( VERTICES_PER_GLYPH * textString.length() ) - 2; //The last two strip sewing vertices are unneeded

	if( out_vertices.data != nullptr )
		free( out_vertices.data );

	SimpleTextVertex* textVertArray = new SimpleTextVertex[ totalTextVertices ];

	FloatVector2 penPosition = baselineOriginPosition;
	float letterMinYPixels = penPosition.y + ( font->fontYDistanceFromBaselineEm * fontHeightPixels );
	float letterMaxYPixels = penPosition.y + fontHeightPixels;

	for( unsigned int i = 0; i < textString.length(); ++i )
	{
		const Glyph& glyph = font->GetGlyphForCharacter( textString[ i ] );

		float letterMinXPixels = penPosition.x + ( glyph.xBearingEm * fontHeightPixels );
		float letterMaxXPixels = letterMinXPixels + ( glyph.xWidthEm * fontHeightPixels );

		textVertArray[ VERTICES_PER_GLYPH * i + 0 ] = SimpleTextVertex( FloatVector2( letterMinXPixels, letterMinYPixels ), textColor, FloatVector2( glyph.minNormalizedUVCoord.x, glyph.maxNormalizedUVCoord.y ) );
		textVertArray[ VERTICES_PER_GLYPH * i + 1 ] = SimpleTextVertex( FloatVector2( letterMaxXPixels, letterMinYPixels ), textColor, glyph.maxNormalizedUVCoord );
		textVertArray[ VERTICES_PER_GLYPH * i + 2 ] = SimpleTextVertex( FloatVector2( letterMinXPixels, letterMaxYPixels ), textColor, glyph.minNormalizedUVCoord );
		textVertArray[ VERTICES_PER_GLYPH * i + 3 ] = SimpleTextVertex( FloatVector2( letterMaxXPixels, letterMaxYPixels ), textColor, FloatVector2( glyph.maxNormalizedUVCoord.x, glyph.minNormalizedUVCoord.y ) );

		penPosition.x += glyph.xPenAdvanceEm * fontHeightPixels;

		//Create four degenerate triangles to sew the triangle strips together.
		//On the last loop, we can't look ahead, so just don't do it at all (and save four vertices in the bargain.)
		if( i + 1 != textString.length() )
		{
			textVertArray[ VERTICES_PER_GLYPH * i + 4 ] = textVertArray[ VERTICES_PER_GLYPH * i + 3 ];

			const Glyph& nextGlyph = font->GetGlyphForCharacter( textString[ i + 1 ] );
			textVertArray[ VERTICES_PER_GLYPH * i + 5 ] = SimpleTextVertex( FloatVector2( penPosition.x + ( nextGlyph.xBearingEm * fontHeightPixels ), letterMinYPixels ), textColor, FloatVector2( 0.f, 0.f ) );
		}
	}

	out_vertices.data = textVertArray;
	out_vertices.vertexSizeBytes = sizeof( SimpleTextVertex );
	out_vertices.shape = RendererInterface::TRIANGLE_STRIP;
	out_vertices.numberOfVertices = totalTextVertices;
	out_vertices.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Vertex, 2, RendererInterface::TYPE_FLOAT, false, sizeof( SimpleTextVertex ), offsetof( SimpleTextVertex, position.x ) ) );
	out_vertices.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Color, 4, RendererInterface::TYPE_UNSIGNED_BYTE, true, sizeof( SimpleTextVertex ), offsetof( SimpleTextVertex, textColor.r ) ) );
	out_vertices.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_TextureCoords, 2, RendererInterface::TYPE_FLOAT, false, sizeof( SimpleTextVertex ), offsetof( SimpleTextVertex, textureUV.x ) ) );
}

//-----------------------------------------------------------------------------------------------
void GenerateShadowedTextMesh()
{

}