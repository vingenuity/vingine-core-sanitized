#pragma once
#ifndef INCLUDED_TOUCH_SCREEN_HPP
#define INCLUDED_TOUCH_SCREEN_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include "../AssertionError.hpp"
#include "../TimeInterface.hpp"


//-----------------------------------------------------------------------------------------------
enum TouchType
{
	TOUCH_None,
	TOUCH_Began,
	TOUCH_Held,
	TOUCH_Moved,
	TOUCH_Ended,
	TOUCH_Cancelled,
	NUMBER_OF_TOUCH_TYPES
};



//-----------------------------------------------------------------------------------------------
struct TouchPoint
{
	TouchPoint( int sysTouchID, float touchX, float touchY, TouchType touchType = TOUCH_None )
		: type( touchType )
		, systemTouchID( sysTouchID )
		, location( touchX, touchY )
		, startTime( GetCurrentTimeSeconds() )
	{ }

	//Data Members
	TouchType type;
	int systemTouchID;
	FloatVector2 location;
	double startTime;
};



//-----------------------------------------------------------------------------------------------
class TouchScreen
{
	static const unsigned int MAX_TOUCHES_SUPPORTED = 10;

public:
	//Lifecycle
	TouchScreen() { }
	void Update( float deltaSeconds );
	~TouchScreen();

	//General Touch Stats
	unsigned int FindNumberOfStartedTouches() const;
	unsigned int GetNumberOfTouches() const { return m_currentTouches.size();}

	//Touch Accessors
	TouchPoint* FindTouchPointWithID( int touchID ) { return const_cast< TouchPoint* >( FindConstTouchPointWithID( touchID ) ); }
	const TouchPoint* FindConstTouchPointWithID( int touchID ) const;
	const TouchPoint& GetTouchPointAtIndex( unsigned int index ) const;

	//Touch Modifiers
	void AddOrUpdateTouchPoint( int sysTouchID, float touchX, float touchY, TouchType touchType );


private:
	//Data Members
	std::vector< TouchPoint* > m_currentTouches;
};



//-----------------------------------------------------------------------------------------------
inline void TouchScreen::Update( float /*deltaSeconds*/ )
{
	for( unsigned int i = 0; i < m_currentTouches.size(); ++i )
	{
		TouchPoint*& touchpoint = m_currentTouches[ i ];

		if( touchpoint->type == TOUCH_Began )
			touchpoint->type = TOUCH_Held;

		if( touchpoint->type == TOUCH_Ended )
		{
			delete m_currentTouches[ i ];
			m_currentTouches.erase( m_currentTouches.begin() + i );
			--i;
		}
	}
}

//-----------------------------------------------------------------------------------------------
inline TouchScreen::~TouchScreen()
{
	for( unsigned int i = 0; i < m_currentTouches.size(); ++i )
	{
		delete m_currentTouches[ i ];
	}
	m_currentTouches.clear();
}

//-----------------------------------------------------------------------------------------------
inline unsigned int TouchScreen::FindNumberOfStartedTouches() const
{
	unsigned int numStartedTouches = 0;
	for( unsigned int i = 0; i < m_currentTouches.size(); ++i )
	{
		if( m_currentTouches[ i ]->type == TOUCH_Began )
			++numStartedTouches;
	}
	return numStartedTouches;
}

//-----------------------------------------------------------------------------------------------
inline const TouchPoint* TouchScreen::FindConstTouchPointWithID( int touchID ) const
{
	for( unsigned int touchPointIndex = 0; touchPointIndex < m_currentTouches.size(); ++touchPointIndex )
	{
		const TouchPoint* const& touchPoint = m_currentTouches[ touchPointIndex ];

		if( touchPoint->systemTouchID == touchID )
			return m_currentTouches[ touchPointIndex ];
	}
	return nullptr;
}

//-----------------------------------------------------------------------------------------------
inline const TouchPoint& TouchScreen::GetTouchPointAtIndex( unsigned int index ) const
{
	FATAL_ASSERTION( index < m_currentTouches.size(), "Touchscreen Error",
		"An attempt was made to access a touch index that was out of range." );

	return *m_currentTouches[ index ];
}

//-----------------------------------------------------------------------------------------------
inline void TouchScreen::AddOrUpdateTouchPoint( int sysTouchID, float touchX, float touchY, TouchType touchType )
{
	TouchPoint* storedTouch = FindTouchPointWithID( sysTouchID );
	if( storedTouch == nullptr )
	{
		TouchPoint* newTouch = new TouchPoint( sysTouchID, touchX, touchY, touchType );
		m_currentTouches.push_back( newTouch );
	}
	else
	{
		storedTouch->type = touchType;
		storedTouch->location.x = touchX;
		storedTouch->location.y = touchY;
	}
}

#endif //INCLUDED_TOUCH_SCREEN_HPP