#pragma once
#ifndef INCLUDED_VERTEX_DATA_HPP
#define INCLUDED_VERTEX_DATA_HPP

//-----------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <vector>

#include "RendererInterface.hpp"
#include "VertexAttribute.hpp"


//-----------------------------------------------------------------------------------------------
struct VertexData
{
public:
	static const unsigned int NO_BUFFER = 0;

	VertexData();
	VertexData( unsigned int numberOfVerts, size_t sizeOfVerts );
	~VertexData();

	void Append( const VertexData& other );
	void Copy( const VertexData& other );
	bool IsBuffered() const { return ( bufferID != NO_BUFFER ); }


	//Data Members
	void* data;
	size_t vertexSizeBytes;
	unsigned int numberOfVertices;
	unsigned int bufferID;
	std::vector< VertexAttribute > attributes;
	RendererInterface::Shape shape;
};



//-----------------------------------------------------------------------------------------------
inline VertexData::VertexData()
	: data( nullptr )
	, vertexSizeBytes( 0 )
	, numberOfVertices( 0 )
	, bufferID( NO_BUFFER )
{ }

//-----------------------------------------------------------------------------------------------
inline VertexData::VertexData( unsigned int numberOfVerts, size_t sizeOfVerts )
	: data( malloc( numberOfVerts * sizeOfVerts ) )
	, vertexSizeBytes( sizeOfVerts )
	, numberOfVertices( numberOfVerts )
	, bufferID( NO_BUFFER )
{ }

//-----------------------------------------------------------------------------------------------
inline VertexData::~VertexData()
{
	//This free is completely safe as long as the structures stored here 
	//	have no vtables or anything that requires a destructor to properly clean up.
	//Since vertex structures should be simple, this should hold true.
	if( data != nullptr )
		free( data );

	if( IsBuffered() )
		RendererInterface::DeleteBufferObject( bufferID );
}



//-----------------------------------------------------------------------------------------------
inline void VertexData::Append( const VertexData& other )
{
	size_t ourSize = ( this->numberOfVertices * this->vertexSizeBytes );
	size_t otherSize = ( other.numberOfVertices * other.vertexSizeBytes );

	data = realloc( data, ourSize + otherSize );
	memcpy( (char*)data + ourSize, other.data, otherSize );
}

//-----------------------------------------------------------------------------------------------
inline void VertexData::Copy( const VertexData& other )
{
	size_t otherSize = ( other.numberOfVertices * other.vertexSizeBytes );

	data = realloc( data, otherSize );
	memcpy( data, other.data, otherSize );
}

#endif //INCLUDED_VERTEX_DATA_HPP
