#pragma once
#ifndef INCLUDED_CONTROLLER_HPP
#define INCLUDED_CONTROLLER_HPP

//-----------------------------------------------------------------------------------------------
#include "../EngineMacros.hpp"

struct Entity;
struct InputComponent;

//-----------------------------------------------------------------------------------------------
ABSTRACT class Controller
{
public:
	virtual ~Controller() { }
	virtual void PossessEntity( Entity* entity ) = 0;
	virtual void UpdatePossessedEntity( float deltaSeconds ) = 0;

private:
	Entity* possessedEntity;
	InputComponent* possessedInput;
};

#endif //INCLUDED_CONTROLLER_HPP
