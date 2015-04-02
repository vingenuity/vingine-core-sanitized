#ifndef INCLUDED_ENTITY_HPP
#define INCLUDED_ENTITY_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <algorithm>
#include <vector>

#include "Math/EulerAngles.hpp"
#include "Math/FloatVector3.hpp"

struct Component;
class EntityBlueprint;

//-----------------------------------------------------------------------------------------------
struct Entity
{
private:
	static const unsigned int ID_Null = 0;
	friend class EntityManager;


public:
	typedef unsigned int TypeID;
	static const TypeID TYPEID_None = 0;


	//Construction/Destruction
	Entity();
	virtual ~Entity();
	

	//Component Helpers
	template< typename ComponentType >
	void AttachComponent( ComponentType* componentToAttach );

	template< typename ComponentType >
	void DetachComponent( ComponentType* componentToDetach );

	template< typename ComponentType >
	ComponentType* FindAttachedComponentOfType();

	template< typename ComponentType >
	bool HasAttachedComponentOfType();


	//Entity Management Helpers
	bool IsHired() const { return ( id != ID_Null ); }


	//Data Members
	unsigned int id;
	TypeID typeID;
	Entity* creator;
	EntityBlueprint* blueprint;
	std::vector< Component* > attachedComponents;

	FloatVector3 position;
	FloatVector3 velocity;
	FloatVector3 acceleration;
	EulerAngles orientation;
	EulerAngles angularVelocity;

private:
	Entity* nextFiredEntity;
};



//-----------------------------------------------------------------------------------------------
inline Entity::Entity()
	: id( ID_Null )
	, typeID( TYPEID_None )
	, creator( nullptr )
	, blueprint( nullptr )
	, nextFiredEntity( nullptr )
{ }



#pragma region Component Helpers
//-----------------------------------------------------------------------------------------------
template< typename ComponentType >
void Entity::AttachComponent( ComponentType* componentToAttach )
{
	attachedComponents.push_back( componentToAttach );
	componentToAttach->owner = this;
}

//-----------------------------------------------------------------------------------------------
template< typename ComponentType >
void Entity::DetachComponent( ComponentType* componentToDetach )
{
	for( unsigned int i = 0; i < attachedComponents.size(); ++i )
	{
		Component*& component = attachedComponents[ i ];

		if( component == componentToDetach )
		{
			componentToDetach->owner = nullptr;
			attachedComponents.erase( attachedComponents.begin() + i );
		}
	}
}

//-----------------------------------------------------------------------------------------------
template< typename ComponentType >
ComponentType* Entity::FindAttachedComponentOfType()
{
	ComponentType* foundComponent;
	for( unsigned int i = 0; i < attachedComponents.size(); ++i )
	{
		foundComponent = dynamic_cast< ComponentType* >( attachedComponents[ i ] );

		if( foundComponent != nullptr )
			return foundComponent;
	}
	return nullptr;
}

//-----------------------------------------------------------------------------------------------
template< typename ComponentType >
bool Entity::HasAttachedComponentOfType()
{
	return ( FindAttachedComponentOfType< ComponentType >() != nullptr );
}
#pragma endregion //Component Helpers

#endif //INCLUDED_ENTITY_HPP
