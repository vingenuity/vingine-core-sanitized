#pragma once
#ifndef INCLUDED_AUDIO_INTERFACE_HPP
#define INCLUDED_AUDIO_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include <string>

#include "../EngineMacros.hpp"


//-----------------------------------------------------------------------------------------------
ABSTRACT SINGLETON class AudioInterface
{
public:
	//Abstracted Audio Identifiers
	typedef size_t SoundID;

	typedef size_t EmitterID;
	static const EmitterID ANY_EMITTER = 0;

	typedef size_t ListenerID;
	static const ListenerID DEFAULT_LISTENER = 0;


	//Lifecycle
	static void Startup();
	static void Update( float deltaSeconds ) { s_activeAudioInterface->DoUpdate( deltaSeconds ); }
	static void Shutdown();


	//Public Interface
	static SoundID GetOrLoadSound( const std::string& soundFileLocation ) { return s_activeAudioInterface->DoGetOrLoadSound( soundFileLocation ); }

	static bool IsEmitterPaused( EmitterID emitter ) { return s_activeAudioInterface->DoIsEmitterPaused( emitter ); }
	static bool IsEmitterPlaying( EmitterID emitter ) { return s_activeAudioInterface->DoIsEmitterPlaying( emitter ); }
	static void PauseEmitter( EmitterID emitter ) { s_activeAudioInterface->DoPauseEmitter( emitter ); }
	static EmitterID PlaySoundThroughEmitter( SoundID sound, EmitterID emitter = ANY_EMITTER, bool shouldLoop = false ) { return s_activeAudioInterface->DoPlaySoundThroughEmitter( sound, emitter, shouldLoop ); }
	static void StopEmitter( EmitterID emitter ) { s_activeAudioInterface->DoStopEmitter( emitter ); }
	static void UnpauseEmitter( EmitterID emitter ) { s_activeAudioInterface->DoUnpauseEmitter( emitter ); }


protected:
	// Should only be called through Startup() and Shutdown() by our derivers
	AudioInterface() { }
	virtual ~AudioInterface() { }


private:
	//We have no need of a pithy assignment or copy operator!
	AudioInterface( const AudioInterface& other );
	AudioInterface& operator=( const AudioInterface& other );


	//Internal Lifecycle
	virtual void PostStartup() = 0;
	virtual void DoUpdate( float deltaSeconds ) = 0;
	virtual void PreShutdown() = 0;

	//Internal Interface
	virtual SoundID DoGetOrLoadSound( const std::string& soundFileLocation ) = 0;

	virtual bool DoIsEmitterPaused( EmitterID emitter ) = 0;
	virtual bool DoIsEmitterPlaying( EmitterID emitter ) = 0;
	virtual void DoPauseEmitter( EmitterID emitter ) = 0;
	virtual EmitterID DoPlaySoundThroughEmitter( SoundID sound, EmitterID emitter = ANY_EMITTER, bool shouldLoop = false ) = 0;
	virtual void DoStopEmitter( EmitterID emitter ) = 0;
	virtual void DoUnpauseEmitter( EmitterID emitter ) = 0;


	//Data Members
	static AudioInterface* s_activeAudioInterface;
};

#endif //INCLUDED_AUDIO_INTERFACE_HPP
