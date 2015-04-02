#pragma once
#ifndef INCLUDED_PHYSICS_COMPONENT_HPP
#define INCLUDED_PHYSICS_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include "Component.hpp"


//-----------------------------------------------------------------------------------------------
struct PhysicsComponent : public Component
{
	PhysicsComponent( float pctAcceleratedByGravity = 0.f );

	float percentAcceleratedByGravity;
};



//-----------------------------------------------------------------------------------------------
inline PhysicsComponent::PhysicsComponent( float pctAcceleratedByGravity )
	: percentAcceleratedByGravity( pctAcceleratedByGravity )
{ }

#endif //INCLUDED_PHYSICS_COMPONENT_HPP
