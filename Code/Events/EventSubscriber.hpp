#pragma once
#ifndef INCLUDED_EVENT_SUBSCRIBER_HPP
#define INCLUDED_EVENT_SUBSCRIBER_HPP

//-----------------------------------------------------------------------------------------------
#include "../EngineMacros.hpp"
#include "EventCourier.hpp"


//-----------------------------------------------------------------------------------------------
ABSTRACT class EventSubscriber
{
protected:
	EventSubscriber() { }
	virtual ~EventSubscriber();


private:
};



//-----------------------------------------------------------------------------------------------
VIRTUAL inline EventSubscriber::~EventSubscriber()
{
	EventCourier::UnsubscribeFromAllEvents( this );
}

#endif //INCLUDED_EVENT_SUBSCRIBER_HPP
