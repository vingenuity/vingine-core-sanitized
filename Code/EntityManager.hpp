#pragma once
#ifndef INCLUDED_ENTITY_MANAGER_HPP
#define INCLUDED_ENTITY_MANAGER_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include "Entity.hpp"


//-----------------------------------------------------------------------------------------------
typedef const Entity* ConstEntityIterator;


//-----------------------------------------------------------------------------------------------
class EntityManager
{
public:
	//Entity Management
	void FireEntity( Entity* entity );
	Entity* HireEntity();
	void QueueEntityForFiring( Entity* entity );

	//Iteration
	ConstEntityIterator GetPoolStart() { return &m_entityPool[0]; }
	ConstEntityIterator GetPoolEnd() { return &m_entityPool[ m_numEntitiesInPool ]; }


private:
	//Only the game interface is allowed to construct an entity manager
	friend class GameInterface;


	//Lifecycle
	EntityManager( size_t maxEntitiesInPool );
	~EntityManager();
	void DoAtEndOfFrame();

	//Having multiple talent managers would probably cause the entities to unionize. Bad idea.
	EntityManager( const EntityManager& other );
	EntityManager& operator=( const EntityManager& other );

	//Helpers
	void ShowEmptyPoolErrorMessage();

	//Data Members
	static unsigned int s_nextEntityID;

	size_t m_numEntitiesInPool;
	Entity* m_entityPool;
	Entity* m_joblessEntityLeader;
	std::vector< Entity* > m_entitiesWaitingForFiring;
};



#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
inline EntityManager::EntityManager( size_t maxEntitiesInPool )
	: m_numEntitiesInPool( maxEntitiesInPool )
	, m_entityPool( new Entity[m_numEntitiesInPool] )
	, m_joblessEntityLeader( nullptr )
{ }

//-----------------------------------------------------------------------------------------------
inline EntityManager::~EntityManager()
{
	delete[] m_entityPool;
}
#pragma endregion //Lifecycle



//-----------------------------------------------------------------------------------------------
inline void EntityManager::QueueEntityForFiring( Entity* entity )
{
	m_entitiesWaitingForFiring.push_back( entity );
}
#endif //INCLUDED_ENTITY_MANAGER_HPP
