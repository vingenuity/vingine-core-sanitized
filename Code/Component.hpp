#ifndef INCLUDED_COMPONENT_HPP
#define INCLUDED_COMPONENT_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "EngineMacros.hpp"
#include "Entity.hpp"


//-----------------------------------------------------------------------------------------------
ABSTRACT struct Component
{
	template<typename ComponentType>
	friend class ComponentSystem;


protected:
	Component();


public:
	virtual ~Component() { }

	bool IsActive() const { return active; }
	bool IsReadyForDeletion() const { return readyForDeletion; }


	//Data Members
private:
	bool active;

public:
	bool readyForDeletion;
	Entity* owner;
};



//-----------------------------------------------------------------------------------------------
inline Component::Component()
	: active( false )
	, readyForDeletion( false )
	, owner( nullptr )
{ }

#endif //INCLUDED_COMPONENT_HPP
