#pragma once
#ifndef INCLUDED_STANDARD_ASSET_INTERFACE_HPP
#define INCLUDED_STANDARD_ASSET_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include "EngineMacros.hpp"

#include <cstdio>
#include <errno.h>
#if defined( PLATFORM_WINDOWS )
	#include <io.h>
	#define PLATFORM_SUPPORTS_FILE_DESCRIPTORS
#elif defined( PLATFORM_VITA ) || defined( PLATFORM_HTML5 ) || defined( PLATFORM_PS3 )
	//No includes
#else
	#include <fcntl.h>
	#include <unistd.h>
	#define PLATFORM_SUPPORTS_FILE_DESCRIPTORS
#endif

#include "AssetInterface.hpp"

//Ignore the warnings about the standard read/write/open functions being unsafe; we know!
#pragma warning( disable : 4996 ) 


//-----------------------------------------------------------------------------------------------
class StandardAssetInterface : public AssetInterface
{
public:
	StandardAssetInterface() { }
	~StandardAssetInterface() { }

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
};



#pragma region Internal File Interface
//-----------------------------------------------------------------------------------------------
inline FILE* StandardAssetInterface::DoOpenAssetAsFile( const char* filename, const char* mode )
{
	return fopen( filename, mode );
}

//-----------------------------------------------------------------------------------------------
inline size_t StandardAssetInterface::DoReadFromAssetFile( void* buffer, size_t bytesPerRead, size_t numReads, FILE* asset )
{
	return fread( buffer, bytesPerRead, numReads, asset );
}

//-----------------------------------------------------------------------------------------------
inline size_t StandardAssetInterface::DoWriteToAssetFile( const void* buffer, size_t bytesPerWrite, size_t numWrites, FILE* asset )
{
	return fwrite( buffer, bytesPerWrite, numWrites, asset );
}

//-----------------------------------------------------------------------------------------------
inline long int StandardAssetInterface::DoGetCurrentPositionInAssetFile( FILE* asset )
{
	return ftell( asset );
}

//-----------------------------------------------------------------------------------------------
inline int StandardAssetInterface::DoSeekInAssetFile( FILE* asset, long int offsetFromOriginBytes, int origin )
{
	return fseek( asset, offsetFromOriginBytes, origin );
}

//-----------------------------------------------------------------------------------------------
inline int StandardAssetInterface::DoCloseAssetFile( FILE* asset )
{
	return fclose( asset );
}
//-----------------------------------------------------------------------------------------------
inline int StandardAssetInterface::DoGetErrorCode()
{
	return errno;
}

//-----------------------------------------------------------------------------------------------
inline char* StandardAssetInterface::DoGetErrorString()
{
	return strerror( errno );
}
#pragma endregion //Internal File Interface



#pragma region Internal File Descriptor Interface

#if defined( PLATFORM_SUPPORTS_FILE_DESCRIPTORS )
//-----------------------------------------------------------------------------------------------
inline int StandardAssetInterface::DoOpenAssetAsDescriptor( const char* filename, int accessMode, long* out_startOffset, long* out_sizeBytes )
{
	int fileDescriptor = open( filename, accessMode );
	*out_startOffset = 0;

	*out_sizeBytes = lseek( fileDescriptor, 0, SEEK_END );
	lseek( fileDescriptor, 0, SEEK_SET );

	return fileDescriptor;
}

//-----------------------------------------------------------------------------------------------
inline size_t StandardAssetInterface::DoReadFromAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToRead )
{
	return read( assetDescriptor, buffer, numBytesToRead );
}

//-----------------------------------------------------------------------------------------------
inline size_t StandardAssetInterface::DoWriteToAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToWrite )
{
	return write( assetDescriptor, buffer, numBytesToWrite );
}

//-----------------------------------------------------------------------------------------------
inline int StandardAssetInterface::DoCloseAssetDescriptor( int assetDescriptor )
{
	return close( assetDescriptor );
}

#else 
//-----------------------------------------------------------------------------------------------
inline int StandardAssetInterface::DoOpenAssetAsDescriptor( const char* /*filename*/, int /*accessMode*/, long* /*out_startOffset*/, long* /*out_sizeBytes*/ )
{
	FATAL_ERROR( "Asset Interface Error", "This platform does not support the use of file descriptors.\nUse FILE pointers instead.\n" );
	return 0;
}

//-----------------------------------------------------------------------------------------------
inline size_t StandardAssetInterface::DoReadFromAssetDescriptor( int /*assetDescriptor*/, void* /*buffer*/, size_t /*numBytesToRead*/ )
{
	FATAL_ERROR( "Asset Interface Error", "This platform does not support the use of file descriptors.\nUse FILE pointers instead.\n" );
	return 0;
}

//-----------------------------------------------------------------------------------------------
inline size_t StandardAssetInterface::DoWriteToAssetDescriptor( int /*assetDescriptor*/, void* /*buffer*/, size_t /*numBytesToWrite*/ )
{
	FATAL_ERROR( "Asset Interface Error", "This platform does not support the use of file descriptors.\nUse FILE pointers instead.\n" );
	return 0;
}

//-----------------------------------------------------------------------------------------------
inline int StandardAssetInterface::DoCloseAssetDescriptor( int /*assetDescriptor*/ )
{
	FATAL_ERROR( "Asset Interface Error", "This platform does not support the use of file descriptors.\nUse FILE pointers instead.\n" );
	return 0;
}
#endif // defined( PLATFORM_SUPPORTS_FILE_DESCRIPTORS )

#pragma endregion //Internal File Descriptor Interface

#endif //INCLUDED_STANDARD_ASSET_INTERFACE_HPP
