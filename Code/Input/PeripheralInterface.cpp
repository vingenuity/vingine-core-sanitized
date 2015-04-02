#include "PeripheralInterface.hpp"

#include "../AssertionError.hpp"
#include "../EngineMacros.hpp"
#include "../Graphics/DebugDrawingSystem2D.hpp"

STATIC PeripheralInterface* PeripheralInterface::s_activePeripheralInterface = nullptr;

#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
STATIC void PeripheralInterface::Startup()
{
	FATAL_ASSERTION( s_activePeripheralInterface == nullptr, "Peripheral Interface Error", "Cannot start up multiple Peripheral Interfaces!" );

	s_activePeripheralInterface = new PeripheralInterface();

	s_activePeripheralInterface->m_keyboard = new Keyboard();
	s_activePeripheralInterface->m_mouse = new Mouse();
	s_activePeripheralInterface->m_touchscreens.push_back( new TouchScreen() );
	s_activePeripheralInterface->m_touchscreens.push_back( new TouchScreen() );
}

//-----------------------------------------------------------------------------------------------
STATIC void PeripheralInterface::Update( float deltaSeconds )
{
	VARIABLE_IS_UNUSED( deltaSeconds );
	s_activePeripheralInterface->m_keyboard->Update();
	s_activePeripheralInterface->m_mouse->Update();
	
	for( unsigned int i = 0; i < s_activePeripheralInterface->m_touchscreens.size(); ++i )
	{
		s_activePeripheralInterface->m_touchscreens[ i ]->Update( deltaSeconds );
	}
	
	for( unsigned char i = 0; i < s_activePeripheralInterface->m_gamepads.size(); ++i )
	{
		Gamepad* const& gamepad = s_activePeripheralInterface->m_gamepads[ i ];
		if( gamepad == nullptr )
			continue;

		memcpy( gamepad->lastAxisState,		gamepad->currAxisState,		Gamepad::MAX_NUMBER_OF_AXES * sizeof( Gamepad::Axis ) );
		memcpy( gamepad->lastButtonState,	gamepad->currButtonState,	Gamepad::MAX_NUMBER_OF_BUTTONS * sizeof( Gamepad::Button ) );
	}

#ifdef DRAW_MOUSE_DATA
	IntVector2 scnPos = PeripheralInterface::GetMouse()->GetScreenPosition();
	IntVector2 winPos = PeripheralInterface::GetMouse()->GetWindowPosition();

// 	DebugDrawingSystem2D* debugUISystem = SystemManager::GetDebugUIRenderingSystem();
// 	debugUISystem->DrawPoint( FloatVector2( (float)scnPos.x, (float)scnPos.y ), 20.f, Color( 1.f, 0.f, 0.f, 1.f ) );
// 	debugUISystem->DrawPoint( FloatVector2( (float)winPos.x, (float)winPos.y ), 20.f, Color( 0.f, 1.f, 0.f, 1.f ) );
// 
// 	if( s_activePeripheralInterface->m_mouse->IsButtonPressedOrHeld( Mouse::BUTTON_LEFT ) )
// 		debugUISystem->DrawText( "L", FloatVector2( winPos.x - 25.f, winPos.y + 15.f ), Color( 0.f, 1.f, 0.f, 1.f ), 25.f );
// 	if( s_activePeripheralInterface->m_mouse->IsButtonPressedOrHeld( Mouse::BUTTON_MIDDLE ) )
// 		debugUISystem->DrawText( "M", FloatVector2( winPos.x - 5.f, winPos.y + 20.f ), Color( 0.f, 1.f, 0.f, 1.f ), 25.f );
// 	if( s_activePeripheralInterface->m_mouse->IsButtonPressedOrHeld( Mouse::BUTTON_RIGHT ) )
// 		debugUISystem->DrawText( "R", FloatVector2( winPos.x + 20.f, winPos.y + 15.f ), Color( 0.f, 1.f, 0.f, 1.f ), 25.f );
// 	if( s_activePeripheralInterface->m_mouse->IsButtonPressedOrHeld( Mouse::BUTTON_EXTRA1 ) )
// 		debugUISystem->DrawText( "1", FloatVector2( winPos.x - 25.f, winPos.y - 15.f ), Color( 0.f, 1.f, 0.f, 1.f ), 25.f );
// 	if( s_activePeripheralInterface->m_mouse->IsButtonPressedOrHeld( Mouse::BUTTON_EXTRA2 ) )
// 		debugUISystem->DrawText( "2", FloatVector2( winPos.x + 20.f, winPos.y - 15.f ), Color( 0.f, 1.f, 0.f, 1.f ), 25.f );
#endif
}

//-----------------------------------------------------------------------------------------------
STATIC void PeripheralInterface::Shutdown()
{
	RECOVERABLE_ASSERTION( s_activePeripheralInterface != nullptr, "Peripheral Interface Error", "Cannot shut down a Peripheral Interface that hasn't yet been started!" );
	delete s_activePeripheralInterface;
}
#pragma endregion //Lifecycle