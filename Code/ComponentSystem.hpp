#pragma once
#ifndef INCLUDED_COMPONENT_SYSTEM_HPP
#define INCLUDED_COMPONENT_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include "AssertionError.hpp"
#include "StringConversion.hpp"
#include "System.hpp"

//-----------------------------------------------------------------------------------------------
template< typename ComponentType >
class ComponentSystem : public System
{
public:
	//Component Acquisition
	ComponentType* AcquireComponent();
	void RelinquishComponent( ComponentType* component );

protected:
	ComponentSystem( size_t maxComponentsInPool );

	//Lifecycle
	virtual void OnUpdate( float /*deltaSeconds*/ ) { }
	virtual void OnDestruction();


	//Component Array
	size_t m_numComponentsInPool;
	ComponentType* m_componentPool;
	size_t m_lastAcquiredComponentIndex;
};



//-----------------------------------------------------------------------------------------------
template< typename ComponentType >
inline ComponentSystem<ComponentType>::ComponentSystem( size_t maxComponentsInPool )
	: System()
	, m_numComponentsInPool( maxComponentsInPool )
	, m_componentPool( new ComponentType[m_numComponentsInPool] )
	, m_lastAcquiredComponentIndex( 0 )
{ }



#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
template< typename ComponentType >
inline VIRTUAL void ComponentSystem<ComponentType>::OnDestruction()
{
	delete[] m_componentPool;
}
#pragma endregion //Lifecycle



#pragma region Component Acquisition
//-----------------------------------------------------------------------------------------------
template< typename ComponentType >
inline ComponentType* ComponentSystem<ComponentType>::AcquireComponent()
{
	ComponentType* currentComponent = nullptr;

	size_t i = m_lastAcquiredComponentIndex;
	do 
	{
		currentComponent = &m_componentPool[ i ];
		if( !currentComponent->active )
		{
			currentComponent->active = true;
			return currentComponent;
		}

		i = (i + 1)%m_numComponentsInPool;
	} while (i != m_lastAcquiredComponentIndex);

	std::string emptyPoolErrorMessage = "The component pool for ";
	emptyPoolErrorMessage.append( typeid( ComponentType ).name() );
	emptyPoolErrorMessage.append( " has run out of components to lend!\n" );
	emptyPoolErrorMessage.append( "Current pool size: " );
	emptyPoolErrorMessage.append( ConvertIntegerToString( m_numComponentsInPool ) );
	FATAL_ERROR( "Component System Error", emptyPoolErrorMessage.c_str()  );
}

//-----------------------------------------------------------------------------------------------
template< typename ComponentType >
inline void ComponentSystem<ComponentType>::RelinquishComponent( ComponentType* component )
{
	component->active = false;
	component->readyForDeletion = false;
	component->owner = nullptr;
}
#pragma endregion //Component Acquisition

#endif //INCLUDED_COMPONENT_SYSTEM_HPP
