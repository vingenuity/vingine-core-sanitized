#include "Mouse.hpp"

#include "../PlatformSpecificHeaders.hpp"

#pragma region Button Definitions
//-----------------------------------------------------------------------------------------------
#if defined( PLATFORM_WINDOWS )
	STATIC const Mouse::Button Mouse::BUTTON_EXTRA1 = XBUTTON1; // 0x1
	STATIC const Mouse::Button Mouse::BUTTON_EXTRA2 = XBUTTON2; // 0x2
	STATIC const Mouse::Button Mouse::BUTTON_LEFT = 3;
	STATIC const Mouse::Button Mouse::BUTTON_MIDDLE = 0;
	STATIC const Mouse::Button Mouse::BUTTON_RIGHT = 4;

//-----------------------------------------------------------------------------------------------
#elif defined( PLATFORM_HTML5 )
	STATIC const Mouse::Button Mouse::BUTTON_EXTRA1 = 3; //This is unused in HTML5 AFAIK.
	STATIC const Mouse::Button Mouse::BUTTON_EXTRA2 = 4; //This is unused in HTML5 AFAIK.
	STATIC const Mouse::Button Mouse::BUTTON_LEFT = 0;
	STATIC const Mouse::Button Mouse::BUTTON_MIDDLE = 1;
	STATIC const Mouse::Button Mouse::BUTTON_RIGHT = 2;

#endif
#pragma endregion //Button Definitions

//-----------------------------------------------------------------------------------------------
void Mouse::SetScreenPosition( int screenX, int screenY )
{
	m_positionDeltaLastFrame.x = m_screenPosition.x - screenX;
	m_positionDeltaLastFrame.y = m_screenPosition.y - screenY;

#if defined( PLATFORM_WINDOWS )
	if( m_isWindowLocked )
	{
		int screenHeight = GetSystemMetrics( SM_CYVIRTUALSCREEN );
		SetCursorPos( m_screenPosition.x, screenHeight - m_screenPosition.y );
	}
	else
	{
		m_screenPosition.x = screenX;
		m_screenPosition.y = screenY;
	}
#else
	m_screenPosition.x = screenX;
	m_screenPosition.y = screenY;
#endif
}

//-----------------------------------------------------------------------------------------------
void Mouse::Update()
{
	for( unsigned int i = 0; i < NUMBER_OF_MOUSE_BUTTONS; ++i )
	{
		m_wasButtonDown[ i ] = m_isButtonDown[ i ];
	}

	m_positionDeltaLastFrame.x = 0;
	m_positionDeltaLastFrame.y = 0;

	m_wheelDelta.x = 0;
	m_wheelDelta.y = 0;
}

//-----------------------------------------------------------------------------------------------
void Mouse::LockToWindow()
{
#if defined( PLATFORM_HTML5 )
	emscripten_request_pointerlock( 0, true );
#elif defined( PLATFORM_WINDOWS )
	ShowCursor( false );
#endif
	m_isWindowLocked = true;
}

//-----------------------------------------------------------------------------------------------
void Mouse::UnlockFromWindow()
{
#if defined( PLATFORM_HTML5 )
	emscripten_exit_pointerlock();
#elif defined( PLATFORM_WINDOWS )
	ShowCursor( true );
#endif
	m_isWindowLocked = false;
}
