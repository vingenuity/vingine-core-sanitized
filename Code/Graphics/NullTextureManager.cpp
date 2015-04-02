#include "NullTextureManager.hpp"

//-----------------------------------------------------------------------------------------------
Texture* NullTextureManager::CreateOrGetTexture( const char* /*textureFileLocation*/, 
												 Texture::FilteringMethod /*filterMethod*/, 
												 Texture::WrappingMode /*wrapMode*/,
												 bool /*flipTexture*/ )
{
	return CreateDefaultDiffuseTexture( NULL_TEXTURE_WIDTH, NULL_TEXTURE_HEIGHT );
}
