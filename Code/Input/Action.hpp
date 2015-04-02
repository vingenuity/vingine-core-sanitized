#pragma once
#ifndef INCLUDED_ACTION_HPP
#define INCLUDED_ACTION_HPP

//-----------------------------------------------------------------------------------------------
#include "../AssertionError.hpp"
#include "../HashedString.hpp"

//-----------------------------------------------------------------------------------------------
typedef HashedString ActionName;
typedef float ContinuousValue;
typedef bool DiscreteValue;

//-----------------------------------------------------------------------------------------------
struct Action
{
	//Value Types
	typedef unsigned char ValueType;
	static const ValueType VALUE_None = 0;
	static const ValueType VALUE_Continuous = 1;
	static const ValueType VALUE_Discrete = 2;


	//Constructors
	Action();
	Action( const std::string& actionName, ValueType type );


	//Convenience Functions
	ContinuousValue GetContinuousValue() const { return value.continuous; }
	DiscreteValue GetDiscreteValue() const { return value.discrete; }

	void SetValue( ContinuousValue continuousAction );
	void SetValue( DiscreteValue discreteAction );


	//Data Members
	ActionName name;
	ValueType valueType;
	union Value
	{
		ContinuousValue continuous;
		DiscreteValue discrete;
	} value;
};



#pragma region Constructors
//-----------------------------------------------------------------------------------------------
inline Action::Action()
	: name()
	, valueType( VALUE_None )
{ }

//-----------------------------------------------------------------------------------------------
inline Action::Action( const std::string& actionName, ValueType type )
	: name( actionName )
	, valueType( type )
{ }
#pragma endregion



#pragma region Convenience Functions
//-----------------------------------------------------------------------------------------------
inline void Action::SetValue( ContinuousValue continuousAction )
{
	RECOVERABLE_ASSERTION( valueType != VALUE_Continuous, "Action Error", 
				"A function has tried to set a continuous value to non-continuous Action." );

	value.continuous = continuousAction;
}

//-----------------------------------------------------------------------------------------------
inline void Action::SetValue( DiscreteValue discreteAction )
{
	RECOVERABLE_ASSERTION( valueType != VALUE_Discrete, "Action Error", 
		"A function has tried to set a discrete value to non-discrete Action." );

	value.discrete = discreteAction;
}
#pragma endregion

#endif //INCLUDED_ACTION_HPP
