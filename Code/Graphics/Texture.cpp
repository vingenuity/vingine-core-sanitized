#include "../EngineMacros.hpp"
#include "../FileIOInterface.hpp"
#include "RendererInterface.hpp"
#include "stb_image.h"
#include "Texture.hpp"

//-----------------------------------------------------------------------------------------------
STATIC const Texture::FilteringMethod Texture::FILTER_nearestNeighbor = RendererInterface::NEAREST_NEIGHBOR;
STATIC const Texture::FilteringMethod Texture::FILTER_linearInterpolation = RendererInterface::LINEAR_INTERPOLATION;

STATIC const Texture::WrappingMode Texture::WRAP_None = RendererInterface::CLAMP_TO_EDGE;
STATIC const Texture::WrappingMode Texture::WRAP_Repeat = RendererInterface::REPEAT_OVER_GEOMETRY;
STATIC const Texture::WrappingMode Texture::WRAP_RepeatFlipped = RendererInterface::REPEAT_FLIPPED;



/*
//-----------------------------------------------------------------------------------------------
Texture::Texture( unsigned int xWidth, unsigned int yHeight, const Color& color )
	: m_openGLTextureID( 0 )
	, m_imageWidth( xWidth )
	, m_imageHeight( yHeight )
	, m_fileLocation( "" )
{
	static const unsigned int NUMBER_OF_COLOR_COMPONENTS = 4;

	unsigned char redComponentAsChar   = static_cast< unsigned char >( color.r * 255.f );
	unsigned char greenComponentAsChar = static_cast< unsigned char >( color.g * 255.f );
	unsigned char blueComponentAsChar  = static_cast< unsigned char >( color.b * 255.f );
	unsigned char alphaComponentAsChar = static_cast< unsigned char >( color.a * 255.f );

	unsigned int totalNumberOfColorComponents = xWidth * yHeight * NUMBER_OF_COLOR_COMPONENTS;
	unsigned char* textureData = new unsigned char[ totalNumberOfColorComponents ];

	for( unsigned int i = 0; i < totalNumberOfColorComponents; i += NUMBER_OF_COLOR_COMPONENTS )
	{
		textureData[ i     ] = redComponentAsChar;
		textureData[ i + 1 ] = greenComponentAsChar;
		textureData[ i + 2 ] = blueComponentAsChar;
		textureData[ i + 3 ] = alphaComponentAsChar;
	}

	RendererInterface* renderer = RendererInterface::GetRenderer();

	static const unsigned int BYTE_ALIGNED = 1;
	RendererInterface::SetTextureInputImageAlignment( BYTE_ALIGNED );

	RendererInterface::GenerateTextureIDs( 1, &m_openGLTextureID );
	RendererInterface::BindTexture( RendererInterface::TEXTURES_2D, this );

	RendererInterface::SetTextureWrappingMode( RendererInterface::TEXTURES_2D, RendererInterface::CLAMP_TO_EDGE );
	RendererInterface::SetTextureMagnificationMode( RendererInterface::TEXTURES_2D, RendererInterface::LINEAR_INTERPOLATION );

	RendererInterface::CreateTextureFrom2DImage( RendererInterface::TEXTURES_2D, 
		DEFAULT_LEVEL_OF_DETAIL,
		RendererInterface::RGBA,
		m_imageWidth,
		m_imageHeight,
		RendererInterface::RGBA,
		RendererInterface::TYPE_UNSIGNED_BYTE,
		textureData );
}

//-----------------------------------------------------------------------------------------------
Texture::Texture( AttachmentType attachmentType, unsigned int framebufferXWidth, unsigned int framebufferYHeight )
	: m_imageWidth( framebufferXWidth )
	, m_imageHeight( framebufferYHeight )
	, m_fileLocation( "" )
{
	RendererInterface* renderer = RendererInterface::GetRenderer();

	RendererInterface::GenerateTextureIDs( 1, &m_openGLTextureID );
	RendererInterface::BindTexture( RendererInterface::TEXTURES_2D, this );

	static const unsigned int NO_MIPMAPS = 0;
	switch( attachmentType )
	{
	case ATTACHMENT_COLOR:
		RendererInterface::SetTextureWrappingMode( RendererInterface::TEXTURES_2D, RendererInterface::CLAMP_TO_EDGE );
		RendererInterface::SetTextureMagnificationMode( RendererInterface::TEXTURES_2D, RendererInterface::LINEAR_INTERPOLATION );
		RendererInterface::SetTextureMinificationMode( RendererInterface::TEXTURES_2D, RendererInterface::LINEAR_INTERPOLATION );

		RendererInterface::CreateTextureFrom2DImage( RendererInterface::TEXTURES_2D, NO_MIPMAPS, RendererInterface::RGBA, 
											m_imageWidth, m_imageHeight, 
											RendererInterface::RGBA, RendererInterface::TYPE_UNSIGNED_BYTE, NULL );
		break;


	case ATTACHMENT_DEPTH:
		RendererInterface::SetTextureWrappingMode( RendererInterface::TEXTURES_2D, RendererInterface::CLAMP_TO_EDGE );
		RendererInterface::SetTextureMagnificationMode( RendererInterface::TEXTURES_2D, RendererInterface::NEAREST_NEIGHBOR );
		RendererInterface::SetTextureMinificationMode( RendererInterface::TEXTURES_2D, RendererInterface::NEAREST_NEIGHBOR );

		RendererInterface::CreateTextureFrom2DImage( RendererInterface::TEXTURES_2D, NO_MIPMAPS, RendererInterface::COMPONENTS_DEPTH, 
											m_imageWidth, m_imageHeight, 
											RendererInterface::COMPONENTS_DEPTH, RendererInterface::TYPE_UNSIGNED_INTEGER, NULL );
		break;


	case ATTACHMENT_STENCIL:
	default:
		break;
	}
}

//-----------------------------------------------------------------------------------------------
Texture::Texture( FILE* textureFile, FilteringMethod filterMethod, WrappingMode wrapMode )
	: m_openGLTextureID( 0 )
	, m_imageWidth( 0 )
	, m_imageHeight( 0 )
{
	int numberOfColorComponents, requestedNumberOfColorComponents = 0;
	unsigned char* textureData = stbi_load_from_file( textureFile, &m_imageWidth, &m_imageHeight, 
											&numberOfColorComponents, requestedNumberOfColorComponents );
	// FIX: Handle error case where texture file does not load
	RendererInterface::ColorComponents bufferTextureComponentFormat;
	if( numberOfColorComponents == 4 )
		bufferTextureComponentFormat = RendererInterface::RGBA;
	else
		bufferTextureComponentFormat = RendererInterface::RGB;
	RendererInterface::ColorComponents cardTextureComponentFormat = bufferTextureComponentFormat;
	// FIX: This doesn't correctly handle exotic component formats

	RendererInterface* renderer = RendererInterface::GetRenderer();

	static const unsigned int BYTE_ALIGNED = 1;
	RendererInterface::SetTextureInputImageAlignment( BYTE_ALIGNED );

	RendererInterface::GenerateTextureIDs( 1, &m_openGLTextureID );
	RendererInterface::BindTexture( RendererInterface::TEXTURES_2D, this );

	switch( wrapMode )
	{
	case WRAP_clampToEdge:
		RendererInterface::SetTextureWrappingMode( RendererInterface::TEXTURES_2D, RendererInterface::CLAMP_TO_EDGE );
		break;
	case WRAP_repeatOverGeometry:
		RendererInterface::SetTextureWrappingMode( RendererInterface::TEXTURES_2D, RendererInterface::REPEAT_OVER_GEOMETRY );
		break;
	default:
		return;
	}

	switch( filterMethod )
	{
	case FILTER_nearestNeighbor:
		RendererInterface::SetTextureMagnificationMode( RendererInterface::TEXTURES_2D, RendererInterface::NEAREST_NEIGHBOR );
		break;
	case FILTER_linearInterpolation:
		RendererInterface::SetTextureMagnificationMode( RendererInterface::TEXTURES_2D, RendererInterface::LINEAR_INTERPOLATION );
		break;
	default:
		return;
	}
	RendererInterface::SetTextureMinificationMode( RendererInterface::TEXTURES_2D, RendererInterface::INTERPOLATE_MIPMAPS_NEAREST_TEXTURE );

	RendererInterface::CreateTextureFrom2DImage( RendererInterface::TEXTURES_2D, 
										DEFAULT_LEVEL_OF_DETAIL,
										cardTextureComponentFormat,
										m_imageWidth,
										m_imageHeight,
										bufferTextureComponentFormat,
										RendererInterface::TYPE_UNSIGNED_BYTE,
										textureData );

	RendererInterface::SetMaximumMipmapLevel( RendererInterface::TEXTURES_2D, 4 );
	RendererInterface::SetMipmapQuality( RendererInterface::FASTEST );
	RendererInterface::GenerateMipmaps( RendererInterface::TEXTURES_2D );

	stbi_image_free( textureData );
}

//-----------------------------------------------------------------------------------------------
Texture::Texture( nv_dds::CDDSImage* image, FilteringMethod filterMethod, WrappingMode wrapMode )
	: m_openGLTextureID( 0 )
	, m_imageWidth( image->get_width() )
	, m_imageHeight( image->get_height() )
{
	RendererInterface* renderer = RendererInterface::GetRenderer();

	RendererInterface::GenerateTextureIDs( 1, &m_openGLTextureID );
	RendererInterface::BindTexture( RendererInterface::TEXTURES_2D, this );

	switch( wrapMode )
	{
	case clampToEdge:
		RendererInterface::SetTextureWrappingMode( RendererInterface::TEXTURES_2D, RendererInterface::CLAMP_TO_EDGE );
		break;
	case repeatOverGeometry:
		RendererInterface::SetTextureWrappingMode( RendererInterface::TEXTURES_2D, RendererInterface::REPEAT_OVER_GEOMETRY );
		break;
	default:
		return;
	}

	switch( filterMethod )
	{
	case nearestNeighbor:
		RendererInterface::SetTextureMagnificationMode( RendererInterface::TEXTURES_2D, RendererInterface::NEAREST_NEIGHBOR );
		break;
	case linearInterpolation:
		RendererInterface::SetTextureMagnificationMode( RendererInterface::TEXTURES_2D, RendererInterface::LINEAR_INTERPOLATION );
		break;
	default:
		return;
	}
	RendererInterface::SetTextureMinificationMode( RendererInterface::TEXTURES_2D, RendererInterface::INTERPOLATE_MIPMAPS_NEAREST_TEXTURE );

	RendererInterface::SetMipmapQuality( RendererInterface::FASTEST );
	RendererInterface::SetMaximumMipmapLevel( RendererInterface::TEXTURES_2D, image->get_num_mipmaps() );

	if( image->is_compressed() )
	{
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, image->get_format(), 
			image->get_width(), image->get_height(), 0, image->get_size(), 
			*image);

		for (unsigned int i = 0; i < image->get_num_mipmaps(); i++)
		{
			nv_dds::CSurface mipmap = image->get_mipmap(i);

			glCompressedTexImage2D(GL_TEXTURE_2D, i+1, image->get_format(), 
				mipmap.get_width(), mipmap.get_height(), 0, mipmap.get_size(), 
				mipmap);
		}
	}
	else
	{	
		RendererInterface::CreateTextureFrom2DImage( RendererInterface::TEXTURES_2D, 
			DEFAULT_LEVEL_OF_DETAIL,
			( RendererInterface::ColorComponents )image->get_components(),
			m_imageWidth,
			m_imageHeight,
			( RendererInterface::ColorComponents )image->get_format(),
			RendererInterface::UNSIGNED_BYTE,
			*image );

		for (unsigned int i = 0; i < image->get_num_mipmaps(); i++)
		{
			RendererInterface::CreateTextureFrom2DImage( RendererInterface::TEXTURES_2D, 
				i+1,
				( RendererInterface::ColorComponents )image->get_components(),
				image->get_mipmap(i).get_width(),
				image->get_mipmap(i).get_height(),
				( RendererInterface::ColorComponents )image->get_format(),
				RendererInterface::UNSIGNED_BYTE,
				image->get_mipmap(i) );
		}
	}
}


//-----------------------------------------------------------------------------------------------
STATIC Texture* Texture::CreateTextureOfSizeWithColor( unsigned int width, unsigned int height, const Color& color )
{
	return new Texture( width, height, color );
}

//-----------------------------------------------------------------------------------------------
STATIC Texture* Texture::CreateColorTextureForFramebuffer( unsigned int windowXWidth, unsigned int windowYHeight )
{
	return new Texture( ATTACHMENT_COLOR, windowXWidth, windowYHeight );
}

//-----------------------------------------------------------------------------------------------
STATIC Texture* Texture::CreateDepthTextureForFramebuffer( unsigned int windowXWidth, unsigned int windowYHeight )
{
	return new Texture( ATTACHMENT_DEPTH, windowXWidth, windowYHeight );
}

//-----------------------------------------------------------------------------------------------
STATIC Texture* Texture::CreateOrGetTexture( const std::string& textureFileLocation, FilteringMethod filterMethod, WrappingMode wrapMode )
{
	std::map< std::string, Texture* >::iterator textureIterator = s_activeTextureRegistry.find( textureFileLocation );

	if( textureIterator == s_activeTextureRegistry.end() )
	{
		std::string textureFileExtension = GetFileExtensionForFilePath( textureFileLocation );
		Texture* texture = nullptr;

		if( textureFileExtension.compare( "dds" ) == 0 )
		{
			//nv_dds::CDDSImage image;
			//image.load( textureFileLocation, false );

			//texture = new Texture( &image, filterMethod, wrapMode );
		}
		else
		{
			FILE* textureFile;
			int errorResult = 0;
		#ifdef PLATFORM_WINDOWS
			errorResult = fopen_s( &textureFile, textureFileLocation.c_str(), "rb" );
		#else
			textureFile = fopen( textureFileLocation.c_str(), "rb" );
		#endif
			if( errorResult != 0 )
			{
				exit( -16 );
			}

			texture = new Texture( textureFile, filterMethod, wrapMode );
		}

		s_activeTextureRegistry[ textureFileLocation ] = texture;
		return texture;
	}

	return textureIterator->second;
}

//-----------------------------------------------------------------------------------------------
STATIC Texture* Texture::CreateOrGetTexture( const std::wstring& textureFileLocation, FilteringMethod filterMethod, WrappingMode wrapMode )
{
	std::map< std::wstring, Texture* >::iterator textureIterator = s_activeTextureRegistryUnicode.find( textureFileLocation );

	if( textureIterator == s_activeTextureRegistryUnicode.end() )
	{
		std::wstring textureFileExtension = GetFileExtensionForFilePath( textureFileLocation );
		Texture* texture = nullptr;

		if( textureFileExtension.compare( L"dds" ) == 0 )
		{
			//nv_dds::CDDSImage image;
			//image.load( textureFileLocation, false );

			//texture = new Texture( &image, filterMethod, wrapMode );
		}
		else
		{
			FILE* textureFile;
			int errorResult = 0;
		#ifdef PLATFORM_WINDOWS
			errorResult = _wfopen_s( &textureFile, textureFileLocation.c_str(), L"rb" );
		#else
			errorResult = 1; //We can't support this, no wide string support
		#endif
			if( errorResult != 0 )
			{
				exit( -16 );
			}

			texture = new Texture( textureFile, filterMethod, wrapMode );
		}

		s_activeTextureRegistryUnicode[ textureFileLocation ] = texture;
		return texture;
	}

	return textureIterator->second;
}

//-----------------------------------------------------------------------------------------------
STATIC void Texture::CleanUpTextureRepositories()
{
	RendererInterface* renderer = RendererInterface::GetRenderer();

	Texture* texture;
	for( std::map< std::string, Texture* >::iterator pair = s_activeTextureRegistry.begin();
													 pair != s_activeTextureRegistry.end();
													 ++pair )
	{
		texture = pair->second;
		RendererInterface::DeleteTextureDataOnCard( texture );
		delete texture;
	}
	s_activeTextureRegistry.clear();

	for( std::map< std::wstring, Texture* >::iterator pair = s_activeTextureRegistryUnicode.begin();
		pair != s_activeTextureRegistryUnicode.end();
		++pair )
	{
		texture = pair->second;
		RendererInterface::DeleteTextureDataOnCard( texture );
		delete texture;
	}
	s_activeTextureRegistryUnicode.clear();
}*/
