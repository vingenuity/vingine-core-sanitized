#pragma once
#ifndef INCLUDED_OPENAL_AUDIO_INTERFACE_HPP
#define INCLUDED_OPENAL_AUDIO_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include <map>
#include <vector>

#include "oal/al.h"
#include "oal/alc.h"
#include "AudioInterface.hpp"


//-----------------------------------------------------------------------------------------------
SINGLETON class OpenALAudioInterface : public AudioInterface
{
	friend class AudioInterface;

	typedef ALuint EmitterSourceID;
	//typedef FMOD::Channel ListenerChannel;
	typedef ALuint SoundBufferID;

public:
	//Lifecycle
	void PostStartup();
	void DoUpdate( float /*deltaSeconds*/ ) { }
	void PreShutdown();


	//Public Interface
	SoundID DoGetOrLoadSound( const std::string& soundFileLocation );

	bool DoIsEmitterPaused( EmitterID emitter );
	bool DoIsEmitterPlaying( EmitterID emitter );
	void DoPauseEmitter( EmitterID emitter );
	EmitterID DoPlaySoundThroughEmitter( SoundID sound, EmitterID emitter = ANY_EMITTER, bool shouldLoop = false );
	void DoStopEmitter( EmitterID emitter );
	void DoUnpauseEmitter( EmitterID emitter );

private:
	// Should only be made by our Audio Interface
	OpenALAudioInterface();
	~OpenALAudioInterface();

	//Helper Functions
	void DieIfOpenALErrorDetected();
	ALenum ConvertSoundFormatToOpenALEnumOrDie( unsigned int numChannels, unsigned int bitsPerSample );

	//Data Members
	ALCcontext* m_oalContext;
	ALCdevice* m_oalDevice;

	std::vector< EmitterSourceID > m_emitterRegistry;
	//std::vector< ListenerChannel* > m_listenerRegistry;

	std::map< std::string, SoundID > m_soundIDMapping;
	std::vector< SoundBufferID > m_soundRegistry;
};



//-----------------------------------------------------------------------------------------------
inline OpenALAudioInterface::OpenALAudioInterface()
	: AudioInterface()
	, m_oalContext( nullptr )
	, m_oalDevice( nullptr )
{ }

//-----------------------------------------------------------------------------------------------
inline OpenALAudioInterface::~OpenALAudioInterface()
{ }

#endif //INCLUDED_OPENAL_AUDIO_INTERFACE_HPP
