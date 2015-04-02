#pragma once
#ifndef INCLUDED_DELEGATE_HPP
#define INCLUDED_DELEGATE_HPP
/************************************************************************************************
This fast, dynamic-allocation free, generic type delegate wasn't made by me.

It is the work of Oliver Mueller at http://blog.coldflake.com/posts/2014-01-12-C%2B%2B-delegates-on-steroids.html.

The only changes that have been made are modifying the names and style to reflect my current preferences.

Last Edited by Vincent Kocks.
************************************************************************************************/

//-----------------------------------------------------------------------------------------------
#include "EngineMacros.hpp"

//-----------------------------------------------------------------------------------------------
#if defined USE_BAD_CODE //This seems to be broken on Emscripten; not sure why
//#if  ( !defined COMPILER_MICROSOFT_C ) || ( COMPILER_MICROSOFT_C_VERSION >= 1800 ) 
//Variadic Delegates are a later C++11 feature and are only supported in VS2013+

#pragma region Variadic Delegate
template<typename ReturnType, typename... FunctionArguments>
class Delegate
{
	typedef ReturnType (*WrappedOneArgFunctionPointer)( void* callee, FunctionArguments... );

public:
	Delegate( WrappedOneArgFunctionPointer functionToDelegate )
		: m_delegateCallee( nullptr )
		, m_delegatedFunction( functionToDelegate ) 
	{}
	Delegate( void* callee, WrappedOneArgFunctionPointer functionToDelegate )
		: m_delegateCallee( callee )
		, m_delegatedFunction( functionToDelegate ) 
	{}

	template <ReturnType (*StaticFunction)(FunctionArguments...)>
	static Delegate GenerateFromFunction()
	{
		Delegate delegateFunc( &StaticFunctionCaller<StaticFunction> );
		return delegateFunc;
	}

	template <typename ObjectType, ReturnType (ObjectType::*ObjectFunction)(FunctionArguments...)>
	static Delegate GenerateFromFunction( ObjectType* callee )
	{
		Delegate delegateFunc( callee, &ObjectFunctionCaller<ObjectType, ObjectFunction> );
		return delegateFunc;
	}

	ReturnType operator()(FunctionArguments... args) const
	{
		return (*m_delegatedFunction)(m_delegateCallee, args...);
	}

private:

	void* m_delegateCallee;
	WrappedOneArgFunctionPointer m_delegatedFunction;

	template <ReturnType (*StaticFunction)(FunctionArguments...)>
	static ReturnType StaticFunctionCaller( void* /*callee*/, FunctionArguments... args )
	{
		return (*StaticFunction)(args...);
	}

	template <typename ObjectType, ReturnType (ObjectType::*ObjectFunction)(FunctionArguments...)>
	static ReturnType ObjectFunctionCaller( void* callee, FunctionArguments... args )
	{
		ObjectType* objectPointer = static_cast<ObjectType*>(callee);
		return (objectPointer->*ObjectFunction)(args...);
	}
};
#pragma endregion //Variadic Delegate

#else //Pre-C++11 Version

template<typename ReturnType, typename FunctionArgument>
class Delegate
{
	typedef ReturnType (*WrappedZeroArgFunctionPointer)( void* callee );
	typedef ReturnType (*WrappedOneArgFunctionPointer)( void* callee, FunctionArgument& arg );
	
public:
 	const void* GetCallee() { return m_delegateCallee; }

	Delegate()
		: m_delegateCallee( nullptr )
		, m_delegatedZeroArgFunction( nullptr ) 
	{}
#pragma region Zero Argument
	Delegate( WrappedZeroArgFunctionPointer functionToDelegate )
		: m_delegateCallee( nullptr )
		, m_delegatedZeroArgFunction( functionToDelegate ) 
	{}
	Delegate( void* callee, WrappedZeroArgFunctionPointer functionToDelegate )
		: m_delegateCallee( callee )
		, m_delegatedZeroArgFunction( functionToDelegate ) 
	{}

	template <ReturnType (*StaticFunction)()>
	static Delegate GenerateFromZeroArgFunction()
	{
		Delegate delegateFunc( &ZeroArgStaticFunctionCaller<StaticFunction> );
		return delegateFunc;
	}

	template <typename ObjectType, ReturnType (ObjectType::*ObjectFunction)()>
	static Delegate GenerateFromZeroArgFunction( ObjectType* callee )
	{
		Delegate delegateFunc( callee, &ZeroArgObjectFunctionCaller<ObjectType, ObjectFunction> );
		return delegateFunc;
	}

	ReturnType operator()() const
	{
		return (*m_delegatedZeroArgFunction)(m_delegateCallee);
	}

private:
	template <ReturnType (*StaticFunction)()>
	static ReturnType ZeroArgStaticFunctionCaller( void* /*callee*/ )
	{
		return (*StaticFunction)();
	}

	template <typename ObjectType, ReturnType (ObjectType::*ObjectFunction)()>
	static ReturnType ZeroArgObjectFunctionCaller( void* callee )
	{
		ObjectType* objectPointer = static_cast<ObjectType*>(callee);
		return (objectPointer->*ObjectFunction)();
	}
#pragma endregion //Zero Argument

#pragma region One Argument
public:
	Delegate( WrappedOneArgFunctionPointer functionToDelegate )
		: m_delegateCallee( nullptr )
		, m_delegatedOneArgFunction( functionToDelegate ) 
	{}
	Delegate( void* callee, WrappedOneArgFunctionPointer functionToDelegate )
		: m_delegateCallee( callee )
		, m_delegatedOneArgFunction( functionToDelegate ) 
	{}

	template <ReturnType (*StaticFunction)(FunctionArgument&)>
	static Delegate GenerateFromOneArgFunction()
	{
		Delegate delegateFunc( &OneArgStaticFunctionCaller<StaticFunction> );
		return delegateFunc;
	}
	
	template <typename ObjectType, ReturnType (ObjectType::*ObjectFunction)(FunctionArgument&)>
	static Delegate GenerateFromOneArgFunction( ObjectType* callee )
	{
		Delegate delegateFunc( callee, &OneArgObjectFunctionCaller<ObjectType, ObjectFunction> );
		return delegateFunc;
	}

	ReturnType operator()( FunctionArgument& argument ) const
	{
		return (*m_delegatedOneArgFunction)( m_delegateCallee, argument );
	}

	bool operator==( const Delegate& other )
	{
		if( m_delegateCallee == other.m_delegateCallee && 
			m_delegatedZeroArgFunction == other.m_delegatedZeroArgFunction &&
			m_delegatedOneArgFunction == other.m_delegatedOneArgFunction )
			return true;
		return false;
	}


private:
	template <ReturnType (*StaticFunction)(FunctionArgument&)>
	static ReturnType OneArgStaticFunctionCaller( void* /*callee*/, FunctionArgument& arg )
	{
		return (*StaticFunction)( arg);
	}

	template <typename ObjectType, ReturnType (ObjectType::*ObjectFunction)(FunctionArgument&)>
	static ReturnType OneArgObjectFunctionCaller( void* callee, FunctionArgument& arg )
	{
		ObjectType* objectPointer = static_cast<ObjectType*>(callee);
		return (objectPointer->*ObjectFunction)( arg );
	}
#pragma endregion //One Argument


private:
	void* m_delegateCallee;
	WrappedZeroArgFunctionPointer m_delegatedZeroArgFunction;
	WrappedOneArgFunctionPointer m_delegatedOneArgFunction;
};
#endif // Pre-C++11 version

#endif //INCLUDED_DELEGATE_HPP