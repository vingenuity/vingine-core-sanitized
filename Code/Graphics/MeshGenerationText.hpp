#ifndef INCLUDED_MESH_GENERATION_TEXT_HPP
#define INCLUDED_MESH_GENERATION_TEXT_HPP

//-----------------------------------------------------------------------------------------------
#include <string>
#include "../Math/FloatVector2.hpp"
#include "../Color.hpp"
#include "VertexData.hpp"

struct BitmapFont;

//-----------------------------------------------------------------------------------------------
float CalculateTextWidth( const std::string& textString, const BitmapFont* font, float textHeight );
float CalculateTextWidthFrom( const std::string& textString, const BitmapFont* font, float textHeight,
	unsigned int startIndex, unsigned int endIndex );

//-----------------------------------------------------------------------------------------------
void GenerateTextMesh( VertexData& out_vertices, const std::string& textString, const FloatVector2& baselineOriginPosition,
					   const Color& textColor, const BitmapFont* font, float fontHeightPixels );
void GenerateShadowedTextMesh();

#endif //INCLUDED_MESH_GENERATION_TEXT_HPP