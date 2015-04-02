#include "MeshGeneration3D.hpp"

#include <cmath>
#include "../AssertionError.hpp"
#include "../Math/EngineMath.hpp"


#pragma region Vertex Structures
//-----------------------------------------------------------------------------------------------
struct Simple3DVertex
{
	FloatVector3 position;
	Color color;

	Simple3DVertex( ) { }

	Simple3DVertex( float vertexX, float vertexY, float vertexZ,
			unsigned char colorR, unsigned char colorG, unsigned char colorB, unsigned char colorA )
		: position( vertexX, vertexY, vertexZ )
		, color( colorR, colorG, colorB, colorA )
	{ }

	Simple3DVertex( float vertexX, float vertexY, float vertexZ, const Color& vertexColor )
		: position( vertexX, vertexY, vertexZ )
		, color( vertexColor )
	{ }

	Simple3DVertex( const FloatVector3& vertexPos, const Color& vertexColor )
		: position( vertexPos )
		, color( vertexColor )
	{ }
};

//-----------------------------------------------------------------------------------------------
struct Textured3DVertex
{
	FloatVector3 position;
	Color color;
	FloatVector2 textureUV;

	Textured3DVertex( ) { }

	Textured3DVertex( float vertexX, float vertexY, float vertexZ,
		unsigned char colorR, unsigned char colorG, unsigned char colorB, unsigned char colorA,
		float texU, float texV )
		: position( vertexX, vertexY, vertexZ )
		, color( colorR, colorG, colorB, colorA )
		, textureUV( texU, texV )
	{ }

	Textured3DVertex( float vertexX, float vertexY, float vertexZ, const Color& vertexColor, const FloatVector2& texUV )
		: position( vertexX, vertexY, vertexZ )
		, color( vertexColor )
		, textureUV( texUV )
	{ }

	Textured3DVertex( float vertexX, float vertexY, float vertexZ, const Color& vertexColor, float texU, float texV )
		: position( vertexX, vertexY, vertexZ )
		, color( vertexColor )
		, textureUV( texU, texV )
	{ }

	Textured3DVertex( const FloatVector3& vertexPos, const Color& vertexColor, const FloatVector2& texUV )
		: position( vertexPos )
		, color( vertexColor )
		, textureUV( texUV )
	{ }

	Textured3DVertex( const FloatVector3& vertexPos, const Color& vertexColor, float texU, float texV )
		: position( vertexPos )
		, color( vertexColor )
		, textureUV( texU, texV )
	{ }
};
#pragma endregion //Vertex Structures



#pragma region Plane Generation
//-----------------------------------------------------------------------------------------------
void FindTwoOrthogonalVectorsToNormal( FloatVector3& out_orthoVector1, FloatVector3& out_orthoVector2, const FloatVector3& normalVector )
{
	RECOVERABLE_ASSERTION( AreAlmostEqual( normalVector.CalculateNorm(), 1.f ), "Vector Finding Error", "Normal vector not length 1. Finding orthogonal vectors could be numerically unstable!" );

	//Find the smallest component of the normal vector and use that axis as vector 1 (this makes the calculation more stable)
	float smallestNormalComponentValue = std::abs( normalVector.x );
	FloatVector3 closestAxisToNormal( 1.f, 0.f, 0.f );
	if( std::abs( normalVector.y ) < smallestNormalComponentValue )
	{
		smallestNormalComponentValue = std::abs( normalVector.y );
		closestAxisToNormal = FloatVector3( 0.f, 1.f, 0.f );
	}
	if( std::abs( normalVector.z < smallestNormalComponentValue ) )
	{
		smallestNormalComponentValue = std::abs( normalVector.z );
		closestAxisToNormal = FloatVector3( 0.f, 0.f, 1.f );
	}
	out_orthoVector1 = closestAxisToNormal;

	//The other vector in the plane is the cross product of the normal with that axis.
	out_orthoVector2 = CrossProduct( out_orthoVector1, normalVector );
}

//-----------------------------------------------------------------------------------------------
void GenerateSimplePlane( VertexData& out_vertData, const FloatVector3& centerPos, const FloatVector3& normalVector, 
						  float width, float height, 
						  const Color color )
{
	static const unsigned int NUM_PLANE_VERTICES = 4;
	Simple3DVertex* boxVertexArray = new Simple3DVertex[ NUM_PLANE_VERTICES ];

	FloatVector3 inPlaneVector1;
	FloatVector3 inPlaneVector2;
	FindTwoOrthogonalVectorsToNormal( inPlaneVector1, inPlaneVector2, normalVector );

	boxVertexArray[ 0] = Simple3DVertex( centerPos + ( -width * inPlaneVector1 ) + ( -height * inPlaneVector2 ), color );
	boxVertexArray[ 1] = Simple3DVertex( centerPos + ( -width * inPlaneVector1 ) + (  height * inPlaneVector2 ), color );
	boxVertexArray[ 2] = Simple3DVertex( centerPos + (  width * inPlaneVector1 ) + ( -height * inPlaneVector2 ), color );
	boxVertexArray[ 3] = Simple3DVertex( centerPos + (  width * inPlaneVector1 ) + (  height * inPlaneVector2 ), color );

	out_vertData.data = &boxVertexArray[0];
	out_vertData.vertexSizeBytes = sizeof( Simple3DVertex );
	out_vertData.numberOfVertices = NUM_PLANE_VERTICES;
	out_vertData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Vertex, 3, RendererInterface::TYPE_FLOAT, false, sizeof( Simple3DVertex ), offsetof( Simple3DVertex, position.x ) ) );
	out_vertData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Color, 4, RendererInterface::TYPE_UNSIGNED_BYTE, true, sizeof( Simple3DVertex ), offsetof( Simple3DVertex, color.r ) ) );
	out_vertData.shape = RendererInterface::TRIANGLE_STRIP;
}

//-----------------------------------------------------------------------------------------------
void GenerateTexturedPlane( VertexData& out_vertData, const FloatVector3& centerPos, const FloatVector3& normalVector,
							float width, float height, const FloatVector2& lowerLeftUV, const FloatVector2& upperRightUV, 
							const Color color )
{
	static const unsigned int NUM_PLANE_VERTICES = 4;
	Textured3DVertex* boxVertexArray = new Textured3DVertex[ NUM_PLANE_VERTICES ];

	float halfWidth = 0.5f * width;
	float halfHeight = 0.5f * height;
	FloatVector3 inPlaneVector1;
	FloatVector3 inPlaneVector2;
	FindTwoOrthogonalVectorsToNormal( inPlaneVector1, inPlaneVector2, normalVector );

	boxVertexArray[ 0] = Textured3DVertex( centerPos + ( -halfWidth * inPlaneVector1 ) + ( -halfHeight * inPlaneVector2 ), color, upperRightUV.x, lowerLeftUV.y );
	boxVertexArray[ 1] = Textured3DVertex( centerPos + ( -halfWidth * inPlaneVector1 ) + (  halfHeight * inPlaneVector2 ), color, upperRightUV );
	boxVertexArray[ 2] = Textured3DVertex( centerPos + (  halfWidth * inPlaneVector1 ) + ( -halfHeight * inPlaneVector2 ), color, lowerLeftUV );
	boxVertexArray[ 3] = Textured3DVertex( centerPos + (  halfWidth * inPlaneVector1 ) + (  halfHeight * inPlaneVector2 ), color, lowerLeftUV.x, upperRightUV.y );

	out_vertData.data = &boxVertexArray[0];
	out_vertData.vertexSizeBytes = sizeof( Textured3DVertex );
	out_vertData.numberOfVertices = NUM_PLANE_VERTICES;
	out_vertData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Vertex, 3, RendererInterface::TYPE_FLOAT, false, sizeof( Textured3DVertex ), offsetof( Textured3DVertex, position.x ) ) );
	out_vertData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Color, 4, RendererInterface::TYPE_UNSIGNED_BYTE, true, sizeof( Textured3DVertex ), offsetof( Textured3DVertex, color.r ) ) );
	out_vertData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_TextureCoords, 2, RendererInterface::TYPE_FLOAT, false, sizeof( Textured3DVertex ), offsetof( Textured3DVertex, textureUV.x ) ) );
	out_vertData.shape = RendererInterface::TRIANGLE_STRIP;
}
#pragma endregion //Plane Generation



#pragma region Box Generation
//-----------------------------------------------------------------------------------------------
void GenerateSimpleBox( VertexData& out_vertData, const FloatVector3& centerPos, 
						float width, float length, float depth, const Color color )
{
	static const unsigned int NUMBER_OF_BOX_VERTICES = 14;
	Simple3DVertex* boxVertexArray = new Simple3DVertex[ NUMBER_OF_BOX_VERTICES ];

	float halfWidth = 0.5f * width;
	float halfLength = 0.5f * length;
	float halfDepth = 0.5f * depth;
	FloatVector3 boxMinPoint = FloatVector3( centerPos.x - halfWidth, centerPos.y - halfLength, centerPos.z - halfDepth );
	FloatVector3 boxMaxPoint = FloatVector3( centerPos.x + halfWidth, centerPos.y + halfLength, centerPos.z + halfDepth );

	boxVertexArray[ 0] = Simple3DVertex( boxMinPoint,									color );
	boxVertexArray[ 1] = Simple3DVertex( boxMaxPoint.x, boxMinPoint.y, boxMinPoint.z,	color );
	boxVertexArray[ 2] = Simple3DVertex( boxMinPoint.x, boxMaxPoint.y, boxMinPoint.z,	color );
	boxVertexArray[ 3] = Simple3DVertex( boxMaxPoint.x, boxMaxPoint.y, boxMinPoint.z,	color );
	boxVertexArray[ 4] = Simple3DVertex( boxMaxPoint,									color );
	boxVertexArray[ 5] = Simple3DVertex( boxMaxPoint.x, boxMinPoint.y, boxMinPoint.z,	color );
	boxVertexArray[ 6] = Simple3DVertex( boxMaxPoint.x, boxMinPoint.y, boxMaxPoint.z,	color );
	boxVertexArray[ 7] = Simple3DVertex( boxMinPoint,									color );
	boxVertexArray[ 8] = Simple3DVertex( boxMinPoint.x, boxMinPoint.y, boxMaxPoint.z,	color );
	boxVertexArray[ 9] = Simple3DVertex( boxMinPoint.x, boxMaxPoint.y, boxMinPoint.z,	color );
	boxVertexArray[10] = Simple3DVertex( boxMinPoint.x, boxMaxPoint.y, boxMaxPoint.z,	color );
	boxVertexArray[11] = Simple3DVertex( boxMaxPoint,									color );
	boxVertexArray[12] = Simple3DVertex( boxMinPoint.x, boxMinPoint.y, boxMaxPoint.z,	color );
	boxVertexArray[13] = Simple3DVertex( boxMaxPoint.x, boxMinPoint.y, boxMaxPoint.z,	color );

	out_vertData.data = &boxVertexArray[0];
	out_vertData.vertexSizeBytes = sizeof( Simple3DVertex );
	out_vertData.numberOfVertices = NUMBER_OF_BOX_VERTICES;
	out_vertData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Vertex, 3, RendererInterface::TYPE_FLOAT, false, sizeof( Simple3DVertex ), offsetof( Simple3DVertex, position.x ) ) );
	out_vertData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Color, 4, RendererInterface::TYPE_UNSIGNED_BYTE, true, sizeof( Simple3DVertex ), offsetof( Simple3DVertex, color.r ) ) );
	out_vertData.shape = RendererInterface::TRIANGLE_STRIP;
}

//-----------------------------------------------------------------------------------------------
void GenerateTexturedBox( VertexData& out_vertData, /*IndexData& out_indexData,*/
		const FloatVector3& centerPos, float width, float length, float depth, const Color color )
{
	static const unsigned int NUMBER_OF_BOX_VERTICES = 14;
	Simple3DVertex* boxVertexArray = new Simple3DVertex[ NUMBER_OF_BOX_VERTICES ];

	float halfWidth = 0.5f * width;
	float halfLength = 0.5f * length;
	float halfDepth = 0.5f * depth;
	FloatVector3 boxMinPoint = FloatVector3( centerPos.x - halfWidth, centerPos.y - halfLength, centerPos.z - halfDepth );
	FloatVector3 boxMaxPoint = FloatVector3( centerPos.x + halfWidth, centerPos.y + halfLength, centerPos.z + halfDepth );

	boxVertexArray[ 0] = Simple3DVertex( boxMinPoint,									color );
	boxVertexArray[ 1] = Simple3DVertex( boxMaxPoint.x, boxMinPoint.y, boxMinPoint.z,	color );
	boxVertexArray[ 2] = Simple3DVertex( boxMinPoint.x, boxMaxPoint.y, boxMinPoint.z,	color );
	boxVertexArray[ 3] = Simple3DVertex( boxMaxPoint.x, boxMaxPoint.y, boxMinPoint.z,	color );
	boxVertexArray[ 4] = Simple3DVertex( boxMaxPoint,									color );
	boxVertexArray[ 5] = Simple3DVertex( boxMaxPoint.x, boxMinPoint.y, boxMinPoint.z,	color );
	boxVertexArray[ 6] = Simple3DVertex( boxMaxPoint.x, boxMinPoint.y, boxMaxPoint.z,	color );
	boxVertexArray[ 7] = Simple3DVertex( boxMinPoint,									color );
	boxVertexArray[ 8] = Simple3DVertex( boxMinPoint.x, boxMinPoint.y, boxMaxPoint.z,	color );
	boxVertexArray[ 9] = Simple3DVertex( boxMinPoint.x, boxMaxPoint.y, boxMinPoint.z,	color );
	boxVertexArray[10] = Simple3DVertex( boxMinPoint.x, boxMaxPoint.y, boxMaxPoint.z,	color );
	boxVertexArray[11] = Simple3DVertex( boxMaxPoint,									color );
	boxVertexArray[12] = Simple3DVertex( boxMinPoint.x, boxMinPoint.y, boxMaxPoint.z,	color );
	boxVertexArray[13] = Simple3DVertex( boxMaxPoint.x, boxMinPoint.y, boxMaxPoint.z,	color );

	out_vertData.data = &boxVertexArray[0];
	out_vertData.vertexSizeBytes = sizeof( Simple3DVertex );
	out_vertData.numberOfVertices = NUMBER_OF_BOX_VERTICES;
	out_vertData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Vertex, 3, RendererInterface::TYPE_FLOAT, false, sizeof( Simple3DVertex ), offsetof( Simple3DVertex, position.x ) ) );
	out_vertData.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Color, 4, RendererInterface::TYPE_UNSIGNED_BYTE, true, sizeof( Simple3DVertex ), offsetof( Simple3DVertex, color.r ) ) );
	out_vertData.shape = RendererInterface::TRIANGLE_STRIP;
}
#pragma endregion //Box Generation
