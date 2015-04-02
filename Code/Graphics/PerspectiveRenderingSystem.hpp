#pragma once
#ifndef INCLUDED_PERSPECTIVE_RENDERING_SYSTEM_HPP
#define INCLUDED_PERSPECTIVE_RENDERING_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include "RenderingSystem.hpp"

struct MeshComponent;


//-----------------------------------------------------------------------------------------------
class PerspectiveRenderingSystem : public RenderingSystem
{
	friend class SystemManager;

public:
	PerspectiveRenderingSystem( double horizFOVDegrees, double aspectRatio,
								double nearClipPlane, double farClipPlane );


protected: //For use only by SystemManager
	void OnAttachment( SystemManager* manager );
	void OnEndFrame();
	void OnRender() const;
	void OnDestruction();


private:
	void RenderMeshComponent( const MeshComponent* mesh ) const;
	void ViewWorldThroughCamera( const CameraComponent* camera ) const;

	//Perspective Settings
	double m_horizontalFOVDegrees;
	double m_aspectRatio;
	double m_nearClippingPlane;
	double m_farClippingPlane;
};



//-----------------------------------------------------------------------------------------------
inline PerspectiveRenderingSystem::PerspectiveRenderingSystem( double horizFOVDegrees, double aspectRatio,
															   double nearClipPlane, double farClipPlane )
	: RenderingSystem()
	, m_horizontalFOVDegrees( horizFOVDegrees )
	, m_aspectRatio( aspectRatio )
	, m_nearClippingPlane( nearClipPlane )
	, m_farClippingPlane( farClipPlane )
{ }
#endif //INCLUDED_PERSPECTIVE_RENDERING_SYSTEM_HPP
