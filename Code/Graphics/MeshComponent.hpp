#pragma once
#ifndef INCLUDED_MESH_COMPONENT_HPP
#define INCLUDED_MESH_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include "../Component.hpp"
#include "VertexData.hpp"

struct Material;

//-----------------------------------------------------------------------------------------------
struct MeshComponent : public Component
{
	MeshComponent();
	~MeshComponent();


	//Data Members
	bool vertexDataIsFlyweight;
	VertexData* vertexData;
	//IndexData* indexData;
	Material* material;
};



//-----------------------------------------------------------------------------------------------
inline MeshComponent::MeshComponent()
	: vertexDataIsFlyweight( false )
	, vertexData( nullptr )
	, material( nullptr )
{ }

//-----------------------------------------------------------------------------------------------
inline MeshComponent::~MeshComponent()
{
	if( !vertexDataIsFlyweight && vertexData != nullptr )
		delete vertexData;
}

#endif //INCLUDED_MESH_COMPONENT_HPP
