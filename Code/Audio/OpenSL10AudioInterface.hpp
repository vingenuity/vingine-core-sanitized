#pragma once
#ifndef INCLUDED_OPENSL_10_AUDIO_INTERFACE_HPP
#define INCLUDED_OPENSL_10_AUDIO_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include "../EngineMacros.hpp"

#include <map>
#include <vector>

#include <SLES/OpenSLES.h>
#if defined( PLATFORM_ANDROID )
	#include <SLES/OpenSLES_Android.h>
	#include <SLES/OpenSLES_AndroidConfiguration.h>
#endif // defined( PLATFORM_ANDROID )

#include "../AssertionError.hpp"
#include "../AssetInterface.hpp"
#include "../DebuggerInterface.hpp"
#include "../StringConversion.hpp"
#include "AudioInterface.hpp"


//-----------------------------------------------------------------------------------------------
const char* GetSLErrorStringForResult( SLresult result )
{
	switch( result )
	{
	case SL_RESULT_SUCCESS:
		return "Operation was successful.";
	case SL_RESULT_PRECONDITIONS_VIOLATED:
		return "The preconditions for this function have been violated. Check the OpenSL documentation for information on these preconditions.";
	case SL_RESULT_PARAMETER_INVALID:
		return "The parameters given to this function are invalid.";
	case SL_RESULT_MEMORY_FAILURE:
		return "This function was unable to allocate or release memory.";
	case SL_RESULT_RESOURCE_ERROR:
		return "There were not enough resources to complete this operation.";
	case SL_RESULT_RESOURCE_LOST:
		return "The operation was ignored because the object is in unrealized or suspended state.";
	case SL_RESULT_IO_ERROR:
		return "The operation failed due to an I/O error.";
	case SL_RESULT_BUFFER_INSUFFICIENT:
		return "A buffer passed to this function is too small to service this function.";
	case SL_RESULT_CONTENT_CORRUPTED:
		return "The operation failed because the content was corrupted.";
	case SL_RESULT_CONTENT_UNSUPPORTED:
		return "The operation failed because this content format was unsupported.";
	case SL_RESULT_CONTENT_NOT_FOUND:
		return "The content requested was not found.";
	case SL_RESULT_PERMISSION_DENIED:
		return "The operation failed because this program did not have the correct permissions.";
	case SL_RESULT_FEATURE_UNSUPPORTED:
		return "The operation failed because this feature is not supported.";
	case SL_RESULT_INTERNAL_ERROR:
		return "The operation failed because an unrecoverable internal error has occurred.";
	case SL_RESULT_OPERATION_ABORTED:
		return "The operation was aborted due to the user's request.";
	case SL_RESULT_CONTROL_LOST:
		return "The operation failed because another entity is currently controlling the interface.";
	case 0x00000011: //SL_RESULT_READONLY:
		return "The operation failed because one or more parameters are read-only.";
	case 0x00000012: //SL_RESULT_ENGINEOPTION_UNSUPPORTED
		return "The operation failed due to an unsupported engine option.";
	case 0x00000013: //SL_RESULT_SOURCE_SINK_INCOMPATIBLE
		return "The source and sink being connected are incompatible.";
	case SL_RESULT_UNKNOWN_ERROR:
	default:
		return "An unknown error has occurred.";
	}
}

//-----------------------------------------------------------------------------------------------
struct EmitterObject
{
	EmitterObject()
		: playerObject( nullptr )
		, playInterface( nullptr )
		, seekInterface( nullptr )
		, attachedSound( 0 )
	{ }

	//Data Members
	SLObjectItf playerObject;
	SLPlayItf playInterface;
	SLSeekItf seekInterface;
	AudioInterface::SoundID attachedSound;
};

//-----------------------------------------------------------------------------------------------
struct SoundBuffer
{
	SoundBuffer()
		: fileDescriptor( -1 )
		, startOffset( 0 )
		, sizeBytes( 0 )
	{ }

	//Data Members
	int fileDescriptor;
	long startOffset;
	long sizeBytes;
};

//-----------------------------------------------------------------------------------------------
SINGLETON class OpenSL10AudioInterface : public AudioInterface
{
	friend class AudioInterface;

	static const bool BLOCK_UNTIL_OPERATION_COMPLETE = false;


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
	OpenSL10AudioInterface();
	~OpenSL10AudioInterface();

	//Helper Functions
	void CleanUpEmitter( EmitterObject emitter );
	static void VerifySLResultOrDie( SLresult result );

	//Data Members
	SLEngineItf m_audioEngine;
	SLObjectItf m_audioEngineObject;
	SLObjectItf m_outputMixObject;

	std::vector< EmitterObject > m_emitterRegistry;
	//std::vector< ListenerChannel* > m_listenerRegistry;

	std::map< std::string, SoundID > m_soundIDMapping;
	std::vector< SoundBuffer > m_soundRegistry;
};



//-----------------------------------------------------------------------------------------------
inline OpenSL10AudioInterface::OpenSL10AudioInterface()
	: AudioInterface()
	, m_audioEngine( nullptr )
	, m_audioEngineObject( nullptr )
	, m_outputMixObject( nullptr )
{ }

//-----------------------------------------------------------------------------------------------
inline OpenSL10AudioInterface::~OpenSL10AudioInterface()
{ }



//-----------------------------------------------------------------------------------------------
inline void OpenSL10AudioInterface::CleanUpEmitter( EmitterObject emitter )
{
	SLresult operationResult = SL_RESULT_SUCCESS;
	operationResult = (*emitter.playInterface)->SetPlayState( emitter.playInterface, SL_PLAYSTATE_STOPPED );
	VerifySLResultOrDie( operationResult );
	(*emitter.playerObject)->Destroy( emitter.playerObject );
}

//-----------------------------------------------------------------------------------------------
STATIC inline void OpenSL10AudioInterface::VerifySLResultOrDie( SLresult result )
{
	if( result != SL_RESULT_SUCCESS )
	{
		std::string errorMessage( "An error has occurred with FMOD.\n" );
		errorMessage.append( "Error number: " );
		errorMessage.append( ConvertIntegerToString( result ) );
		errorMessage.append( "\nError string: " );
		errorMessage.append( GetSLErrorStringForResult( result ) );

		FATAL_ERROR( "FMOD Audio Interface Error", errorMessage );
	}
}



#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
inline void OpenSL10AudioInterface::PostStartup()
{
	SLresult operationResult = SL_RESULT_SUCCESS;

	// Declare engine object, from which all other objects will be created.
	static const SLuint32 NO_OPTIONAL_FEATURES = 0;
	const SLuint32 numEngineInterfacesToSupport = 1;
	const SLInterfaceID engineInterfaceIDs[] = { SL_IID_ENGINE };
	const SLboolean areEngineInterfacesRequired[] = { SL_BOOLEAN_TRUE };
	operationResult = slCreateEngine( &m_audioEngineObject, NO_OPTIONAL_FEATURES, nullptr,
								 numEngineInterfacesToSupport, engineInterfaceIDs, areEngineInterfacesRequired );
	VerifySLResultOrDie( operationResult );

	// Realize (initialize) the engine object
	operationResult = (*m_audioEngineObject)->Realize( m_audioEngineObject, BLOCK_UNTIL_OPERATION_COMPLETE );
	VerifySLResultOrDie( operationResult );

	// Extract the engine interface from the engine object.
	operationResult = (*m_audioEngineObject)->GetInterface( m_audioEngineObject, SL_IID_ENGINE, &m_audioEngine );
	VerifySLResultOrDie( operationResult );

	// Declare our output mix (the output device(s) our audio will play from)
	const SLuint32 numOutputMixesNeeded = 0;
	const SLInterfaceID outputMixInterfaceIDs[] = { };
	const SLboolean areOutputMixesRequired[] = { };
	operationResult = (*m_audioEngine)->CreateOutputMix( m_audioEngine, &m_outputMixObject,
					numOutputMixesNeeded, outputMixInterfaceIDs, areOutputMixesRequired );
	VerifySLResultOrDie( operationResult );

	//  Realize (initialize) the output mix object
	operationResult = (*m_outputMixObject)->Realize( m_outputMixObject, BLOCK_UNTIL_OPERATION_COMPLETE );
	VerifySLResultOrDie( operationResult );

}

//-----------------------------------------------------------------------------------------------
inline void OpenSL10AudioInterface::PreShutdown()
{
	for( unsigned int emitterID = 0; emitterID < m_emitterRegistry.size(); ++emitterID )
	{
		EmitterObject& activeEmitter = m_emitterRegistry[ emitterID ];

		CleanUpEmitter( activeEmitter );
		m_emitterRegistry.erase( m_emitterRegistry.begin() + emitterID );
		--emitterID;
	}

	if( m_outputMixObject != nullptr )
	{
		(*m_outputMixObject)->Destroy( m_outputMixObject );
		m_outputMixObject = nullptr;
	}

	if( m_audioEngineObject != nullptr )
	{
		(*m_audioEngineObject)->Destroy( m_audioEngineObject );
		m_audioEngineObject = nullptr;
	}
}
#pragma endregion //Lifecycle



#pragma region Public Interface
//-----------------------------------------------------------------------------------------------
inline AudioInterface::SoundID OpenSL10AudioInterface::DoGetOrLoadSound( const std::string& soundFileLocation )
{
	// If we've already loaded this sound, just pass it on.
	std::map< std::string, SoundID >::iterator soundIterator;
	soundIterator = m_soundIDMapping.find( soundFileLocation );
	if( soundIterator != m_soundIDMapping.end() )
		return soundIterator->second;

	// Otherwise load it into a buffer
	SoundBuffer soundBuffer;
	soundBuffer.fileDescriptor = AssetInterface::OpenAssetAsDescriptor( soundFileLocation.c_str(), 0,
		&soundBuffer.startOffset, &soundBuffer.sizeBytes );
	if( soundBuffer.fileDescriptor < 0 )
	{
		std::string errorMessage( "The audio interface was unable to load the sound located at '" );
		errorMessage.append( soundFileLocation );
		errorMessage.append( "'.\n Error Code: " );
		errorMessage.append( ConvertIntegerToString( AssetInterface::GetErrorCode() ) );
		errorMessage.append( "\n Error Text: " );
		errorMessage.append( AssetInterface::GetErrorString() );

		FATAL_ERROR( "Audio Interface Error", errorMessage );
	}

	// And hold onto the buffer to give it to the sound library later.
	SoundID newSoundID = m_soundRegistry.size();
	m_soundRegistry.push_back( soundBuffer );
	m_soundIDMapping[ soundFileLocation ] = newSoundID;
	return newSoundID;
}

//-----------------------------------------------------------------------------------------------
inline bool OpenSL10AudioInterface::DoIsEmitterPaused( EmitterID emitter )
{
	SLuint32 currentPlayState = 0;

	EmitterObject& emitterObject = m_emitterRegistry[ emitter ];
	SLresult operationResult = (*emitterObject.playInterface)->GetPlayState( emitterObject.playInterface, &currentPlayState );
	VerifySLResultOrDie( operationResult );

	return ( currentPlayState == SL_PLAYSTATE_PAUSED );
}

//-----------------------------------------------------------------------------------------------
inline bool OpenSL10AudioInterface::DoIsEmitterPlaying( EmitterID emitter )
{
	SLuint32 currentPlayState = 0;

	EmitterObject& emitterObject = m_emitterRegistry[ emitter ];
	SLresult operationResult = (*emitterObject.playInterface)->GetPlayState( emitterObject.playInterface, &currentPlayState );
	VerifySLResultOrDie( operationResult );

	return ( currentPlayState == SL_PLAYSTATE_PLAYING );
}

//-----------------------------------------------------------------------------------------------
inline void OpenSL10AudioInterface::DoPauseEmitter( EmitterID emitter )
{
	EmitterObject& emitterObject = m_emitterRegistry[ emitter ];
	SLresult operationResult = (*emitterObject.playInterface)->SetPlayState( emitterObject.playInterface, SL_PLAYSTATE_PAUSED );
	VerifySLResultOrDie( operationResult );
}

//-----------------------------------------------------------------------------------------------
inline AudioInterface::EmitterID OpenSL10AudioInterface::DoPlaySoundThroughEmitter( SoundID sound, EmitterID emitter, bool shouldLoop )
{
// 	//If an emitter ID was given, use that one
// 	if( emitter != ANY_EMITTER )
// 	{
// 		DoStopEmitter( emitter );
// 		DoUnpauseEmitter( emitter );
// 		return emitter;
// 	}
// 
// 	// Otherwise, find an open emitter ( one that isn't playing right now and has our sound )
// 	// This is necessary because OpenSL sound objects can't be made to play more than their original sound.
// 	for( unsigned int emitterID = 0; emitterID < m_emitterRegistry.size(); ++emitterID )
// 	{
// 		EmitterObject& activeEmitter = m_emitterRegistry[ emitterID ];
// 
// 		if( ( activeEmitter.attachedSound == sound ) && DoIsEmitterPaused( emitterID ) )
// 		{
// 			WriteToDebuggerOutput( "Reusing old emitter." );
// 			DoStopEmitter( emitter );
// 			DoUnpauseEmitter( emitter );
// 			return emitterID;
// 		}
// 	}
	// WORKAROUND: For the moment, reusing audio engines causes them to repeatedly play whatever the last sound was, regardless of their original attached sound.
	// So, we are going to always create a new audio engine and kill off the old ones in this call.
	for( unsigned int emitterID = 0; emitterID < m_emitterRegistry.size(); ++emitterID )
	{
		EmitterObject& activeEmitter = m_emitterRegistry[ emitterID ];

		if( DoIsEmitterPaused( emitterID ) )
		{
			CleanUpEmitter( activeEmitter );
			m_emitterRegistry.erase( m_emitterRegistry.begin() + emitterID );
			--emitterID;
		}
	}

	// Otherwise, make a new emitter
	SLresult operationResult = SL_RESULT_SUCCESS;
	EmitterObject newEmitter;
	SoundBuffer& requestedSound = m_soundRegistry[ sound ];

	// Set up the audio parameters for this sound
	SLDataLocator_AndroidFD descriptorLocator = { SL_DATALOCATOR_ANDROIDFD, requestedSound.fileDescriptor, 
		requestedSound.startOffset, requestedSound.sizeBytes };
	SLDataFormat_MIME mimeFormat = { SL_DATAFORMAT_MIME, nullptr, SL_CONTAINERTYPE_UNSPECIFIED };
	SLDataSource audioSource = { &descriptorLocator, &mimeFormat };

	// Set up the "sink" for this sound (where it will be output)
	SLDataLocator_OutputMix outputLocator = { SL_DATALOCATOR_OUTPUTMIX, m_outputMixObject };
	SLDataSink audioSink = { &outputLocator,  nullptr };

	// Create our audio player
	const SLuint32 numPlayerInterfacesToSupport = 2;
	const SLInterfaceID playerInterfaceIDsToSupport[] = { SL_IID_PLAY, SL_IID_SEEK };
	const SLboolean arePlayerInterfacesRequired[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
	operationResult = (*m_audioEngine)->CreateAudioPlayer( m_audioEngine, &newEmitter.playerObject, &audioSource,
		&audioSink, numPlayerInterfacesToSupport, playerInterfaceIDsToSupport, arePlayerInterfacesRequired );
	VerifySLResultOrDie( operationResult );

	// Realize the player
	operationResult = (*newEmitter.playerObject)->Realize( newEmitter.playerObject, BLOCK_UNTIL_OPERATION_COMPLETE );
	// this will always succeed on Android, but we check result for portability to other platforms
	VerifySLResultOrDie( operationResult );

	// Get the seek interface
	operationResult = (*newEmitter.playerObject)->GetInterface( newEmitter.playerObject, SL_IID_SEEK, &newEmitter.seekInterface );
	VerifySLResultOrDie( operationResult );

	// Set the loop state (full loops using 0 to SL_TIME_UNKNOWN is the only working option on Android right now)
	operationResult = (*newEmitter.seekInterface)->SetLoop( newEmitter.seekInterface, shouldLoop, 0, SL_TIME_UNKNOWN );
	VerifySLResultOrDie( operationResult );

	// Get the play interface
	operationResult = (*newEmitter.playerObject)->GetInterface( newEmitter.playerObject, SL_IID_PLAY, &newEmitter.playInterface );
	VerifySLResultOrDie( operationResult );

	// Play the sound
	operationResult = (*newEmitter.playInterface)->SetPlayState( newEmitter.playInterface, SL_PLAYSTATE_PLAYING );
	VerifySLResultOrDie( operationResult );

	newEmitter.attachedSound = sound;
	m_emitterRegistry.push_back( newEmitter );
	return ANY_EMITTER;
}

//-----------------------------------------------------------------------------------------------
inline void OpenSL10AudioInterface::DoStopEmitter( EmitterID emitter )
{
	EmitterObject& emitterObject = m_emitterRegistry[ emitter ];
	SLresult operationResult = (*emitterObject.playInterface)->SetPlayState( emitterObject.playInterface, SL_PLAYSTATE_STOPPED );
	VerifySLResultOrDie( operationResult );
}

//-----------------------------------------------------------------------------------------------
inline void OpenSL10AudioInterface::DoUnpauseEmitter( EmitterID emitter )
{
	EmitterObject& emitterObject = m_emitterRegistry[ emitter ];
	SLresult operationResult = (*emitterObject.playInterface)->SetPlayState( emitterObject.playInterface, SL_PLAYSTATE_PLAYING );
	VerifySLResultOrDie( operationResult );
}
#pragma endregion //Public Interface

#endif //INCLUDED_OPENSL_10_AUDIO_INTERFACE_HPP