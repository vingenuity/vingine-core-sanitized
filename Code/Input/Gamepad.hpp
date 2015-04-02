#pragma once
#ifndef INCLUDED_GAMEPAD_HPP
#define INCLUDED_GAMEPAD_HPP

#pragma warning( push )
#pragma warning( disable: 4351 ) //default initialization of arrays

//----------------------------------------------------------------------------------------------------
#include <cmath>
#include <cstring>
#include <math.h>
#include <string.h>

#include "../Bitmasking.hpp"


//----------------------------------------------------------------------------------------------------
struct Gamepad
{
	typedef unsigned char ID;
	static const ID ID_Null;

	typedef unsigned char PlayerID;
	static const PlayerID PLAYER_None;

	typedef float Axis;
	typedef unsigned char AxisID;
	static const size_t MAX_NUMBER_OF_BUTTONS = 16;

	typedef float Button;
	typedef unsigned char ButtonID;
	typedef unsigned int ButtonSet;
	static const float BUTTON_DEADZONE;
	static const size_t MAX_NUMBER_OF_AXES = 8;


	//Constructors
private:
	Gamepad();
public:
	Gamepad( ID padID, size_t numAxes, size_t numButtons,
			 float axisDeadzones = 0.2f, PlayerID playID = PLAYER_None );


	//Axes
	float	GetRawAxisValue( AxisID axisID ) const { return currAxisState[ axisID ]; }
	float	GetSmoothedAxisValue( AxisID axisID ) const;
	bool	IsAxisPressed( AxisID axisID ) const;
	bool	IsAxisPressedOrHeld( AxisID axisID ) const;

	//Buttons
	bool IsButtonPressed( ButtonID buttonID ) const;
	bool IsButtonPressedOrHeld( ButtonID buttonID ) const;


	//Data Members
	ID gamepadID;
	PlayerID playerID;
	size_t numberOfAxes;
	size_t numberOfButtons;

	float axisDeadzone;
	Axis currAxisState[ MAX_NUMBER_OF_AXES ];
	Axis lastAxisState[ MAX_NUMBER_OF_AXES ];

	Button currButtonState[ MAX_NUMBER_OF_BUTTONS ];
	Button lastButtonState[ MAX_NUMBER_OF_BUTTONS ];
};



#pragma region Constructors
//----------------------------------------------------------------------------------------------------
inline Gamepad::Gamepad()
	: gamepadID( ID_Null )
	, playerID( PLAYER_None )
	, numberOfAxes( 0 )
	, numberOfButtons( 0 )
	, axisDeadzone( 0.f )
	, currAxisState()
	, lastAxisState()
	, currButtonState()
	, lastButtonState()
{
	memset( currAxisState, 0, MAX_NUMBER_OF_AXES );
	memset( lastAxisState, 0, MAX_NUMBER_OF_AXES );

	memset( currButtonState, 0, MAX_NUMBER_OF_BUTTONS );
	memset( lastButtonState, 0, MAX_NUMBER_OF_BUTTONS );
}

//----------------------------------------------------------------------------------------------------
inline Gamepad::Gamepad( ID padID, size_t numAxes, size_t numButtons,
						 float axisDeadzones, PlayerID playID )
	: gamepadID( padID )
	, playerID( playID )
	, numberOfAxes( numAxes )
	, numberOfButtons( numButtons )
	, axisDeadzone( axisDeadzones )
	, currAxisState()
	, lastAxisState()
	, currButtonState()
	, lastButtonState()
{
	memset( currAxisState, 0, MAX_NUMBER_OF_AXES );
	memset( lastAxisState, 0, MAX_NUMBER_OF_AXES );

	memset( currButtonState, 0, MAX_NUMBER_OF_BUTTONS );
	memset( lastButtonState, 0, MAX_NUMBER_OF_BUTTONS );
}
#pragma endregion //Constructors



//----------------------------------------------------------------------------------------------------
inline float Gamepad::GetSmoothedAxisValue( AxisID axisID ) const
{
	if( fabs( currAxisState[ axisID ] ) < axisDeadzone )
		return 0.f;
	else
	{
		if( currAxisState[ axisID ] > axisDeadzone )
			return ( currAxisState[ axisID ] - axisDeadzone ) / ( 1.f - axisDeadzone );
		else
			return ( currAxisState[ axisID ] + axisDeadzone ) / ( 1.f - axisDeadzone );

	}
}

//----------------------------------------------------------------------------------------------------
inline bool Gamepad::IsAxisPressed( AxisID axisID ) const
{
	bool axisPressedLastFrame = ( std::abs( lastAxisState[ axisID ] ) > axisDeadzone );

	return axisPressedLastFrame && IsAxisPressedOrHeld( axisID );
}

//----------------------------------------------------------------------------------------------------
inline bool Gamepad::IsAxisPressedOrHeld( AxisID axisID ) const
{
	return ( std::abs( currAxisState[ axisID ] ) > axisDeadzone );
}

//----------------------------------------------------------------------------------------------------
inline bool Gamepad::IsButtonPressed( ButtonID buttonID ) const
{
	bool buttonPressedLastFrame = ( lastButtonState[ buttonID ] > BUTTON_DEADZONE );

	return !buttonPressedLastFrame && IsButtonPressedOrHeld( buttonID );
}

//----------------------------------------------------------------------------------------------------
inline bool Gamepad::IsButtonPressedOrHeld( ButtonID buttonID ) const
{
	return ( currButtonState[ buttonID ] > BUTTON_DEADZONE );
}
#pragma warning( pop )

#endif //INCLUDED_GAMEPAD_HPP
