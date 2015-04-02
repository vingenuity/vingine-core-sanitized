#pragma once
#ifndef INCLUDED_ENTITY_BLUEPRINT_HPP
#define INCLUDED_ENTITY_BLUEPRINT_HPP

//-----------------------------------------------------------------------------------------------
#include "EngineMacros.hpp"
#include "Entity.hpp"


//-----------------------------------------------------------------------------------------------
ABSTRACT class EntityBlueprint
{
public:
	virtual ~EntityBlueprint() { }

	virtual void BuildEntity( Entity& out_entity ) = 0;
};
#endif //INCLUDED_ENTITY_BLUEPRINT_HPP
