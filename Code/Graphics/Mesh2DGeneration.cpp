#include "Mesh2DGeneration.hpp"


//-----------------------------------------------------------------------------------------------
struct Simple2DVertex
{
	FloatVector2 position;
	Color color;

	Simple2DVertex( ) { }

	Simple2DVertex( float vertexX, float vertexY,
		unsigned char colorR, unsigned char colorG, unsigned char colorB, unsigned char colorA )
		: position( vertexX, vertexY )
		, color( colorR, colorG, colorB, colorA )
	{ }

	Simple2DVertex( float vertexX, float vertexY, const Color& vertexColor )
		: position( vertexX, vertexY )
		, color( vertexColor )
	{ }

	Simple2DVertex( const FloatVector2& vertexPos, const Color& vertexColor )
		: position( vertexPos )
		, color( vertexColor )
	{ }
};



//-----------------------------------------------------------------------------------------------
void Generate2DPoint( VertexData& out_vertexData, const FloatVector2& centerPosition, float size, const Color& color )
{
	if( out_vertexData.data != nullptr )
		free( out_vertexData.data );

	float halfLength = size * 0.5f;

	static const unsigned int NUM_POINT_VERTICES = 4;
	Simple2DVertex* pointVertexArray = new Simple2DVertex[ NUM_POINT_VERTICES ];
	pointVertexArray[ 0] = Simple2DVertex( centerPosition.x - halfLength, centerPosition.y, color );
	pointVertexArray[ 1] = Simple2DVertex( centerPosition.x + halfLength, centerPosition.y, color );
	pointVertexArray[ 2] = Simple2DVertex( centerPosition.x, centerPosition.y - halfLength, color );
	pointVertexArray[ 3] = Simple2DVertex( centerPosition.x, centerPosition.y + halfLength, color );

	out_vertexData.data = &pointVertexArray[0];
	out_vertexData.vertexSizeBytes = sizeof( Simple2DVertex );
	out_vertexData.numberOfVertices = NUM_POINT_VERTICES;
	out_vertexData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Vertex, 2, RendererInterface::TYPE_FLOAT, false, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, position.x ) ) );
	out_vertexData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Color, 4, RendererInterface::TYPE_UNSIGNED_BYTE, true, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, color.r ) ) );
	out_vertexData.shape = RendererInterface::LINES;
}

//-----------------------------------------------------------------------------------------------
void Generate2DRectangleFilled( VertexData& out_vertexData, const FloatVector2& lowerLeftCornerPos,
	float width, float height, const Color& fillColor )
{
	if( out_vertexData.data != nullptr )
		free( out_vertexData.data );

	static const unsigned int NUM_RECTANGLE_VERTICES = 4;
	Simple2DVertex* rectVertexArray = new Simple2DVertex[ NUM_RECTANGLE_VERTICES ];
	rectVertexArray[ 0] = Simple2DVertex( lowerLeftCornerPos.x + width,	lowerLeftCornerPos.y,			fillColor );
	rectVertexArray[ 1] = Simple2DVertex( lowerLeftCornerPos.x,			lowerLeftCornerPos.y,			fillColor );
	rectVertexArray[ 2] = Simple2DVertex( lowerLeftCornerPos.x + width,	lowerLeftCornerPos.y + height,	fillColor );
	rectVertexArray[ 3] = Simple2DVertex( lowerLeftCornerPos.x,			lowerLeftCornerPos.y + height,	fillColor );

	out_vertexData.data = &rectVertexArray[0];
	out_vertexData.vertexSizeBytes = sizeof( Simple2DVertex );
	out_vertexData.numberOfVertices = NUM_RECTANGLE_VERTICES;
	out_vertexData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Vertex, 2, RendererInterface::TYPE_FLOAT, false, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, position.x ) ) );
	out_vertexData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Color, 4, RendererInterface::TYPE_UNSIGNED_BYTE, true, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, color.r ) ) );
	out_vertexData.shape = RendererInterface::TRIANGLE_STRIP;
}

//-----------------------------------------------------------------------------------------------
void Generate2DRectangleOutline( VertexData& out_vertexData, const FloatVector2& lowerLeftCornerPos,
	float width, float height, const Color& outlineColor )
{
	if( out_vertexData.data != nullptr )
		free( out_vertexData.data );

	static const unsigned int NUM_RECTANGLE_VERTICES = 4;
	Simple2DVertex* rectVertexArray = new Simple2DVertex[ NUM_RECTANGLE_VERTICES ];
	rectVertexArray[ 0] = Simple2DVertex( lowerLeftCornerPos.x,			lowerLeftCornerPos.y,			outlineColor );
	rectVertexArray[ 1] = Simple2DVertex( lowerLeftCornerPos.x + width,	lowerLeftCornerPos.y,			outlineColor );
	rectVertexArray[ 2] = Simple2DVertex( lowerLeftCornerPos.x + width,	lowerLeftCornerPos.y + height,	outlineColor );
	rectVertexArray[ 3] = Simple2DVertex( lowerLeftCornerPos.x,			lowerLeftCornerPos.y + height,	outlineColor );

	out_vertexData.data = &rectVertexArray[0];
	out_vertexData.vertexSizeBytes = sizeof( Simple2DVertex );
	out_vertexData.numberOfVertices = NUM_RECTANGLE_VERTICES;
	out_vertexData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Vertex, 2, RendererInterface::TYPE_FLOAT, false, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, position.x ) ) );
	out_vertexData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Color, 4, RendererInterface::TYPE_UNSIGNED_BYTE, true, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, color.r ) ) );
	out_vertexData.shape = RendererInterface::LINE_LOOP;
}
