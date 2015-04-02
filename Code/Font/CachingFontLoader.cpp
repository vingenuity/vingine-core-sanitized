#include "CachingFontLoader.hpp"

#include "../Graphics/RendererInterface.hpp"
#include "../XML/pugixml.hpp"
#include "../AssertionError.hpp"
#include "../AssetInterface.hpp"
#include "../StringConversion.hpp"

//-----------------------------------------------------------------------------------------------
BitmapFont* CachingFontLoader::GetOrLoadFontFromXML(	const char* fontDefinitionFileLocation,
														const char* fontAtlasDirectory )
{
	std::map< const char*, BitmapFont >::iterator fontInRegistry;
	fontInRegistry = m_fontRegistry.find( fontDefinitionFileLocation );
	if( fontInRegistry != m_fontRegistry.end() )
	{
		return &fontInRegistry->second;
	}
	FILE* fontXMLFile = AssetInterface::OpenAssetAsFile( fontDefinitionFileLocation, "rb" );
	AssetInterface::SeekInAssetFile( fontXMLFile, 0, SEEK_END );
	size_t fontXMLFileSize = AssetInterface::GetCurrentPositionInAssetFile( fontXMLFile );
	AssetInterface::SeekInAssetFile( fontXMLFile, 0, SEEK_SET );

	unsigned char* fontXMLData = new unsigned char[ fontXMLFileSize ];
	AssetInterface::ReadFromAssetFile( fontXMLData, sizeof( unsigned char ), fontXMLFileSize, fontXMLFile );
	AssetInterface::CloseAssetFile( fontXMLFile );
	RECOVERABLE_ASSERTION( fontXMLData != nullptr, "Font Loading Error", "Unable to load font file." );

	pugi::xml_document fontXMLmetadata;
	pugi::xml_parse_result xmlParseResult = fontXMLmetadata.load_buffer( fontXMLData, fontXMLFileSize );
	delete fontXMLData;
	FATAL_ASSERTION( xmlParseResult != true, "Font Loading Error", "Unable to parse XML in font file." );

	// This will always be safe, as the font object is created by the lookup
	BitmapFont* newFont = &m_fontRegistry[ fontDefinitionFileLocation ];
	pugi::xml_node rootNode = fontXMLmetadata.child( "font" );



	pugi::xml_node fontInfoNode = rootNode.child( "info" );
	newFont->fontName = fontInfoNode.attribute( "face" ).value();



	pugi::xml_node fontCommonNode = rootNode.child( "common" );
	float lineHeightPixels = ConvertStringToFloat( fontCommonNode.attribute( "lineHeight" ).value() );
	float pixelToEmConversionFactor = 1.f / lineHeightPixels;

	// Since I render from bottom left instead of top right, some baseline conversion is necessary.
	float yDistanceFromTopToBaselinePixels = ConvertStringToFloat( fontCommonNode.attribute( "base" ).value() );
	newFont->fontYDistanceFromBaselineEm = ( lineHeightPixels - yDistanceFromTopToBaselinePixels ) * pixelToEmConversionFactor;

	float textureWidthPixels = ConvertStringToFloat( fontCommonNode.attribute( "scaleW" ).value() );
	float textureHeightPixels = ConvertStringToFloat( fontCommonNode.attribute( "scaleH" ).value() );
	FloatVector2 xyToUVConversionFactor( 1.f / textureWidthPixels, 1.f / textureHeightPixels );

	newFont->numberOfTextureAtlases = ConvertStringToUnsignedInt( fontCommonNode.attribute( "pages" ).value() );
	newFont->textureAtlases = new Texture*[ newFont->numberOfTextureAtlases ];



	pugi::xml_node fontAtlasNode = rootNode.child( "pages" );
	for ( pugi::xml_node atlasData = fontAtlasNode.child( "page" ); atlasData != nullptr; atlasData = atlasData.next_sibling( "page" ) )
	{
		unsigned int atlasIndex = ConvertStringToUnsignedInt( atlasData.attribute( "id" ).value() );
		std::string fontAtlasLocation = fontAtlasDirectory;
		fontAtlasLocation.append( atlasData.attribute( "file" ).value() );
		Texture* newAtlas = RendererInterface::GetTextureManager()->CreateOrGetTexture( fontAtlasLocation.c_str(), Texture::FILTER_linearInterpolation, Texture::WRAP_None, false );
		newFont->textureAtlases[ atlasIndex ] = newAtlas;
	}



	pugi::xml_node fontCharsNode = rootNode.child( "chars" );
	for ( pugi::xml_node glyphData = fontCharsNode.child( "char" ); glyphData != nullptr; glyphData = glyphData.next_sibling( "char" ) )
	{
		FloatVector2 glyphMinXYInPixels( ConvertStringToFloat( glyphData.attribute( "x" ).value() ),
			ConvertStringToFloat( glyphData.attribute( "y" ).value() ) );

		float glyphWidthInPixels = ConvertStringToFloat( glyphData.attribute( "width" ).value() );
		FloatVector2 glyphDimensionsInPixels( glyphWidthInPixels,
			ConvertStringToFloat( glyphData.attribute( "height" ).value() ) );

		Glyph newGlyph( ConvertStringToUnsignedInt( glyphData.attribute( "page" ).value() ),
			MultiplyComponents( glyphMinXYInPixels, xyToUVConversionFactor ),
			MultiplyComponents( glyphMinXYInPixels + glyphDimensionsInPixels, xyToUVConversionFactor ),
			ConvertStringToFloat( glyphData.attribute( "xadvance" ).value() ) * pixelToEmConversionFactor,
			ConvertStringToFloat( glyphData.attribute( "xoffset" ).value() ) * pixelToEmConversionFactor,
			glyphWidthInPixels * pixelToEmConversionFactor );

		unsigned int ucsIndex = ConvertStringToUnsignedInt( glyphData.attribute( "id" ).value() );
		newFont->glyphData[ ucsIndex ] = newGlyph;
	}
	return newFont;
}