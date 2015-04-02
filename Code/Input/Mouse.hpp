#pragma once
#ifndef INCLUDED_MOUSE_HPP
#define INCLUDED_MOUSE_HPP

//-----------------------------------------------------------------------------------------------
#include <cstring>
#include <string.h>

#include "../Math/FloatVector2.hpp"
#include "../Math/IntVector2.hpp"


//-----------------------------------------------------------------------------------------------
class Mouse
{
public:
	//Button Declarations
	static const unsigned char NUMBER_OF_MOUSE_BUTTONS = 5;
	typedef unsigned char Button;
	static const Button BUTTON_EXTRA1;
	static const Button BUTTON_EXTRA2;
	static const Button BUTTON_LEFT;
	static const Button BUTTON_MIDDLE;
	static const Button BUTTON_RIGHT;


public:
	//Lifecycle
	Mouse();
	~Mouse() { }

	void Update();
	
	//Getters
	const IntVector2& GetPositionDelta() const { return m_positionDeltaLastFrame; }
	const IntVector2& GetScreenPosition() const { return m_screenPosition; }
	const FloatVector2& GetWheelDelta() const { return m_wheelDelta; }
	const IntVector2& GetWindowPosition() const { return m_windowPosition; }
	bool IsButtonPressed( Button button ) const { return m_isButtonDown[ button ] && ( !m_wasButtonDown[ button ] ); }
	bool IsButtonPressedOrHeld( Button button ) const { return m_isButtonDown[ button ]; }
	bool IsLockedToWindow() const { return m_isWindowLocked; }

	//Setters
	void LockToWindow();
	void SetButtonState( Button button, bool isPressed ) { m_isButtonDown[ button ] = isPressed; }
	void SetPositionDelta( int xDelta, int yDelta );
	void SetScreenPosition( int screenX, int screenY );
	void SetWheelDelta( float deltaX, float deltaY );
	void SetWheelDeltaX( float deltaX ) { m_wheelDelta.x = deltaX; }
	void SetWheelDeltaY( float deltaY ) { m_wheelDelta.y = deltaY; }
	void SetWindowPosition( int windowX, int windowY );
	void UnlockFromWindow();


private:
	bool m_isWindowLocked;

	bool m_isButtonDown[ NUMBER_OF_MOUSE_BUTTONS ];
	bool m_wasButtonDown[ NUMBER_OF_MOUSE_BUTTONS ];

	IntVector2 m_screenPosition;
	IntVector2 m_windowPosition;
	IntVector2 m_positionDeltaLastFrame;
	FloatVector2 m_wheelDelta;
};



//-----------------------------------------------------------------------------------------------
inline Mouse::Mouse()
	: m_isWindowLocked( false )
{
	memset( m_isButtonDown, 0, NUMBER_OF_MOUSE_BUTTONS );
	memset( m_wasButtonDown, 0, NUMBER_OF_MOUSE_BUTTONS );
}

//-----------------------------------------------------------------------------------------------
inline void Mouse::SetPositionDelta( int xDelta, int yDelta )
{
	m_positionDeltaLastFrame.x = xDelta;
	m_positionDeltaLastFrame.y = yDelta;
}

//-----------------------------------------------------------------------------------------------
inline void Mouse::SetWheelDelta( float deltaX, float deltaY )
{
	m_wheelDelta.x = deltaX;
	m_wheelDelta.y = deltaY;
}

//-----------------------------------------------------------------------------------------------
inline void Mouse::SetWindowPosition( int windowX, int windowY )
{
	m_windowPosition.x = windowX;
	m_windowPosition.y = windowY;
}

#endif //INCLUDED_MOUSE_HPP
