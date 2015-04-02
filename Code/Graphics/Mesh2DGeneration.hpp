#pragma once
#ifndef INCLUDED_MESH_2D_GENERATION_HPP
#define INCLUDED_MESH_2D_GENERATION_HPP

//-----------------------------------------------------------------------------------------------
#include "../Math/FloatVector2.hpp"
#include "../Color.hpp"
#include "VertexData.hpp"


//-----------------------------------------------------------------------------------------------
void Generate2DPoint( VertexData& out_vertexData, const FloatVector2& centerPosition, float size, const Color& color );


//-----------------------------------------------------------------------------------------------
void Generate2DRectangleFilled( VertexData& out_vertexData, const FloatVector2& lowerLeftCornerPos,
	float width, float height, const Color& fillColor = Color( 255, 255, 255, 255 ) );

void Generate2DRectangleOutline( VertexData& out_vertexData, const FloatVector2& lowerLeftCornerPos,
	float width, float height, const Color& outlineColor = Color( 255, 255, 255, 255 ) );

#endif //INCLUDED_MESH_2D_GENERATION_HPP
