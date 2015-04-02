#pragma once
#ifndef INCLUDED_FMOD_AUDIO_INTERFACE_HPP
#define INCLUDED_FMOD_AUDIO_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include <map>
#include <vector>

#include "AudioInterface.hpp"
#include "../EngineMacros.hpp"

#pragma warning( disable : 4505 ) //FMOD_ErrorString() in fmod_errors.h is an unreferenced local
#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

#pragma region Linking Pragmas
#if defined( PLATFORM_PS3 )
/* As with the main PS3 includes in GameInterface,
 * these lib comments need the directory "$(SCE_PS3_ROOT)\host-win32\ppu\bin" to be part of the include directories.
 * The SN PS3 SDK should have this directory added by default. */
	#pragma comment( lib, "../lib/libaudio_stub.a" )
	#pragma comment( lib, "../lib/libfios.a" )
	#pragma comment( lib, "../lib/libmic_stub.a" )
	#pragma comment( lib, "../lib/libnet_stub.a" )
	#pragma comment( lib, "../lib/libsysutil_stub.a" )
	#pragma comment( lib, "../lib/libsysutil_avconf_ext_stub.a" )
	#pragma comment( lib, "../lib/libsysutil_np_stub.a" )

// This lib comment requires the FMOD SPU thread library to be inside Vingine's Libraries folder.
	#pragma comment( lib, "libfmod_sputhreads.a" )

#elif defined( PLATFORM_VITA )
	#pragma comment( lib, "libSceAudiodec_stub.a" )
	#pragma comment( lib, "libScePerf_stub.a" )
	#pragma comment( lib, "libSceAudio_stub.a" )
	#pragma comment( lib, "libSceAudioIn_stub.a" )
	#pragma comment( lib, "libSceGxm_stub.a" )
	#pragma comment( lib, "libSceDbgFont.a" )
	#pragma comment( lib, "libSceCtrl_stub.a" )
	#pragma comment( lib, "libSceDisplay_stub.a" )
	#pragma comment( lib, "libSceFpu.a" )
	#pragma comment( lib, "libSceSysmodule_stub.a" )
	#pragma comment( lib, "libSceNet_stub.a" )

// This lib comment requires the FMOD Vita library to be inside Vingine's Libraries folder.
	#pragma comment( lib, "libfmodL.a" )

#endif
#pragma endregion // Linking Pragmas


//-----------------------------------------------------------------------------------------------
SINGLETON class FMODAudioInterface : public AudioInterface
{
	struct SoundData
	{
		SoundData( FMOD::Sound* soundObject, char* soundBuffer ) : fmodSound( soundObject ), soundDataBuffer( soundBuffer ) { }
		FMOD::Sound* fmodSound;
		char* soundDataBuffer;
	};
	friend class AudioInterface;

	typedef FMOD::Channel EmitterChannel;
	typedef FMOD::Channel ListenerChannel;

public:
	//Lifecycle
	void PostStartup();
	void DoUpdate( float deltaSeconds );
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
	FMODAudioInterface();
	~FMODAudioInterface();

	//Error Checker
	static void VerifyFMODResultOrDie( FMOD_RESULT result );

	FMOD::System* m_fmodSystem;

	std::vector< EmitterChannel* > m_emitterRegistry;
	std::vector< ListenerChannel* > m_listenerRegistry;

	std::map< std::string, SoundID > m_soundIDMapping;
	std::vector< SoundData > m_soundRegistry;
};



//-----------------------------------------------------------------------------------------------
inline FMODAudioInterface::FMODAudioInterface()
	: AudioInterface()
	, m_fmodSystem( nullptr )
{ }

//-----------------------------------------------------------------------------------------------
inline FMODAudioInterface::~FMODAudioInterface()
{
	for( unsigned int i = 0; i < m_soundRegistry.size(); ++i )
	{
		//FMOD should take care of the FMOD::Sound objects.
		if( m_soundRegistry[i].soundDataBuffer != nullptr )
			delete m_soundRegistry[ i ].soundDataBuffer;
	}
}

#endif //INCLUDED_FMOD_AUDIO_INTERFACE_HPP
