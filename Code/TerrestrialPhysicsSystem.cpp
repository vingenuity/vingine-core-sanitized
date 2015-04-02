#include "TerrestrialPhysicsSystem.hpp"

#include "PhysicsComponent.hpp"



#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void TerrestrialPhysicsSystem::OnAttachment( SystemManager* /*manager*/ )
{

}

//-----------------------------------------------------------------------------------------------
void TerrestrialPhysicsSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_physComponents.size(); ++i )
	{
		if( m_physComponents[ i ]->IsReadyForDeletion() )
		{
			delete m_physComponents[ i ];
			m_physComponents.erase( m_physComponents.begin() + i );
		}
	}
}

//-----------------------------------------------------------------------------------------------
void TerrestrialPhysicsSystem::OnUpdate( float deltaSeconds )
{
	for( unsigned int i = 0; i < m_physComponents.size(); ++i )
	{
		UpdatePhysicsComponent( m_physComponents[ i ], deltaSeconds );
	}
}

//-----------------------------------------------------------------------------------------------
void TerrestrialPhysicsSystem::OnDestruction()
{
	for( unsigned int i = 0; i < m_physComponents.size(); ++i )
	{
		delete m_physComponents[ i ];
	}
	m_physComponents.clear();
}
#pragma endregion //Lifecycle



//-----------------------------------------------------------------------------------------------
void TerrestrialPhysicsSystem::UpdatePhysicsComponent( PhysicsComponent* physicsComponent, float deltaSeconds )
{
	Entity*& physicsOwner = physicsComponent->owner;
	physicsOwner->velocity += ( m_gravityAccelerationVector * physicsComponent->percentAcceleratedByGravity ) * deltaSeconds;
	physicsOwner->position += physicsOwner->velocity * deltaSeconds;

	physicsOwner->velocity *= 0.9f;
	if( physicsOwner->position.z < 2.f )
	{
		physicsOwner->velocity.z = 0.f;
		physicsOwner->position.z = 2.f;
	}
}
