#pragma once
#ifndef INCLUDED_MESH_GENERATION_3D_HPP
#define INCLUDED_MESH_GENERATION_3D_HPP

//-----------------------------------------------------------------------------------------------
#include "../Math/FloatVector2.hpp"
#include "../Math/FloatVector3.hpp"
#include "../Color.hpp"
#include "VertexData.hpp"

//-----------------------------------------------------------------------------------------------
//Planes
void GenerateSimplePlane( VertexData& out_vertData, const FloatVector3& centerPos, const FloatVector3& normalVector, 
						  float width, float height, 
						  const Color color = Color( 255, 255, 255, 255 ) );

void GenerateTexturedPlane( VertexData& out_vertData, const FloatVector3& centerPos, const FloatVector3& normalVector,
							float width, float height, const FloatVector2& lowerLeftUV, const FloatVector2& upperRightUV, 
							const Color color = Color( 255, 255, 255, 255 ) );



//-----------------------------------------------------------------------------------------------
//Boxes
void GenerateSimpleBox( VertexData& out_vertData, const FloatVector3& centerPos, 
	float width, float length, float depth,
	const Color color = Color( 255, 255, 255, 255 ) );

#endif //INCLUDED_MESH_GENERATION_3D_HPP
