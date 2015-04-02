#pragma once
#ifndef INCLUDED_SYSTEM_HPP
#define INCLUDED_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include "EngineMacros.hpp"
//#include "../Time/Clock.hpp"

class SystemManager;

//-----------------------------------------------------------------------------------------------
ABSTRACT class System
{
	friend class SystemManager;

public:
	System() { }
	virtual ~System() {}

protected: //For use only by SystemManager
	virtual void OnAttachment( SystemManager* manager ) = 0;
	virtual void OnEndFrame() = 0;
	virtual void OnRender() const = 0;
	virtual void OnUpdate( float deltaSeconds ) = 0;
	virtual void OnDestruction() = 0;

	//Data Members
	//Clock m_clock;
};
#endif //INCLUDED_SYSTEM_HPP
