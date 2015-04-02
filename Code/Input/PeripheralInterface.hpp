#pragma once
#ifndef INCLUDED_PERIPHERAL_INTERFACE_HPP
#define INCLUDED_PERIPHERAL_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include <map>

#include "../Math/EngineMath.hpp"
#include "../EngineMacros.hpp"

#include "Gamepad.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Touchscreen.hpp"


/* The peripheral interface class serves as a service locator for all local peripherals the player can use.
	By wrapping all inputs in this locator, we can avoid having a number of singletons while also
	keeping input separate from both the main file as well as the game class.*/
//-----------------------------------------------------------------------------------------------
ABSTRACT SINGLETON class PeripheralInterface
{
public:
	//Lifecycle
	static void Startup();
	static void Update( float deltaSeconds );
	static void Shutdown();


	//Locators
	static Gamepad* GetGamepadAtIndex( unsigned int index );
	static Gamepad* GetGamepad( Gamepad::ID gamepadID ) { return s_activePeripheralInterface->m_gamepads[ gamepadID ]; }
	static Keyboard* GetKeyboard() { return s_activePeripheralInterface->m_keyboard; }
	static Mouse* GetMouse() { return s_activePeripheralInterface->m_mouse; }
	static TouchScreen* GetTouchscreen( unsigned int touchscreenID = 0 );


	//Modifiers
	static Gamepad::ID ConnectNewGamepad( unsigned char numAxes, unsigned char numButtons, float axisDeadzones = 0.2f );
	static void DisconnectGamepad( Gamepad::ID id );


	//Helpers
	static float ConvertDigitalInputsIntoAngleDegrees( bool upPressed, bool rightPressed, bool downPressed, bool leftPressed );


private:
	// Should only be called through Startup() and Shutdown()
	PeripheralInterface();
	~PeripheralInterface();

	//We have no need of a pithy assignment or copy operator!
	PeripheralInterface( const PeripheralInterface& other );
	PeripheralInterface& operator=( const PeripheralInterface& other );

	//Data Members
	static PeripheralInterface* s_activePeripheralInterface;

	Keyboard* m_keyboard;
	Mouse* m_mouse;

	Gamepad::ID m_nextGamepadID;
	std::map< Gamepad::ID, Gamepad* > m_gamepads;

	std::vector< TouchScreen* > m_touchscreens;
};



//-----------------------------------------------------------------------------------------------
inline PeripheralInterface::PeripheralInterface()
	: m_keyboard( nullptr )
	, m_mouse( nullptr )
	, m_nextGamepadID( 0 )
{ }

//-----------------------------------------------------------------------------------------------
inline PeripheralInterface::~PeripheralInterface()
{
	if( m_keyboard != nullptr )
		delete m_keyboard;

	if( m_mouse != nullptr )
		delete m_mouse;

	for( unsigned int i = 0; i < m_touchscreens.size(); ++i )
	{
		delete m_touchscreens[ i ];
	}
	m_touchscreens.clear();
}

//-----------------------------------------------------------------------------------------------
STATIC inline TouchScreen* PeripheralInterface::GetTouchscreen( unsigned int touchscreenID )
{
	if( touchscreenID >= s_activePeripheralInterface->m_touchscreens.size() )
		return nullptr;

	return s_activePeripheralInterface->m_touchscreens[ touchscreenID ];
}

//-----------------------------------------------------------------------------------------------
STATIC inline Gamepad::ID PeripheralInterface::ConnectNewGamepad(	unsigned char numAxes,
																	unsigned char numButtons,
																	float axisDeadzones )
{
	Gamepad::ID newGamepadID = s_activePeripheralInterface->m_nextGamepadID;
	++s_activePeripheralInterface->m_nextGamepadID;

	s_activePeripheralInterface->m_gamepads[ newGamepadID ] = new Gamepad( newGamepadID, numAxes, numButtons, axisDeadzones );
	return newGamepadID;
}

//-----------------------------------------------------------------------------------------------
STATIC inline Gamepad* PeripheralInterface::GetGamepadAtIndex( unsigned int index )
{
	if( index >= s_activePeripheralInterface->m_gamepads.size() )
		return nullptr;
	else
	{
		std::map< Gamepad::ID, Gamepad* >::iterator gamepadMappingAtIndex = s_activePeripheralInterface->m_gamepads.begin();
		std::advance( gamepadMappingAtIndex, index );
		return gamepadMappingAtIndex->second;
	}
}

//-----------------------------------------------------------------------------------------------
STATIC inline void PeripheralInterface::DisconnectGamepad( Gamepad::ID id )
{
	//We don't care if it didn't exist in the first place; deleting nullptr is innocuous.
	Gamepad* disconnectedGamepad = s_activePeripheralInterface->m_gamepads[ id ];
	delete disconnectedGamepad;
	s_activePeripheralInterface->m_gamepads.erase( id );
}

//-----------------------------------------------------------------------------------------------
STATIC inline float PeripheralInterface::ConvertDigitalInputsIntoAngleDegrees( bool upPressed, bool rightPressed, bool downPressed, bool leftPressed )
{
	static int upDirectionValue = 2, rightDirectionValue = 2, downDirectionValue = 2, leftDirectionValue = 2;

	upDirectionValue = static_cast< int >( upPressed );
	downDirectionValue = -static_cast< int >( downPressed );
	leftDirectionValue = -static_cast< int >( leftPressed );
	rightDirectionValue = static_cast< int >( rightPressed );

	float desiredOrientationRadians = atan2f( static_cast<float>( upDirectionValue + downDirectionValue ), 
											  static_cast<float>( rightDirectionValue + leftDirectionValue ) );
	float desiredOrientationDegrees = ConvertRadiansToDegrees( desiredOrientationRadians );
	return desiredOrientationDegrees;
}

#endif //INCLUDED_PERIPHERAL_INTERFACE_HPP