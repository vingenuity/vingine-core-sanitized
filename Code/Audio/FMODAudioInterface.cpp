#include "FMODAudioInterface.hpp"

#include "../AssertionError.hpp"
#include "../AssetInterface.hpp"
#include "../StringConversion.hpp"


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void FMODAudioInterface::PostStartup()
{
	// Create the main system object.
	FMOD_RESULT result = FMOD::System_Create( &m_fmodSystem );
	VerifyFMODResultOrDie( result );

	// Initialize FMOD.
	result = m_fmodSystem->init( 512, FMOD_INIT_NORMAL, 0 );
	VerifyFMODResultOrDie( result );
}

//-----------------------------------------------------------------------------------------------
void FMODAudioInterface::DoUpdate( float /*deltaSeconds*/ )
{
	m_fmodSystem->update();
}

//-----------------------------------------------------------------------------------------------
void FMODAudioInterface::PreShutdown()
{
	m_fmodSystem->release();
	m_fmodSystem = nullptr; 
}
#pragma endregion //Lifecycle




#pragma region Public Interface
//-----------------------------------------------------------------------------------------------
AudioInterface::SoundID FMODAudioInterface::DoGetOrLoadSound( const std::string& soundFileLocation )
{
	std::map< std::string, SoundID >::const_iterator soundIDIterator = m_soundIDMapping.find( soundFileLocation );

	if( soundIDIterator != m_soundIDMapping.end() )
		return soundIDIterator->second;

	SoundID soundIDForNewSound = m_soundRegistry.size();

	FILE* soundFile = AssetInterface::OpenAssetAsFile( soundFileLocation.c_str(), "rb" );
	AssetInterface::SeekInAssetFile( soundFile, 0, SEEK_END );
	size_t soundFileSize = AssetInterface::GetCurrentPositionInAssetFile( soundFile );
	AssetInterface::SeekInAssetFile( soundFile, 0, SEEK_SET );

	char* soundData = new char[ soundFileSize ];
	AssetInterface::ReadFromAssetFile( soundData, sizeof( char ), soundFileSize, soundFile );
	AssetInterface::CloseAssetFile( soundFile );
	RECOVERABLE_ASSERTION( soundData != nullptr, "Sound Loading Error", "Unable to load a sound from file." );

	FMOD::Sound* newSound = nullptr;
	FMOD_CREATESOUNDEXINFO* soundBufferInfo = new FMOD_CREATESOUNDEXINFO();
	soundBufferInfo->cbsize = sizeof( FMOD_CREATESOUNDEXINFO );
	soundBufferInfo->length = soundFileSize;
	FMOD_RESULT soundLoadingResult = m_fmodSystem->createSound( soundData, FMOD_2D | FMOD_OPENMEMORY, soundBufferInfo, &newSound );
	VerifyFMODResultOrDie( soundLoadingResult );
	delete soundBufferInfo;
	delete soundData;

	m_soundRegistry.push_back( SoundData( newSound, nullptr ) );
	m_soundIDMapping[ soundFileLocation ] = soundIDForNewSound;
	return soundIDForNewSound;
}

//-----------------------------------------------------------------------------------------------
bool FMODAudioInterface::DoIsEmitterPaused( EmitterID emitter )
{
	bool emitterIsPaused = false;
	FMOD_RESULT boolResult = m_emitterRegistry[ emitter ]->getPaused( &emitterIsPaused );
	VerifyFMODResultOrDie( boolResult );
	return emitterIsPaused;
}

//-----------------------------------------------------------------------------------------------
bool FMODAudioInterface::DoIsEmitterPlaying( EmitterID emitter )
{
	bool emitterIsPlaying;
	FMOD_RESULT boolResult = m_emitterRegistry[ emitter ]->isPlaying( &emitterIsPlaying );
	VerifyFMODResultOrDie( boolResult );
	return emitterIsPlaying;
}

//-----------------------------------------------------------------------------------------------
void FMODAudioInterface::DoPauseEmitter( EmitterID emitter )
{
	FMOD_RESULT pauseResult = m_emitterRegistry[ emitter ]->setPaused( true );
	VerifyFMODResultOrDie( pauseResult );
}

//-----------------------------------------------------------------------------------------------
AudioInterface::EmitterID FMODAudioInterface::DoPlaySoundThroughEmitter( SoundID sound, EmitterID emitter, bool shouldLoop )
{
	static FMOD::ChannelGroup* NO_CHANNEL_GROUP = nullptr;

	FMOD_RESULT playResult;
	EmitterChannel* emitterChannel;

	if( emitter == ANY_EMITTER )
	{
		playResult = m_fmodSystem->playSound( m_soundRegistry[ sound ].fmodSound, NO_CHANNEL_GROUP, false, &emitterChannel );
		emitterChannel->getIndex( (int*)&emitter );
		++emitter; //our indices are 1 higher than FMOD's, since we have default 0.
	}
	else
	{
		FATAL_ASSERTION( emitter < m_emitterRegistry.size(), "FMOD Emitter Error", "Emitter number given is larger than the emitter registry's size!" );

		emitterChannel = m_emitterRegistry[ emitter ];
		playResult = m_fmodSystem->playSound( m_soundRegistry[ sound ].fmodSound, NO_CHANNEL_GROUP, false, &emitterChannel );
	}
	VerifyFMODResultOrDie( playResult );

	emitterChannel->setMode( ( shouldLoop * FMOD_LOOP_NORMAL ) + ( !shouldLoop * FMOD_LOOP_OFF ) );

	return emitter;
}

//-----------------------------------------------------------------------------------------------
void FMODAudioInterface::DoStopEmitter( EmitterID emitter )
{
	FMOD_RESULT stopResult = m_emitterRegistry[ emitter ]->stop();
	VerifyFMODResultOrDie( stopResult );
}

//-----------------------------------------------------------------------------------------------
void FMODAudioInterface::DoUnpauseEmitter( EmitterID emitter )
{
	FMOD_RESULT pauseResult = m_emitterRegistry[ emitter ]->setPaused( false );
	VerifyFMODResultOrDie( pauseResult );
}
#pragma endregion //Public Interface




//-----------------------------------------------------------------------------------------------
STATIC void FMODAudioInterface::VerifyFMODResultOrDie( FMOD_RESULT result )
{
	if( result != FMOD_OK )
	{
		std::string errorMessage( "An error has occurred with FMOD.\n" );
		errorMessage.append( "Error number: " );
		errorMessage.append( ConvertIntegerToString( result ) );
		errorMessage.append( "\nError string: " );
		errorMessage.append( FMOD_ErrorString( result ) );

		FATAL_ERROR( "FMOD Audio Interface Error", errorMessage );
	}
}
