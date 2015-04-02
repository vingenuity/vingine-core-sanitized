#include "AudioFileLoader.hpp"

#include "../AssertionError.hpp"
#include "../AssetInterface.hpp"
#include "../StringConversion.hpp"

//-----------------------------------------------------------------------------------------------
struct FOURCC
{
	FOURCC() { } //Default initialization should be good enough

	FOURCC( const char* code )
	{
		FATAL_ASSERTION( strlen( code ) == 4, "FOURCC Error", 
			"Attempted to create a FOURCC with a code greater than four characters.\n" );

		// Definitely more efficient to do this without the for loop.
		charCode[0] = code[0];
		charCode[1] = code[1];
		charCode[2] = code[2];
		charCode[3] = code[3];
	}

	bool operator==( const FOURCC& rhs ) const
	{
		if( this->charCode[0] == rhs.charCode[0] &&
			this->charCode[1] == rhs.charCode[1] &&
			this->charCode[2] == rhs.charCode[2] &&
			this->charCode[3] == rhs.charCode[3] )
		{
			return true;
		}

		return false;
	}

	bool operator!=( const FOURCC& rhs ) const { return !( *this == rhs ); }


	//Data Member
	char charCode[4];
};


// Chunk identifying FOURCCs
static const FOURCC RIFF_HEADER_ID = "RIFF";
static const FOURCC WAVE_FORMAT_CHUNK_ID = "fmt ";
static const FOURCC WAVE_DATA_CHUNK_ID = "data";

static const FOURCC RIFF_TYPE_WAVE = "WAVE";


/* The WAV format defines a number of compression schemes.
 * Here is a list of most of the common schemes (even though I don't know how to decompress them.)*/
typedef unsigned short CompressionCode;
static const CompressionCode COMPRESSION_Unknown = 0;
static const CompressionCode COMPRESSION_UncompressedPCM = 1;
static const CompressionCode COMPRESSION_MicrosoftADPCM = 2;
static const CompressionCode COMPRESSION_ITUg711ALaw = 6;
static const CompressionCode COMPRESSION_ITUg711AuLaw = 7;
static const CompressionCode COMPRESSION_IMA_ADPCM = 17;
static const CompressionCode COMPRESSION_ITUg723ADPCM = 20;
static const CompressionCode COMPRESSION_GSM610 = 49;
static const CompressionCode COMPRESSION_ITUg721ADPCM = 64;
static const CompressionCode COMPRESSION_MPEG = 80;
static const CompressionCode COMPRESSION_Experimental = 65535;

#pragma region WAV File Chunks
/*This WAV file information courtesy of: http://www.sonicspot.com/guide/wavefiles.html */

/* This generic chunk header can be used to check what the next chunk is without reading the whole thing.*/
struct RIFFChunkHeader
{
	FOURCC chunkID;
	unsigned int chunkSize; //File size of the chunk NOT including the header.
};

/*The RIFF file header always kicks off the WAV file with the file size and type. */
struct RIFFFileHeader
{
	FOURCC chunkID;			//Should always be 'RIFF'.
	unsigned int chunkSize; //File size not including the 8 byte header
	FOURCC riffType;		//For WAV files, should always be 'WAVE'.
};


/* The remaining chunks can be in ANY order! 
 * Most WAV files have the format next, followed by data, but this is a convention, NOT a standard! */



/*The format chunk contains all the info needed to decode the sound data.*/
static const size_t SIZE_OF_WAVE_FORMAT_CHUNK_BYTES = 16;
struct WaveFormatChunk
{
	FOURCC chunkID; //Should always be 'fmt '. (Mind the space!)
	unsigned int chunkSize;
	CompressionCode compressionCode;
	unsigned short numChannels;
	unsigned int sampleRateHz;
	unsigned int avgBytesPerSecond;
	unsigned short blockAlignment;
	unsigned short bitsPerSample; //Typically 8, 16, 24, or 32.
};

/*The data chunk contains the information needed to load the chunk into a buffer.*/
struct WaveDataChunk
{
	FOURCC chunkID;		// Should always read 'data'.
	unsigned int chunkSize; //Stores the size of the data block
};
#pragma endregion //WAV File Chunks



//-----------------------------------------------------------------------------------------------
void VerifyRIFFFileHeaderOrDie( const char* filename, const RIFFFileHeader& fileHeader, const FOURCC& desiredRIFFType )
{
	if( fileHeader.chunkID != RIFF_HEADER_ID )
	{
		std::string errorMessage( "Unable to open RIFF file located at " );
		errorMessage.append( filename );
		errorMessage.append( ".\n The RIFF header appears to be corrupt." );
		FATAL_ERROR( "WAV File Loading Error", errorMessage );
	}
	if( fileHeader.riffType != desiredRIFFType )
	{
		std::string errorMessage( "Unable to open RIFF file located at " );
		errorMessage.append( filename );
		errorMessage.append( ".\n The file header indicates that this file does not contain WAVE audio data." );
		FATAL_ERROR( "WAV File Loading Error", errorMessage );
	}
}

//-----------------------------------------------------------------------------------------------
void ExtractWAVFormatFromFileOrDie( WaveFormatChunk& out_formatData, const char* filename, FILE* soundFile )
{
	AssetInterface::ReadFromAssetFile( &out_formatData, sizeof( WaveFormatChunk ), 1, soundFile );
	if( out_formatData.chunkID != WAVE_FORMAT_CHUNK_ID )
	{
		std::string errorMessage( "Unable to open WAV file located at " );
		errorMessage.append( filename );
		errorMessage.append( ".\n The WAV format chunk appears to be corrupt." );
		FATAL_ERROR( "WAV File Loading Error", errorMessage );
	}

	// Seek past the end of the format chunk (it can potentially be varying size due to extra format data)
	if ( out_formatData.chunkSize > SIZE_OF_WAVE_FORMAT_CHUNK_BYTES )
		AssetInterface::SeekInAssetFile( soundFile, out_formatData.chunkSize - SIZE_OF_WAVE_FORMAT_CHUNK_BYTES, SEEK_CUR );
}

void ExtractWAVDataOrDie( unsigned char*& out_buffer, size_t& out_bufferSize, 
							const char* filename, FILE* soundFile )
{
	WaveDataChunk wavData;

	AssetInterface::ReadFromAssetFile( &wavData, sizeof( WaveDataChunk ), 1, soundFile );
	if( wavData.chunkID != WAVE_DATA_CHUNK_ID )
	{
		if( soundFile != nullptr )
			AssetInterface::CloseAssetFile( soundFile );

		std::string errorMessage( "Unable to open WAV file located at " );
		errorMessage.append( filename );
		errorMessage.append( ".\n The WAV data chunk appears to be corrupt." );
		FATAL_ERROR( "WAV File Loading Error", errorMessage );
	}

	//Now, we can finally read in the actual audio data.
	out_bufferSize = wavData.chunkSize;
	out_buffer = new unsigned char[ wavData.chunkSize ];
	if( out_buffer == nullptr )
	{
		if( soundFile != nullptr )
			AssetInterface::CloseAssetFile( soundFile );

		std::string errorMessage( "Unable to open WAV file located at " );
		errorMessage.append( filename );
		errorMessage.append( ".\nUnable to allocate enough memory to load the file into memory." );
		FATAL_ERROR( "WAV File Loading Error", errorMessage );
	}

	size_t chunksRead = AssetInterface::ReadFromAssetFile( out_buffer, wavData.chunkSize, 1, soundFile );
	if( chunksRead != 1 )
	{
		if( soundFile != nullptr )
			AssetInterface::CloseAssetFile( soundFile );

		std::string errorMessage( "Unable to fully load the WAV file at " );
		errorMessage.append( filename );
		errorMessage.append( " into memory.\nLoaded " );
		errorMessage.append( ConvertIntegerToString( chunksRead * wavData.chunkSize ) );
		errorMessage.append( " bytes, where " );
		errorMessage.append( ConvertIntegerToString( wavData.chunkSize ) );
		errorMessage.append( " bytes were requested." );
		FATAL_ERROR( "WAV File Loading Error", errorMessage );
	}
}

//-----------------------------------------------------------------------------------------------
void LoadWAVOrDie( const char* filename, unsigned char*& out_buffer,
	size_t& out_bufferSize, unsigned int& out_sampleRateHertz,
	unsigned int& out_numChannels, unsigned int& out_bitsPerSample )
{
#pragma region Unused Compression Variables (stifles warnings on some compilers)
	VARIABLE_IS_UNUSED( COMPRESSION_Unknown );
	VARIABLE_IS_UNUSED( COMPRESSION_MicrosoftADPCM );
	VARIABLE_IS_UNUSED( COMPRESSION_ITUg711ALaw );
	VARIABLE_IS_UNUSED( COMPRESSION_ITUg711AuLaw );
	VARIABLE_IS_UNUSED( COMPRESSION_IMA_ADPCM );
	VARIABLE_IS_UNUSED( COMPRESSION_ITUg723ADPCM );
	VARIABLE_IS_UNUSED( COMPRESSION_GSM610 );
	VARIABLE_IS_UNUSED( COMPRESSION_ITUg721ADPCM );
	VARIABLE_IS_UNUSED( COMPRESSION_MPEG );
	VARIABLE_IS_UNUSED( COMPRESSION_Experimental );
#pragma endregion // Unused Compression Variables

	FILE* soundFile = nullptr;
	RIFFChunkHeader chunkHeader;
	RIFFFileHeader fileHeader;

	soundFile = AssetInterface::OpenAssetAsFile( filename, "rb" );
	if( soundFile == nullptr )
	{
		std::string errorMessage( "Unable to open file located at " );
		errorMessage.append( filename );
		errorMessage.append( ".\n The file either doesn't exist or has raised permissions." );
		FATAL_ERROR( "File Loading Error", errorMessage );
	}

	AssetInterface::ReadFromAssetFile( &fileHeader, sizeof( RIFFFileHeader ), 1, soundFile );
	VerifyRIFFFileHeaderOrDie( filename, fileHeader, RIFF_TYPE_WAVE );

	// Loop through the remaining chunks, skipping those I don't know how to deal with.
	long chunkStartPosition = AssetInterface::GetCurrentPositionInAssetFile( soundFile );
	AssetInterface::ReadFromAssetFile( &chunkHeader, sizeof( RIFFChunkHeader ), 1, soundFile );
	while ( !feof( soundFile ) )
	{
		if( chunkHeader.chunkID == WAVE_FORMAT_CHUNK_ID )
		{
			AssetInterface::SeekInAssetFile( soundFile, chunkStartPosition, SEEK_SET );

			WaveFormatChunk wavFormat;
			ExtractWAVFormatFromFileOrDie( wavFormat, filename, soundFile );

			// For now, we can only use Uncompressed PCM, as I don't have time to write decoders.
			if( wavFormat.compressionCode != COMPRESSION_UncompressedPCM )
			{
				std::string errorMessage( "Unable to open WAV file located at " );
				errorMessage.append( filename );
				errorMessage.append( ".\n This WAV uses a compressed format that this loader does not support." );
				FATAL_ERROR( "WAV File Loading Error", errorMessage );
			}

			out_sampleRateHertz = wavFormat.sampleRateHz;
			out_numChannels = wavFormat.numChannels;
			out_bitsPerSample = wavFormat.bitsPerSample;
		}
		else if( chunkHeader.chunkID == WAVE_DATA_CHUNK_ID )
		{
			AssetInterface::SeekInAssetFile( soundFile, chunkStartPosition, SEEK_SET );
			ExtractWAVDataOrDie( out_buffer, out_bufferSize, filename, soundFile );
		}
		else // This chunk is unsupported
		{
			// Don't bother rewinding here, the chunk size doesn't include the header anyway.
			AssetInterface::SeekInAssetFile( soundFile, chunkHeader.chunkSize, SEEK_CUR );
		}

		chunkStartPosition = AssetInterface::GetCurrentPositionInAssetFile( soundFile );
		AssetInterface::ReadFromAssetFile( &chunkHeader, sizeof( RIFFChunkHeader ), 1, soundFile );
	}
}
