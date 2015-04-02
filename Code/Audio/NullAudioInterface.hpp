#pragma once
#ifndef INCLUDED_NULL_AUDIO_INTERFACE_HPP
#define INCLUDED_NULL_AUDIO_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include <string>

#include "AudioInterface.hpp"


//-----------------------------------------------------------------------------------------------
class NullAudioInterface : public AudioInterface
{
	friend class AudioInterface;

public:
	//Lifecycle
	void PostStartup() { }
	void DoUpdate( float /*deltaSeconds*/ ) { }
	void PreShutdown() { }


	//Public Interface
	SoundID DoGetOrLoadSound( const std::string& /*soundFileLocation*/ ) { return 0; }

	bool DoIsEmitterPaused( EmitterID /*emitter*/ ) { return false; }
	bool DoIsEmitterPlaying( EmitterID /*emitter*/ ) { return true; }
	void DoPauseEmitter( EmitterID /*emitter*/ ) { }
	EmitterID DoPlaySoundThroughEmitter( SoundID /*sound*/, EmitterID /*emitter = ANY_EMITTER*/, bool /*shouldLoop = false*/ ) { return 0; }
	void DoStopEmitter( EmitterID /*emitter*/ ) { }
	void DoUnpauseEmitter( EmitterID /*emitter*/ ) { }

private:
	// Should only be made by our Audio Interface
	NullAudioInterface() { }
	~NullAudioInterface() { }
};

#endif //INCLUDED_NULL_AUDIO_INTERFACE_HPP