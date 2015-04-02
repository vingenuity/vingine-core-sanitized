#pragma once
#ifndef INCLUDED_ASSET_INTERFACE_HPP
#define INCLUDED_ASSET_INTERFACE_HPP

//-----------------------------------------------------------------------------------------------
#include "EngineMacros.hpp"

#include <stdio.h>
//#include <fcntl.h>
#include <string>


//-----------------------------------------------------------------------------------------------
/************************************************************************************************
	The purpose of this global interface is to abstract the platform-dependent loading of assets.
Most platforms need nothing more than fopen/fclose. However, some platforms automatically
compress assets or store them in specific directories.
This class is made to abstract that loading difficulty.
************************************************************************************************/
ABSTRACT SINGLETON class AssetInterface
{
public:
	//Lifecycle
	static void Startup( void* extraData = nullptr );
	static void Shutdown();

	//Public File Interface
	static FILE* OpenAssetAsFile( const char* filename, const char* mode );
	static size_t ReadFromAssetFile( void* buffer, size_t bytesPerRead, size_t numReads, FILE* assetFile );
	static size_t WriteToAssetFile( const void* buffer, size_t bytesPerWrite, size_t numWrites, FILE* assetFile );
	static long int GetCurrentPositionInAssetFile( FILE* assetFile );
	static int SeekInAssetFile( FILE* assetFile, long int offsetFromOriginBytes, int origin );
	static int CloseAssetFile( FILE* assetFile );

	//Public File Descriptor Interface
	static int OpenAssetAsDescriptor( const char* filename, int accessMode = 0, long* out_startOffset = nullptr, long* out_sizeBytes = nullptr );
	static size_t ReadFromAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToRead );
	static size_t WriteToAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToWrite );
	static int CloseAssetDescriptor( int assetDescriptor );

	//Public Helper Functions
	static int GetErrorCode() { return s_activeAssetInterface->DoGetErrorCode(); }
	static char* GetErrorString() { return s_activeAssetInterface->DoGetErrorString(); }


protected:
	AssetInterface() { }
	virtual ~AssetInterface() { }

	//Internal File Interface
	virtual FILE* DoOpenAssetAsFile( const char* filename, const char* mode ) = 0;
	virtual size_t DoReadFromAssetFile( void* buffer, size_t bytesPerRead, size_t numReads, FILE* asset ) = 0;
	virtual size_t DoWriteToAssetFile( const void* buffer, size_t bytesPerWrite, size_t numWrites, FILE* asset ) = 0;
	virtual long int DoGetCurrentPositionInAssetFile( FILE* asset ) = 0;
	virtual int DoSeekInAssetFile( FILE* asset, long int offsetFromOriginBytes, int origin ) = 0;
	virtual int DoCloseAssetFile( FILE* asset ) = 0;

	//Internal File Descriptor Interface
	virtual int DoOpenAssetAsDescriptor( const char* filename, int accessMode = 0, long* out_startOffset = nullptr, long* out_sizeBytes = nullptr ) = 0;
	virtual size_t DoReadFromAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToRead ) = 0;
	virtual size_t DoWriteToAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToWrite ) = 0;
	virtual int DoCloseAssetDescriptor( int assetDescriptor ) = 0;

	//Helper Functions
	virtual int DoGetErrorCode() = 0;
	virtual char* DoGetErrorString() = 0;


private:
	//We have no need of a pithy assignment or copy operator!
	AssetInterface( const AssetInterface& other );
	AssetInterface& operator=( const AssetInterface& other );

	//Only the interface has control over the internal pointer
	static AssetInterface* s_activeAssetInterface;
	static const char* s_rootAssetDirectory;
};



#pragma region Public File Interface
//-----------------------------------------------------------------------------------------------
inline STATIC FILE* AssetInterface::OpenAssetAsFile( const char* filename, const char* mode )
{
	std::string fileLocation( s_rootAssetDirectory );
	fileLocation.append( filename );

	return s_activeAssetInterface->DoOpenAssetAsFile( fileLocation.c_str(), mode );
}

//-----------------------------------------------------------------------------------------------
inline STATIC size_t AssetInterface::ReadFromAssetFile( void* buffer, size_t bytesPerRead, size_t numReads, FILE* asset )
{
	return s_activeAssetInterface->DoReadFromAssetFile( buffer, bytesPerRead, numReads, asset );
}

//-----------------------------------------------------------------------------------------------
inline STATIC size_t AssetInterface::WriteToAssetFile( const void* buffer, size_t bytesPerWrite, size_t numWrites, FILE* asset )
{
	return s_activeAssetInterface->DoWriteToAssetFile( buffer, bytesPerWrite, numWrites, asset );
}

//-----------------------------------------------------------------------------------------------
inline STATIC long int AssetInterface::GetCurrentPositionInAssetFile( FILE* asset )
{
	return s_activeAssetInterface->DoGetCurrentPositionInAssetFile( asset );
}

//-----------------------------------------------------------------------------------------------
inline STATIC int AssetInterface::SeekInAssetFile( FILE* asset, long int offsetFromOriginBytes, int origin )
{
	return s_activeAssetInterface->DoSeekInAssetFile( asset, offsetFromOriginBytes, origin );
}

//-----------------------------------------------------------------------------------------------
inline STATIC int AssetInterface::CloseAssetFile( FILE* asset )
{
	return s_activeAssetInterface->DoCloseAssetFile( asset );
}
#pragma endregion //Public File Interface



#pragma region Public File Descriptor Interface
//-----------------------------------------------------------------------------------------------
inline STATIC int AssetInterface::OpenAssetAsDescriptor( const char* filename, int accessMode, long* out_startOffset, long* out_sizeBytes )
{
	std::string fileLocation( s_rootAssetDirectory );
	fileLocation.append( filename );

	return s_activeAssetInterface->DoOpenAssetAsDescriptor( fileLocation.c_str(), accessMode, out_startOffset, out_sizeBytes );
}

//-----------------------------------------------------------------------------------------------
inline STATIC size_t AssetInterface::ReadFromAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToRead )
{
	return s_activeAssetInterface->DoReadFromAssetDescriptor( assetDescriptor, buffer, numBytesToRead );
}

//-----------------------------------------------------------------------------------------------
inline STATIC size_t AssetInterface::WriteToAssetDescriptor( int assetDescriptor, void* buffer, size_t numBytesToWrite )
{
	return s_activeAssetInterface->DoWriteToAssetDescriptor( assetDescriptor, buffer, numBytesToWrite );
}

//-----------------------------------------------------------------------------------------------
inline STATIC int AssetInterface::CloseAssetDescriptor( int assetDescriptor )
{
	return s_activeAssetInterface->DoCloseAssetDescriptor( assetDescriptor );
}

#pragma endregion //Public File Descriptor Interface

#endif //INCLUDED_ASSET_INTERFACE_HPP
