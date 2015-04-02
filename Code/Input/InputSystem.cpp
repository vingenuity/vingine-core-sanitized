#include "InputSystem.hpp"

#include "Controller.hpp"
#include "InputComponent.hpp"
#include "../Entity.hpp"

//-----------------------------------------------------------------------------------------------
STATIC InputSystem* InputSystem::s_activeInputSystem = nullptr;

//-----------------------------------------------------------------------------------------------
VIRTUAL InputSystem::~InputSystem()
{
	for( unsigned int i = 0; i < m_controllers.size(); ++i )
	{
		delete m_controllers[ i ];
	}
	m_controllers.clear();

	for( unsigned int i = 0; i < m_inputs.size(); ++i )
	{
		delete m_inputs[ i ];
	}
	m_inputs.clear();
}

//-----------------------------------------------------------------------------------------------
VIRTUAL void InputSystem::OnAttachment( SystemManager* )
{

}

//-----------------------------------------------------------------------------------------------
void InputSystem::OnUpdate( float )
{
	for( unsigned int i = 0; i < m_controllers.size(); ++i )
	{
		m_controllers[ i ]->UpdatePossessedEntity( 0.0166f );
	}

	for( unsigned int i = 0; i < m_inputs.size(); ++i )
	{
		//fill the entity with our set of inputs
		//m_inputs[ i ]->owner->eventParameters->SetParameter( "actionsQueued", m_inputs[ i ]->m_queuedActions );
		//m_inputs[ i ]->owner->FireEntityEvent();
	}
}

//-----------------------------------------------------------------------------------------------
VIRTUAL void InputSystem::OnDestruction()
{

}
