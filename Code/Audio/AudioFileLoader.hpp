#pragma once
#ifndef INCLUDED_AUDIO_FILE_LOADER_HPP
#define INCLUDED_AUDIO_FILE_LOADER_HPP

//-----------------------------------------------------------------------------------------------
#include <string>

//-----------------------------------------------------------------------------------------------
void LoadWAVOrDie( const char* filename, unsigned char*& out_buffer,
	size_t& out_bufferSize, unsigned int& out_sampleRateHertz,
	unsigned int& out_numChannels, unsigned int& out_bitsPerSample );

#endif //INCLUDED_AUDIO_FILE_LOADER_HPP
