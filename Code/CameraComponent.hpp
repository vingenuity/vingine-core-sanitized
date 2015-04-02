#pragma once
#ifndef INCLUDED_CAMERA_COMPONENT_HPP
#define INCLUDED_CAMERA_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include "Component.hpp"


//-----------------------------------------------------------------------------------------------
struct OrthographicProjection
{
	double leftEdgeX;
	double rightEdgeX;
	double bottomEdgeY;
	double topEdgeY;
	double nearClippingPlane;
	double farClippingPlane;
};



//-----------------------------------------------------------------------------------------------
struct PerspectiveProjection
{
	double horizontalFOVDegrees;
	double aspectRatio;
	double nearClippingPlane;
	double farClippingPlane;
};



//-----------------------------------------------------------------------------------------------
struct CameraComponent : public Component
{
	typedef unsigned char ProjectionType;
	static const ProjectionType PROJECTION_NONE = 0;
	static const ProjectionType PROJECTION_ORTHOGRAPHIC = 1;
	static const ProjectionType PROJECTION_PERSPECTIVE = 2;

	CameraComponent();

	//Helpers
	void SetOrthographicProjection(	double leftXEdge, double rightXEdge,
									double bottomYEdge, double topYEdge,
									double nearClipPlane, double farClipPlane );
	void SetPerspectiveProjection(	double horizFOVDegrees, double screenAspectRatio, 
									double nearClipPlane, double farClipPlane );
	
	//Data Members
	ProjectionType projectionType;
	union ProjectionData
	{
		OrthographicProjection orthographic;
		PerspectiveProjection perspective;
	} projection;
};



//-----------------------------------------------------------------------------------------------
inline CameraComponent::CameraComponent()
	: projectionType( PROJECTION_NONE )
{ }

//-----------------------------------------------------------------------------------------------
inline void CameraComponent::SetOrthographicProjection( double leftXEdge, double rightXEdge,
	double bottomYEdge, double topYEdge,
	double nearClipPlane, double farClipPlane )
{
	projectionType = PROJECTION_ORTHOGRAPHIC;

	projection.orthographic.leftEdgeX = leftXEdge;
	projection.orthographic.rightEdgeX = rightXEdge;
	projection.orthographic.bottomEdgeY = bottomYEdge;
	projection.orthographic.topEdgeY = topYEdge;
	projection.orthographic.nearClippingPlane = nearClipPlane;
	projection.orthographic.farClippingPlane = farClipPlane;
}

//-----------------------------------------------------------------------------------------------
inline void CameraComponent::SetPerspectiveProjection( double horizFOVDegrees, double screenAspectRatio, 
	double nearClipPlane, double farClipPlane )
{
	projectionType = PROJECTION_PERSPECTIVE;

	projection.perspective.horizontalFOVDegrees = horizFOVDegrees;
	projection.perspective.aspectRatio = screenAspectRatio;
	projection.perspective.nearClippingPlane = nearClipPlane;
	projection.perspective.farClippingPlane = farClipPlane;
}
#endif //INCLUDED_CAMERA_COMPONENT_HPP