#pragma once
#ifndef INCLUDED_RENDERING_SYSTEM_HPP
#define INCLUDED_RENDERING_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include "../System.hpp"

struct CameraComponent;
struct MeshComponent;

//-----------------------------------------------------------------------------------------------
ABSTRACT class RenderingSystem : public System
{
public:
	RenderingSystem() : System(), m_activeCamera( nullptr ) { }

	void AddMeshComponent( MeshComponent* mesh ) { m_meshes.push_back( mesh ); }
	void SetActiveCamera( CameraComponent* camera ) { m_activeCamera = camera; }

	//Lifecycle
	virtual void OnAttachment( SystemManager* manager ) = 0;
	virtual void OnEndFrame() { }
	virtual void OnRender() const = 0;
	virtual void OnUpdate(  float /*deltaSeconds*/ ) { }
	virtual void OnDestruction() = 0;

	//Stored Components
	std::vector< MeshComponent* > m_meshes;
	CameraComponent* m_activeCamera;
};

#endif //INCLUDED_RENDERING_SYSTEM_HPP
