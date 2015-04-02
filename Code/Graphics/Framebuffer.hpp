#ifndef INCLUDED_FRAME_BUFFER_HPP
#define INCLUDED_FRAME_BUFFER_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "Texture.hpp"

//-----------------------------------------------------------------------------------------------
class Framebuffer
{
public:
	static const unsigned int MAXIMUM_COLOR_OUTPUT_TEXTURES = 4;

	enum Target
	{
		TARGET_FOR_WRITING = 1,
		TARGET_FOR_READING = 2,
		TARGET_FOR_READING_AND_WRITING = 3
	};

	Framebuffer()
		: m_framebufferID( 0 )
		, m_targetForReadingOrWriting( TARGET_FOR_READING_AND_WRITING )
		, m_attachedDepthTexture( nullptr )
		, m_attachedStencilTexture( nullptr )
	{
		memset( m_attachedColorTextures, 0, MAXIMUM_COLOR_OUTPUT_TEXTURES * sizeof( Texture* ) );
	}

	Framebuffer( int framebufferID, Target targetForReadingOrWriting )
		: m_framebufferID( framebufferID )
		, m_targetForReadingOrWriting( targetForReadingOrWriting )
		, m_attachedDepthTexture( nullptr )
		, m_attachedStencilTexture( nullptr )
	{
		memset( m_attachedColorTextures, 0, MAXIMUM_COLOR_OUTPUT_TEXTURES * sizeof( Texture* ) );
	}

	~Framebuffer()
	{
		for( unsigned int i = 0; i < MAXIMUM_COLOR_OUTPUT_TEXTURES; ++i )
		{
			if( m_attachedColorTextures[ i ] != nullptr )
				delete m_attachedColorTextures[ i ];
		}

		if( m_attachedDepthTexture != nullptr )
			delete m_attachedDepthTexture;

		if( m_attachedStencilTexture != nullptr )
			delete m_attachedStencilTexture;
	}

	unsigned int GetID() const { return m_framebufferID; }
	Target GetOutputTarget() const { return m_targetForReadingOrWriting; }
	const Texture* GetAttachedColorTexture( unsigned int colorSlot ) const
	{
		assert( colorSlot < MAXIMUM_COLOR_OUTPUT_TEXTURES );
		return m_attachedColorTextures[ colorSlot ];
	}
	const Texture* GetAttachedDepthTexture() const { return m_attachedDepthTexture; }
	const Texture* GetAttachedStencilTexture() const { return m_attachedStencilTexture; }

	unsigned int m_framebufferID;
	Target m_targetForReadingOrWriting;
	Texture* m_attachedColorTextures[ MAXIMUM_COLOR_OUTPUT_TEXTURES ];
	Texture* m_attachedDepthTexture;
	Texture* m_attachedStencilTexture;
};

#endif //INCLUDED_FRAME_BUFFER_HPP
