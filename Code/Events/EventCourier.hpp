#pragma once
#ifndef INCLUDED_EVENT_COURIER_HPP
#define INCLUDED_EVENT_COURIER_HPP

//-----------------------------------------------------------------------------------------------
#include <map>
#include <vector>

#include "../AssertionError.hpp"
#include "../Delegate.hpp"
#include "../EngineMacros.hpp"
#include "../NamedDataBundle.hpp"

typedef HashedString EventName;
typedef NamedDataBundle EventDataBundle;
typedef Delegate< void, EventDataBundle > EventObserver;


//-----------------------------------------------------------------------------------------------
SINGLETON class EventCourier
{
private:
	typedef std::map< EventName, std::vector< EventObserver > > EventObserverMap;


public:
	//Lifecycle
	static void Startup();
	static void Shutdown();

	//Static Public Interface
	static void SendEvent( const EventName& eventName );
	static void SendEvent( const EventName& eventName, EventDataBundle& eventData );

	static void SubscribeForEvent( const EventName& eventName, const EventObserver& observer );
	static void UnsubscribeFromEvent( const EventName& eventName, const EventObserver& observer );
	
	template<typename EventSubscriber>
	static void UnsubscribeFromAllEvents( const EventSubscriber* subscriber );


private:
	EventCourier() { }
	~EventCourier() { }

	//Private Instance Interface
	void DoSendEvent( const EventName& eventName, EventDataBundle& eventData );

	void DoSubscribeForEvent( const EventName& eventName, const EventObserver& observer );
	void DoUnsubscribeFromEvent( const EventName& eventName, const EventObserver& observer );
	
	template<typename EventSubscriber>
	void DoUnsubscribeFromAllEvents( const EventSubscriber* subscriber );

	//Static Members
	static EventCourier* s_activeEventCourier;
	static EventDataBundle DEFAULT_EVENT_DATA;

	//Data Members
	EventObserverMap m_eventMapping;
};



#pragma region Static Public Interface
//-----------------------------------------------------------------------------------------------
STATIC inline void EventCourier::SendEvent( const EventName& eventName )
{
	FATAL_ASSERTION( s_activeEventCourier != nullptr, "Event Courier Error",
		"The event courier was used without being started up!" );

	s_activeEventCourier->DoSendEvent( eventName, DEFAULT_EVENT_DATA );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void EventCourier::SendEvent( const EventName& eventName, EventDataBundle& eventData )
{
	FATAL_ASSERTION( s_activeEventCourier != nullptr, "Event Courier Error",
		"The event courier was used without being started up!" );

	s_activeEventCourier->DoSendEvent( eventName, eventData );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void EventCourier::SubscribeForEvent( const EventName& eventName, const EventObserver& observer )
{
	FATAL_ASSERTION( s_activeEventCourier != nullptr, "Event Courier Error",
		"The event courier was used without being started up!" );

	s_activeEventCourier->DoSubscribeForEvent( eventName, observer );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void EventCourier::UnsubscribeFromEvent( const EventName& eventName, const EventObserver& observer )
{
	FATAL_ASSERTION( s_activeEventCourier != nullptr, "Event Courier Error",
		"The event courier was used without being started up!" );

	s_activeEventCourier->DoSubscribeForEvent( eventName, observer );
}

//-----------------------------------------------------------------------------------------------
template< typename EventSubscriber >
STATIC inline void EventCourier::UnsubscribeFromAllEvents( const EventSubscriber* subscriber )
{
	FATAL_ASSERTION( s_activeEventCourier != nullptr, "Event Courier Error",
		"The event courier was used without being started up!" );

	s_activeEventCourier->DoUnsubscribeFromAllEvents( subscriber );
}
#pragma endregion //Static Public Interface

//-----------------------------------------------------------------------------------------------
template<typename EventSubscriber>
void EventCourier::DoUnsubscribeFromAllEvents( const EventSubscriber* subscriber )
{
	EventObserverMap::iterator eventNameObserverPair;
	for( eventNameObserverPair = m_eventMapping.begin(); 
		eventNameObserverPair != m_eventMapping.end(); 
		++eventNameObserverPair )
	{
		std::vector< EventObserver >& observerArray = eventNameObserverPair->second;
		std::vector< EventObserver >::iterator observer;
		
		for( observer = observerArray.begin(); observer != observerArray.end(); )
		{
			if( observer->GetCallee() == subscriber )
				observer = observerArray.erase( observer );
			else
				++observer;
		}
	}
	return;
}

#endif //INCLUDED_EVENT_COURIER_HPP
