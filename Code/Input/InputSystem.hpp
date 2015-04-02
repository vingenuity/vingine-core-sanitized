#pragma once
#ifndef INCLUDED_INPUT_SYSTEM_HPP
#define INCLUDED_INPUT_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>
#include "../System.hpp"

class Controller;
struct InputComponent;

//-----------------------------------------------------------------------------------------------
class InputSystem : public System
{
public:
	InputSystem()
		: System()
	{ }

	virtual ~InputSystem();

	void AddComponent( InputComponent* inputComponent ) { m_inputs.push_back( inputComponent ); }
	void AddController( Controller* controller ) { m_controllers.push_back( controller ); }

	static void AddNewComponent( InputComponent* inputComponent ) { s_activeInputSystem->AddComponent( inputComponent ); }
	static void AddNewController( Controller* controller ) { s_activeInputSystem->AddController( controller ); }

	static void Create() { s_activeInputSystem = new InputSystem(); }
	static void Destroy() { delete s_activeInputSystem; s_activeInputSystem = nullptr; }


protected: //For use only by SystemManager
	virtual void OnAttachment( SystemManager* manager );
	virtual void OnEndFrame() { }
	virtual void OnRender() const { }
	virtual void OnUpdate(  float deltaSeconds );
	virtual void OnDestruction();


	//Data Members
	static InputSystem* s_activeInputSystem;

	std::vector< Controller* > m_controllers;
	std::vector< InputComponent* > m_inputs;
};

#endif //INCLUDED_INPUT_SYSTEM_HPP