#pragma once
#ifndef INCLUDED_GAME_INTERFACE_HPP
#define INCLUDED_GAME_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include "Events/EventCourier.hpp"
#include "EngineMacros.hpp"
#include "EntityManager.hpp"

/* These pragma comments are needed in order to force the compiler to link needed engine libraries in.
 * GameInterface is guaranteed to be used by the game, so these comment linkages will get rolled in.*/
#pragma region Platform-Specific Library Linkages
//-----------------------------------------------------------------------------------------------
#if defined( PLATFORM_ANDROID )
/* This function is used by the Android NDK's app glue in order to ensure the lifecycle functions don't get compiled out.
 * This is going to be called by GameInterface on startup in the Android build.*/
	extern "C" { void app_dummy(); }



//-----------------------------------------------------------------------------------------------
#elif defined( PLATFORM_PS3 )
/* These comments need the directory "$(SCE_PS3_ROOT)\host-win32\ppu\bin" to be part of the include directories.
 * Thankfully, the SN PS3 SDK has this directory added by default.*/
	#pragma comment( lib, "../lib/PSGL/RSX/debug/libPSGL.a" )
	#pragma comment( lib, "../lib/PSGL/RSX/debug/libPSGLU.a" )
	#pragma comment( lib, "../lib/libcgc.a" ) 
	#pragma comment( lib, "../lib/PSGL/RSX/debug/libPSGLcgc.a" )
	#pragma comment( lib, "../lib/libresc_stub.a" )
	#pragma comment( lib, "../lib/libgcm_cmd.a" )
	#pragma comment( lib, "../lib/libgcm_sys_stub.a" )
	#pragma comment( lib, "../lib/libio_stub.a" )
	#pragma comment( lib, "../lib/libsysmodule_stub.a" )
	#pragma comment( lib, "../lib/libsysutil_stub.a" )

#endif // defined( PLATFORM_MACROS )
#pragma endregion // Platform-Specific Library Linkages

//-----------------------------------------------------------------------------------------------
ABSTRACT SINGLETON class GameInterface // is an EventSubscriber (but can't use the class due to timing)
{
private:
	//We have no need of a pithy assignment or copy operator!
	GameInterface( const GameInterface& other );
	GameInterface& operator=( const GameInterface& other );

	//Only the interface has control over the internal pointer
	static GameInterface* s_gameInstancePointer;


protected:
	//These "systems" are so often needed by other systems and blueprints that they are now standard.
	EntityManager* m_activeEntityManager;

	virtual ~GameInterface() { }

	//Private Lifecycle
	virtual void DoBeforeEngineInitialization() { }
	virtual void DoBeforeFirstFrame( unsigned int /*windowWidth*/, unsigned int /*windowHeight*/ ) { }

	virtual void DoUpdate( float deltaSeconds ) = 0;
	virtual void DoRender() const = 0;
	virtual void DoAtEndOfFrame();

	virtual void DoBeforeEngineDestruction();
	virtual void DoAfterEngineDestruction() { }

	static bool s_engineShouldShutdown;

public:
	GameInterface();

	//Public Lifecycle
	static void BeforeEngineInitialization() { s_gameInstancePointer->DoBeforeEngineInitialization(); }
	static void BeforeFirstFrame( unsigned int windowWidth, unsigned int windowHeight ) { s_gameInstancePointer->DoBeforeFirstFrame( windowWidth, windowHeight ); }

	static void Update( float deltaSeconds ) { s_gameInstancePointer->DoUpdate( deltaSeconds ); }
	static void Render() { s_gameInstancePointer->DoRender(); }
	static void EndOfFrame() { s_gameInstancePointer->DoAtEndOfFrame(); }

	static void BeforeEngineDestruction() { s_gameInstancePointer->DoBeforeEngineDestruction(); }
	static void AfterEngineDestruction() { s_gameInstancePointer->DoAfterEngineDestruction(); }

	//Accessors
	static bool EngineShouldShutdown() { return s_engineShouldShutdown; }
	static EntityManager& GetEntityManager() { return *s_gameInstancePointer->m_activeEntityManager; }
};
#endif //INCLUDED_GAME_INTERFACE_HPP
