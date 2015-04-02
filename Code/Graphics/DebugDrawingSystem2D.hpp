#pragma once
#ifndef INCLUDED_DEBUG_DRAWING_SYSTEM_2D_HPP
#define INCLUDED_DEBUG_DRAWING_SYSTEM_2D_HPP

//-----------------------------------------------------------------------------------------------
#include <string>
#include <vector>

#include "../Math/FloatVector2.hpp"
#include "../Color.hpp"
#include "../System.hpp"

struct BitmapFont;
struct Entity;
struct Material;
struct MeshComponent;

//-----------------------------------------------------------------------------------------------
class DebugDrawingSystem2D : public System
{
	friend class SystemManager;

	struct TimedMesh
	{
		TimedMesh( float lifetimeSeconds, MeshComponent* mesh ) 
			: secondsLeftUntilDestroyed( lifetimeSeconds )
			, meshComponent( mesh ) 
		{ }

		float secondsLeftUntilDestroyed;
		MeshComponent* meshComponent;
	};

public:
	DebugDrawingSystem2D( float leftXEdge, float rightXEdge, float bottomYEdge, float topYEdge );
	virtual ~DebugDrawingSystem2D() { }

	void DrawPoint( const FloatVector2& centerPosition, float size, const Color& color );
	void DrawPointForTime( const FloatVector2& centerPosition, float size, const Color& color, float lifetimeSeconds );
	void DrawText( const std::string& textString, const FloatVector2& position, const Color& color, 
				   float fontHeight );
	void DrawTextForTime( const std::string& textString, const FloatVector2& position, const Color& color, 
						  float fontHeight, float lifetimeSeconds );

	//Lifecycle
	void OnAttachment( SystemManager* manager );
	void OnEndFrame();
	void OnRender() const;
	void OnUpdate( float deltaSeconds );
	void OnDestruction();

	void CleanupTimedMesh( TimedMesh* timedMesh );
	void RenderMeshComponent( const MeshComponent* mesh ) const;

	//Data Members
	//Clock m_clock;
	Entity* m_debugMeshOwningEntity;
	std::vector< TimedMesh* > m_meshes;
	BitmapFont* m_debugFont;
	Material* m_debugMeshMaterial;
	Material* m_debugTextMaterial;

	float m_leftXEdge;
	float m_rightXEdge;
	float m_bottomYEdge;
	float m_topYEdge;
};



//-----------------------------------------------------------------------------------------------
inline DebugDrawingSystem2D::DebugDrawingSystem2D( float leftXEdge, float rightXEdge, 
												   float bottomYEdge, float topYEdge )
	: System()
	, m_debugMeshOwningEntity( nullptr )
	, m_debugFont( nullptr )
	, m_debugMeshMaterial( nullptr )
	, m_debugTextMaterial( nullptr )
	, m_leftXEdge( leftXEdge )
	, m_rightXEdge( rightXEdge )
	, m_bottomYEdge( bottomYEdge )
	, m_topYEdge( topYEdge )
{
	m_meshes.reserve( 25 );
}

//-----------------------------------------------------------------------------------------------
inline void DebugDrawingSystem2D::DrawPoint( const FloatVector2& centerPosition, float size, const Color& color )
{
	DrawPointForTime( centerPosition, size, color, 0.f );
}

//-----------------------------------------------------------------------------------------------
inline void DebugDrawingSystem2D::DrawText( const std::string& textString, const FloatVector2& position, const Color& color, float fontHeight )
{
	DrawTextForTime( textString, position, color, fontHeight, 0.f );
}

#endif //INCLUDED_DEBUG_DRAWING_SYSTEM_2D_HPP
