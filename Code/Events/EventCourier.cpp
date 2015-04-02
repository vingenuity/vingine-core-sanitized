#include "EventCourier.hpp"


//-----------------------------------------------------------------------------------------------
STATIC EventCourier* EventCourier::s_activeEventCourier = nullptr;
STATIC EventDataBundle EventCourier::DEFAULT_EVENT_DATA = EventDataBundle();



#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
STATIC void EventCourier::Startup()
{
	FATAL_ASSERTION( s_activeEventCourier == nullptr, "Event Courier Error",
		"Cannot start up multiple Event Couriers!" );

	s_activeEventCourier = new EventCourier();
}

//-----------------------------------------------------------------------------------------------
STATIC void EventCourier::Shutdown()
{
	RECOVERABLE_ASSERTION( s_activeEventCourier != nullptr, "Event Courier Error",
		"Cannot shut down an Event Courier that has not yet been started!" );
	
	delete s_activeEventCourier;
}
#pragma endregion //Lifecycle



#pragma region Private Instance Interface
//-----------------------------------------------------------------------------------------------
void EventCourier::DoSendEvent( const EventName& eventName, EventDataBundle& eventData )
{
	EventObserverMap::iterator eventNameObserverPair = m_eventMapping.find( eventName );
	if( eventNameObserverPair == m_eventMapping.end() )
	{
		//TODO: Warn?
		//No observer list for this event name
		return;
	}
	std::vector< EventObserver >& observersForThisEvent = eventNameObserverPair->second;

	std::vector< EventObserver >::iterator observer;
	for( observer = observersForThisEvent.begin(); observer != observersForThisEvent.end(); ++observer )
	{
		(*observer)( eventData );
	}
}

//-----------------------------------------------------------------------------------------------
void EventCourier::DoSubscribeForEvent( const EventName& eventName, const EventObserver& observer )
{
	std::vector< EventObserver >& observersForThisEvent = m_eventMapping[ eventName ];

	bool observerNotAlreadySubscribed = true;
	std::vector< EventObserver >::iterator observerInArray;
	for(	observerInArray = observersForThisEvent.begin(); 
			observerInArray != observersForThisEvent.end();
			++observerInArray )
	{
		if( *observerInArray == observer )
		{
			//TODO: Warn?
			observerNotAlreadySubscribed = false;
			break;
		}
	}

	if( observerNotAlreadySubscribed )
	{
		observersForThisEvent.push_back( observer );
	}
}

//-----------------------------------------------------------------------------------------------
void EventCourier::DoUnsubscribeFromEvent( const EventName& eventName, const EventObserver& observer )
{
	EventObserverMap::iterator eventNameObserverPair = m_eventMapping.find( eventName );
	if( eventNameObserverPair == m_eventMapping.end() )
	{
		//TODO: Warn?
		//No observer list for this event name
		return;
	}
	std::vector< EventObserver >& observersForThisEvent = eventNameObserverPair->second;

	std::vector< EventObserver >::iterator observerInArray;
	for(	observerInArray = observersForThisEvent.begin(); 
			observerInArray != observersForThisEvent.end(); )
	{
		if( *observerInArray == observer )
			observerInArray = observersForThisEvent.erase( observerInArray );
		else
			++observerInArray;
	}
}
#pragma endregion //Private Instance Interface
