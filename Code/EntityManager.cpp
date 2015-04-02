#include "EntityManager.hpp"

#include "AssertionError.hpp"
#include "Component.hpp"
#include "StringConversion.hpp"
#include "EngineMacros.hpp"


//-----------------------------------------------------------------------------------------------
STATIC unsigned int EntityManager::s_nextEntityID = 1;



//-----------------------------------------------------------------------------------------------
void EntityManager::DoAtEndOfFrame()
{
	for( unsigned int i = 0; i < m_entitiesWaitingForFiring.size(); ++i )
	{
		FireEntity( m_entitiesWaitingForFiring[ i ] );
	}
	m_entitiesWaitingForFiring.clear();
}



#pragma region Entity Management
//-----------------------------------------------------------------------------------------------
void EntityManager::FireEntity( Entity* entity )
{
	entity->position.x = entity->position.y = entity->position.z = 0.f;
	entity->velocity.x = entity->velocity.y = entity->velocity.z = 0.f;
	entity->acceleration.x = entity->acceleration.y = entity->acceleration.z = 0.f;
	entity->orientation.rollDegreesAboutX	= entity->angularVelocity.rollDegreesAboutX = 0.f;
	entity->orientation.pitchDegreesAboutY	= entity->angularVelocity.pitchDegreesAboutY = 0.f;
	entity->orientation.yawDegreesAboutZ	= entity->angularVelocity.yawDegreesAboutZ = 0.f;

	for( unsigned int i = 0; i < entity->attachedComponents.size(); ++i )
	{
		entity->attachedComponents[ i ]->readyForDeletion = true;
	}
	entity->attachedComponents.clear();

	entity->id = Entity::ID_Null;
	entity->typeID = Entity::TYPEID_None;
	entity->blueprint = nullptr;
}

//-----------------------------------------------------------------------------------------------
Entity* EntityManager::HireEntity()
{
	Entity* hiredEntity = nullptr;
	for( unsigned int i = 0; i < m_numEntitiesInPool; ++i )
	{
		Entity& entity = m_entityPool[ i ];

		if( !entity.IsHired() )
		{
			hiredEntity = &entity;
			break;
		}
	}

	if( hiredEntity == nullptr )
		ShowEmptyPoolErrorMessage();

	hiredEntity->id = s_nextEntityID;
	++s_nextEntityID;

	return hiredEntity;
}
#pragma endregion //Entity Management



//-----------------------------------------------------------------------------------------------
/* This function has been separated so that includers of EntityManager don't need to know about
		assertions or string conversion. */
void EntityManager::ShowEmptyPoolErrorMessage()
{
	std::string errorMessage = "Ran out of entities to lend to the game!\n";
	errorMessage.append( "Current Entity Pool Size: " );
	errorMessage.append( ConvertIntegerToString( m_numEntitiesInPool ) );
	FATAL_ERROR( "Entity Manager Error", errorMessage.c_str() );
}
