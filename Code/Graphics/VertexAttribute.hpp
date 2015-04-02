#pragma once
#ifndef INCLUDED_VERTEX_ATTRIBUTE_HPP
#define INCLUDED_VERTEX_ATTRIBUTE_HPP

//-----------------------------------------------------------------------------------------------
#include "RendererInterface.hpp"

//-----------------------------------------------------------------------------------------------
struct VertexAttribute
{
	VertexAttribute( const char* variableName, unsigned int numComponents, RendererInterface::CoordinateType type, 
					 bool normalizeFixedPtData, unsigned int bytesToNextInstance, size_t attributeStructureOffset );


	//Data Members
	const char* shaderVariableName;
	unsigned int numberOfComponents;
	RendererInterface::CoordinateType coordinateType;
	bool normalizeFixedPointData;
	unsigned int bytesUntilNextInstance;
	size_t attributeOffsetInStructure;
};



//-----------------------------------------------------------------------------------------------
inline VertexAttribute::VertexAttribute( const char* variableName, unsigned int numComponents, RendererInterface::CoordinateType type, 
										 bool normalizeFixedPtData, unsigned int bytesToNextInstance, size_t attributeStructureOffset )
	: shaderVariableName( variableName )
	, numberOfComponents( numComponents )
	, coordinateType( type )
	, normalizeFixedPointData( normalizeFixedPtData )
	, bytesUntilNextInstance( bytesToNextInstance )
	, attributeOffsetInStructure( attributeStructureOffset )
{ }

#endif //INCLUDED_VERTEX_ATTRIBUTE_HPP
