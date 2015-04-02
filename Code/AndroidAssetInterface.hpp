#pragma once
#ifndef INCLUDED_ANDROID_ASSET_INTERFACE_HPP
#define INCLUDED_ANDROID_ASSET_INTERFACE_HPP

#include "EngineMacros.hpp"

#if defined( PLATFORM_ANDROID )

//-----------------------------------------------------------------------------------------------
#include <cstdio>
#include <errno.h>
#include <fcntl.h>

#include "AssetInterface.hpp"
#include <android/asset_manager.h>


//-----------------------------------------------------------------------------------------------
class AndroidAssetInterface : public AssetInterface
{
public:
	AndroidAssetInterface( void* assetManagerAsVoid );
	~AndroidAssetInterface() { }

	//Internal File Interface
	FILE* DoOpenAssetAsFile( const char* filename, const char* mode );
	size_t DoReadFromAssetFile( void* buffer, size_t bytesPerRead, size_t numReads, FILE* asset );
	size_t DoWriteToAssetFile( const void* buffer, size_t bytesPerWrite, size_t numWrites, FILE* asset );
	long int DoGetCurrentPositionInAssetFile( FILE* asset );
	int DoSeekInAssetFile( FILE* asset, long int offsetFromOriginBytes, int origin );
	int DoCloseAssetFile( FILE* asset );
	int DoGetErrorCode();
	char* DoGetErrorString();

	//Internal File Descriptor Interface
	int DoOpenAssetAsDescriptor( const char* filename, int accessMode = 0, long* out_startOffset = nullptr, long* out_sizeBytes = nullptr );
	size_t DoReadFromAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToRead );
	size_t DoWriteToAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToWrite );
	int DoCloseAssetDescriptor( int assetDescriptor );

private:
	// Overridden functions for use with funopen()
	static int android_read( void* cookie, char* buffer, int bufferSize );
	static int android_write( void* cookie, const char* buffer, int bufferSize );
	static fpos_t android_seek( void* cookie, long int offsetFromOriginBytes, int origin );
	static int android_close( void* cookie );

	//Data Members
	AAssetManager* m_assetManager;
};



#pragma region Internal File Interface
//-----------------------------------------------------------------------------------------------
inline AndroidAssetInterface::AndroidAssetInterface( void* assetManagerAsVoid )
	: m_assetManager( static_cast< AAssetManager* >( assetManagerAsVoid ) )
{

}

//-----------------------------------------------------------------------------------------------
inline FILE* AndroidAssetInterface::DoOpenAssetAsFile( const char* filename, const char* mode )
{
	if( mode[0] == 'w' ) return nullptr;

	AAsset* asset = AAssetManager_open( m_assetManager, filename, AASSET_MODE_UNKNOWN );
	if( !asset ) return nullptr;

	return funopen( asset, android_read, android_write, android_seek, android_close );
}

//-----------------------------------------------------------------------------------------------
inline size_t AndroidAssetInterface::DoReadFromAssetFile( void* buffer, size_t bytesPerRead, size_t numReads, FILE* asset )
{
	return fread( buffer, bytesPerRead, numReads, asset );
}

//-----------------------------------------------------------------------------------------------
inline size_t AndroidAssetInterface::DoWriteToAssetFile( const void* buffer, size_t bytesPerWrite, size_t numWrites, FILE* asset )
{
	return fwrite( buffer, bytesPerWrite, numWrites, asset );
}

//-----------------------------------------------------------------------------------------------
inline long int AndroidAssetInterface::DoGetCurrentPositionInAssetFile( FILE* asset )
{
	return ftell( asset );
}

//-----------------------------------------------------------------------------------------------
inline int AndroidAssetInterface::DoSeekInAssetFile( FILE* asset, long int offsetFromOriginBytes, int origin )
{
	return fseek( asset, offsetFromOriginBytes, origin );
}

//-----------------------------------------------------------------------------------------------
inline int AndroidAssetInterface::DoCloseAssetFile( FILE* asset )
{
	return fclose( asset );
}
//-----------------------------------------------------------------------------------------------
inline int AndroidAssetInterface::DoGetErrorCode()
{
	return errno;
}

//-----------------------------------------------------------------------------------------------
inline char* AndroidAssetInterface::DoGetErrorString()
{
	return strerror( errno );
}
#pragma endregion //Internal File Interface



#pragma region Internal File Descriptor Interface
//-----------------------------------------------------------------------------------------------
inline int AndroidAssetInterface::DoOpenAssetAsDescriptor( const char* filename, int accessMode, long* out_startOffset, long* out_sizeBytes )
{
	AAsset* asset = AAssetManager_open( m_assetManager, filename, AASSET_MODE_UNKNOWN );
	if( !asset ) return -1;

	int assetDescriptor = AAsset_openFileDescriptor( asset, out_startOffset, out_sizeBytes );
	AAsset_close( asset );

	return assetDescriptor;
}

//-----------------------------------------------------------------------------------------------
inline size_t AndroidAssetInterface::DoReadFromAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToRead )
{
	return read( assetDescriptor, buffer, numBytesToRead );
}

//-----------------------------------------------------------------------------------------------
inline size_t AndroidAssetInterface::DoWriteToAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToWrite )
{
	return write( assetDescriptor, buffer, numBytesToWrite );
}

//-----------------------------------------------------------------------------------------------
inline int AndroidAssetInterface::DoCloseAssetDescriptor( int assetDescriptor )
{
	return close( assetDescriptor );
}
#pragma endregion //Internal File Descriptor Interface



#pragma region funopen Functions
//-----------------------------------------------------------------------------------------------
STATIC int AndroidAssetInterface::android_read( void* cookie, char* buffer, int bufferSize )
{
	return AAsset_read( (AAsset*)cookie, buffer, bufferSize );
}

//-----------------------------------------------------------------------------------------------
STATIC int AndroidAssetInterface::android_write( void* cookie, const char* buffer, int bufferSize )
{
	return EACCES; // No writing into the apk
}

//-----------------------------------------------------------------------------------------------
STATIC fpos_t AndroidAssetInterface::android_seek( void* cookie, long int offsetFromOriginBytes, int origin )
{
	return AAsset_seek( (AAsset*)cookie, offsetFromOriginBytes, origin );
}

//-----------------------------------------------------------------------------------------------
STATIC int AndroidAssetInterface::android_close( void* cookie )
{
	AAsset_close( (AAsset*)cookie );
	return 0;
}
#pragma endregion //funopen Functions

#endif //defined( PLATFORM_ANDROID )

#endif //INCLUDED_ANDROID_ASSET_INTERFACE_HPP