#ifndef INCLUDED_KEYBOARD_HPP
#define INCLUDED_KEYBOARD_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <cstring>
#include <queue>
#include <string.h>

#include "../Bitmasking.hpp"

//-----------------------------------------------------------------------------------------------
class Keyboard
{
	static const unsigned int NUMBER_OF_KEYS = 256;

public:
	typedef unsigned char Key;
	#pragma region Key Declarations
	static const Key A;
	static const Key B;
	static const Key C;
	static const Key D;
	static const Key E;
	static const Key F;
	static const Key G;
	static const Key H;
	static const Key I;
	static const Key J;
	static const Key K;
	static const Key L;
	static const Key M;
	static const Key N;
	static const Key O;
	static const Key P;
	static const Key Q;
	static const Key R;
	static const Key S;
	static const Key T;
	static const Key U;
	static const Key V;
	static const Key W;
	static const Key X;
	static const Key Y;
	static const Key Z;

	static const Key GRAVE;
	static const Key TILDE;
	static const Key NUMBER_1;
	static const Key NUMBER_2;
	static const Key NUMBER_3;
	static const Key NUMBER_4;
	static const Key NUMBER_5;
	static const Key NUMBER_6;
	static const Key NUMBER_7;
	static const Key NUMBER_8;
	static const Key NUMBER_9;
	static const Key NUMBER_0;
	static const Key MINUS;
	static const Key EQUALS;

	static const Key FUNCTION_1;
	static const Key FUNCTION_2;
	static const Key FUNCTION_3;
	static const Key FUNCTION_4;
	static const Key FUNCTION_5;
	static const Key FUNCTION_6;
	static const Key FUNCTION_7;
	static const Key FUNCTION_8;
	static const Key FUNCTION_9;
	static const Key FUNCTION_10;
	static const Key FUNCTION_11;
	static const Key FUNCTION_12;

	static const Key BACKSPACE;
	static const Key SPACEBAR;
	static const Key ENTER;
	static const Key ESCAPE;
	static const Key TAB;
	static const Key LEFT_ALT;
	static const Key RIGHT_ALT;
	static const Key LEFT_CONTROL;
	static const Key RIGHT_CONTROL;
	static const Key LEFT_SHIFT;
	static const Key RIGHT_SHIFT;

	static const Key INSERT;
	static const Key DELETE_KEY;
	static const Key HOME;
	static const Key END;
	static const Key PAGE_UP;
	static const Key PAGE_DOWN;

	static const Key ARROW_UP;
	static const Key ARROW_DOWN;
	static const Key ARROW_LEFT;
	static const Key ARROW_RIGHT;

	static const Key NUMPAD_0;
	static const Key NUMPAD_1;
	static const Key NUMPAD_2;
	static const Key NUMPAD_3;
	static const Key NUMPAD_4;
	static const Key NUMPAD_5;
	static const Key NUMPAD_6;
	static const Key NUMPAD_7;
	static const Key NUMPAD_8;
	static const Key NUMPAD_9;
	static const Key NUMPAD_ENTER;
	static const Key NUMPAD_PERIOD;
	static const Key NUMPAD_PLUS;
	static const Key NUMPAD_MINUS;
	static const Key NUMPAD_TIMES;
	static const Key NUMPAD_DIVIDE;
	#pragma endregion //Key Declarations

	//Key Modifiers
	typedef unsigned int KeyModifier;
	static const KeyModifier MODIFIER_NONE;
	static const KeyModifier MODIFIER_CONTROL;
	static const KeyModifier MODIFIER_SHIFT;
	static const KeyModifier MODIFIER_CTRL_SHIFT;
	static const KeyModifier MODIFIER_ALT;
	static const KeyModifier MODIFIER_CTRL_ALT;
	static const KeyModifier MODIFIER_ALT_SHIFT;
	static const KeyModifier MODIFIER_CTRL_ALT_SHIFT;

private:
	std::queue< unsigned char > m_characterInput;
	KeyModifier				m_keyModifiers;

	bool m_wasKeyDown[ NUMBER_OF_KEYS ];
	bool m_isKeyDown [ NUMBER_OF_KEYS ];

public:
	Keyboard()
	{
		memset( m_isKeyDown, 0, NUMBER_OF_KEYS );
		memset( m_wasKeyDown, 0, NUMBER_OF_KEYS );
		m_keyModifiers = MODIFIER_NONE;
	}

	void AddCharacterToInputQueue( unsigned char key ) { m_characterInput.push( key ); }
	void ClearCurrentState() { memset( m_isKeyDown, 0, NUMBER_OF_KEYS ); }
	void ClearInputQueue();
	bool InputQueueIsEmpty() const { return m_characterInput.empty(); }
	unsigned char GetCharacterFromInputQueue();

	bool KeyIsPressedOrHeld( Key key ) const	{ return m_isKeyDown[ key ];  }
	bool KeyIsNotPressedOrHeld( Key key ) const	{ return !KeyIsPressedOrHeld( key ); }

	bool KeyIsPressed( Key key ) const			{ return ( m_isKeyDown[ key ] && !m_wasKeyDown[ key ] ); }
	bool KeyIsNotPressed( Key key ) const		{ return !KeyIsPressed( key ); }

	bool KeyCombinationIsPressed( KeyModifier modifier, Key key );
	bool KeyCombinationIsNotPressed( KeyModifier modifier, Key key ) { return !KeyCombinationIsPressed( modifier, key ); }

	void SetKeyDown( Key key );
	//void SetKeyDown( unsigned char key ) { SetKeyDown( Key( key ) ); };
	//void SetKeyUp( unsigned char key ) { SetKeyUp( Key( key ) ); };
	void SetKeyUp( Key key );

	void Update();
};







//-----------------------------------------------------------------------------------------------
inline void Keyboard::ClearInputQueue() 
{ 
	while ( !m_characterInput.empty() )
	{
		m_characterInput.pop();
	}
}

//-----------------------------------------------------------------------------------------------
inline bool Keyboard::KeyCombinationIsPressed( KeyModifier modifier, Key key )
{
	if( modifier != m_keyModifiers )
	{
		if( !IsBitSetInMask( static_cast< unsigned int >( modifier ), m_keyModifiers ) )
			return false;
	}

	if( KeyIsNotPressed( key ) )
		return false;
	return true;
}

//-----------------------------------------------------------------------------------------------
inline unsigned char Keyboard::GetCharacterFromInputQueue()
{ 
	unsigned char frontChar = m_characterInput.front();
	m_characterInput.pop(); 
	return frontChar;
}

//-----------------------------------------------------------------------------------------------
inline void Keyboard::Update()
{
	for( unsigned int key = 0; key < NUMBER_OF_KEYS; ++key )
	{
		if( m_isKeyDown[ key ] )
		{
			m_wasKeyDown[ key ] = true;
		}
		else
			m_wasKeyDown[ key ] = false;
	}
	ClearInputQueue();
}

#endif //INCLUDED_KEYBOARD_HPP
