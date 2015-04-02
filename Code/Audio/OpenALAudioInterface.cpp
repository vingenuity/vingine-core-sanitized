#include "../BuildPreferences.hpp"

#if defined( AUDIO_INTERFACE_USE_OPENAL )

#include "OpenALAudioInterface.hpp"

#include <iostream>
#include "../AssertionError.hpp"
#include "../StringConversion.hpp"
#include "AudioFileLoader.hpp"


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void OpenALAudioInterface::PostStartup()
{
	m_oalDevice = alcOpenDevice( nullptr );
	m_oalContext = alcCreateContext( m_oalDevice, nullptr );
	alcMakeContextCurrent( m_oalContext );

	EmitterSourceID defaultEmitter = 0;
	alGenSources( 1, &defaultEmitter );
	alSourcef( defaultEmitter, AL_GAIN, 1 );
	alSourcef( defaultEmitter, AL_PITCH, 1 );
	alSourcei( defaultEmitter, AL_LOOPING, AL_FALSE );
	alSource3f( defaultEmitter, AL_POSITION, 0, 0, 0 );
	alSource3f( defaultEmitter, AL_VELOCITY, 0, 0, 0 );
	m_emitterRegistry.push_back( defaultEmitter );
}

//-----------------------------------------------------------------------------------------------
void OpenALAudioInterface::PreShutdown()
{
	for( unsigned int i = 0; i < m_emitterRegistry.size(); ++i )
	{
		AudioInterface::StopEmitter( i );
	}

	for( unsigned int i = 0; i < m_soundRegistry.size(); ++i )
	{
		alDeleteBuffers( 1, &m_soundRegistry[i] );
	}
	alcDestroyContext( m_oalContext );
	alcCloseDevice( m_oalDevice );
}
#pragma endregion //Lifecycle


#pragma region Public Interface
//-----------------------------------------------------------------------------------------------
AudioInterface::SoundID OpenALAudioInterface::DoGetOrLoadSound( const std::string& soundFileLocation )
{
	ALuint bufferID = 0;
	unsigned char* soundBuffer = nullptr;
	size_t bufferSize = 0;
	unsigned int soundSampleRate = 0;
	unsigned int numChannels = 0;
	unsigned int bitsPerSample = 0;
	LoadWAVOrDie( soundFileLocation.c_str(), soundBuffer, bufferSize, soundSampleRate, numChannels, bitsPerSample );

	//Now generate the buffers and load the audio into OpenAL
	alGenBuffers( 1, &bufferID );
	DieIfOpenALErrorDetected();

	ALenum soundFormat = ConvertSoundFormatToOpenALEnumOrDie( numChannels, bitsPerSample );
	alBufferData( bufferID, soundFormat, (void*)soundBuffer, bufferSize, soundSampleRate );
	DieIfOpenALErrorDetected();

	delete[] soundBuffer;

	m_soundRegistry.push_back( bufferID );
	m_soundIDMapping[ soundFileLocation ] = bufferID;
	return m_soundRegistry.size() - 1;
}

//-----------------------------------------------------------------------------------------------
bool OpenALAudioInterface::DoIsEmitterPaused( EmitterID emitter )
{
	ALint isPausedResult = 0;
	alGetSourcei( m_emitterRegistry[ emitter ], AL_PAUSED, &isPausedResult );
	return ( isPausedResult != 0 );
}

//-----------------------------------------------------------------------------------------------
bool OpenALAudioInterface::DoIsEmitterPlaying( EmitterID emitter )
{
	ALint isPlayingResult = 0;
	alGetSourcei( m_emitterRegistry[ emitter ], AL_PLAYING, &isPlayingResult );
	return ( isPlayingResult != 0 );
}

//-----------------------------------------------------------------------------------------------
void OpenALAudioInterface::DoPauseEmitter( EmitterID emitter )
{
	alSourcePause( m_emitterRegistry[ emitter ] );
}

//-----------------------------------------------------------------------------------------------
AudioInterface::EmitterID OpenALAudioInterface::DoPlaySoundThroughEmitter( SoundID sound, EmitterID emitter, bool shouldLoop )
{
	if( emitter == ANY_EMITTER )
	{
// 		for( unsigned int emitterID = 0; emitterID < m_emitterRegistry.size(); ++emitterID )
// 		{
// 			EmitterID& emitterInRegistry = m_emitterRegistry[ emitterID ];
// 			if( !IsEmitterPlaying( emitterID ) )
// 			{
// 				//find an open emitter
// 				alSourcei( emitterInRegistry, AL_BUFFER, m_soundRegistry[ sound ] );
// 				//alSourcei( emitterInRegistry, AL_LOOPING,  shouldLoop );
// 				alSourcePlay( emitterInRegistry );
// 				return emitterID;
// 			}
// 		}

		//If we got here, we didn't find an open emitter
		EmitterID newEmitter = 0;
		alGenSources( 1, &newEmitter );
		alSourcef( newEmitter, AL_GAIN, 1 );
		alSourcef( newEmitter, AL_PITCH, 1 );
		alSourcei( newEmitter, AL_LOOPING,  shouldLoop );
		alSourcei( newEmitter, AL_BUFFER, m_soundRegistry[ sound ] );
		alSource3f( newEmitter, AL_POSITION, 0, 0, 0 );
		alSource3f( newEmitter, AL_VELOCITY, 0, 0, 0 );
		alSourcePlay( newEmitter );
		m_emitterRegistry.push_back( newEmitter );

		return m_emitterRegistry.size() - 1;
	}
	else
	{
		alSourcei( m_emitterRegistry[ emitter ], AL_BUFFER, m_soundRegistry[ sound ] );
		alSourcei( m_emitterRegistry[ emitter ], AL_LOOPING,  shouldLoop );
		alSourcePlay( m_emitterRegistry[ emitter ] );
		return emitter;
	}
}

//-----------------------------------------------------------------------------------------------
void OpenALAudioInterface::DoStopEmitter( EmitterID emitter )
{
	alSourceStop(  m_emitterRegistry[ emitter ] );
}

//-----------------------------------------------------------------------------------------------
void OpenALAudioInterface::DoUnpauseEmitter( EmitterID emitter )
{
	alSourcePlay( m_emitterRegistry[ emitter ] );
}
#pragma endregion //Public Interface



#pragma region Helper Functions
//-----------------------------------------------------------------------------------------------
void OpenALAudioInterface::DieIfOpenALErrorDetected()
{
	ALenum alErrorState = alGetError();
	if( alErrorState != AL_NO_ERROR )
	{
		std::string errorMessage( "An error has occurred in openAL.\n" );
		errorMessage.append( "OpenAL Error Number: " );
		errorMessage.append( ConvertIntegerToString( alErrorState ) );
		errorMessage.append( "\nOpenAL Error String: " );
		errorMessage.append( alGetString( alErrorState ) );

		FATAL_ERROR( "OpenAL Audio Error", errorMessage );
	}
}

//-----------------------------------------------------------------------------------------------
ALenum OpenALAudioInterface::ConvertSoundFormatToOpenALEnumOrDie( unsigned int numChannels, unsigned int bitsPerSample )
{
	if ( numChannels == 1 )
	{
		switch( bitsPerSample )
		{
		case 8:
			return AL_FORMAT_MONO8;
		case 16:
			return AL_FORMAT_MONO16;
		default:
			break;
		}
	}
	else if ( numChannels == 2 )
	{
		switch( bitsPerSample )
		{
		case 8:
			return AL_FORMAT_STEREO8;
		case 16:
			return AL_FORMAT_STEREO16;
		default:
			break;
		}
	}

	//If we've made it here, OpenAL has no equivalent for this format
	std::string errorMessage( "OpenAL is unable to decode audio with this format.\n" );
	errorMessage.append( "Format of this file: " );
	errorMessage.append( ConvertIntegerToString( numChannels ) );
	errorMessage.append( " channels, with " );
	errorMessage.append( ConvertIntegerToString( bitsPerSample ) );
	errorMessage.append( " bits per sample." );

	FATAL_ERROR( "OpenAL Audio Error", errorMessage );
}
#pragma endregion //Helper Functions

#endif // defined( AUDIO_INTERFACE_USE_OPENAL )
