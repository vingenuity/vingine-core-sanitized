#pragma once
#ifndef INCLUDED_TERRESTRIAL_PHYSICS_SYSTEM_HPP
#define INCLUDED_TERRESTRIAL_PHYSICS_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include "Math/FloatVector3.hpp"
#include "System.hpp"

struct PhysicsComponent;


//-----------------------------------------------------------------------------------------------
class TerrestrialPhysicsSystem : public System
{
public:
	TerrestrialPhysicsSystem( const FloatVector3& gravityForceVector );

	void AddPhysicsComponent( PhysicsComponent* physicsComponent ) { m_physComponents.push_back( physicsComponent ); }

	//Lifecycle
	void OnAttachment( SystemManager* manager );
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float deltaSeconds );
	void OnDestruction();


private:
	void UpdatePhysicsComponent( PhysicsComponent* physicsComponent, float deltaSeconds );

	//Data Members
	FloatVector3 m_gravityAccelerationVector;
	std::vector< PhysicsComponent* > m_physComponents;

};



//-----------------------------------------------------------------------------------------------
inline TerrestrialPhysicsSystem::TerrestrialPhysicsSystem( const FloatVector3& gravityForceVector )
	: m_gravityAccelerationVector( gravityForceVector )
{ }

#endif //INCLUDED_TERRESTRIAL_PHYSICS_SYSTEM_HPP
