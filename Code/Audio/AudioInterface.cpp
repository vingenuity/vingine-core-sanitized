#include "AudioInterface.hpp"

#include "../AssertionError.hpp"
#include "../BuildPreferences.hpp"

#if defined( AUDIO_INTERFACE_USE_FMOD )
	#include "FMODAudioInterface.hpp"
#elif defined( AUDIO_INTERFACE_USE_OPENAL )
	#include "OpenALAudioInterface.hpp"
#elif defined( AUDIO_INTERFACE_USE_OPENSL_ES10 )
	#include "OpenSL10AudioInterface.hpp"
#endif // defined( AUDIO_INTERFACE_USE_MACROS )
#include "NullAudioInterface.hpp"


//-----------------------------------------------------------------------------------------------
STATIC AudioInterface* AudioInterface::s_activeAudioInterface = nullptr;



#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
STATIC void AudioInterface::Startup()
{
	FATAL_ASSERTION( s_activeAudioInterface == nullptr, "Audio Interface Error", "Cannot start up multiple Audio Interfaces!" );

#if defined( AUDIO_INTERFACE_USE_FMOD )
	s_activeAudioInterface = new FMODAudioInterface();
#elif defined( AUDIO_INTERFACE_USE_OPENAL )
	s_activeAudioInterface = new OpenALAudioInterface();
#elif defined( AUDIO_INTERFACE_USE_OPENSL_ES10 )
	s_activeAudioInterface = new OpenSL10AudioInterface();
#else
	s_activeAudioInterface = new NullAudioInterface();
#endif
	s_activeAudioInterface->PostStartup();
}

//-----------------------------------------------------------------------------------------------
STATIC void AudioInterface::Shutdown()
{
	FATAL_ASSERTION( s_activeAudioInterface != nullptr, "Audio Interface Error", "Cannot shut down an Audio Interface that hasn't yet been started!" );

	s_activeAudioInterface->PreShutdown();
	delete s_activeAudioInterface;
}
#pragma endregion //Lifecycle