#include "AssetInterface.hpp"

#include "AssertionError.hpp"
#include "EngineMacros.hpp"

#include "AndroidAssetInterface.hpp"
#include "StandardAssetInterface.hpp"

//-----------------------------------------------------------------------------------------------
STATIC AssetInterface* AssetInterface::s_activeAssetInterface = nullptr;
STATIC const char* AssetInterface::s_rootAssetDirectory = nullptr;


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
STATIC void AssetInterface::Startup( void* extraData )
{
	FATAL_ASSERTION( s_activeAssetInterface == nullptr, "Asset Interface Error",
		"Cannot start up multiple Asset Interfaces!" );

#if defined( PLATFORM_WINDOWS )
	VARIABLE_IS_UNUSED( extraData );
	s_activeAssetInterface = new StandardAssetInterface();
	s_rootAssetDirectory = "Data/";
#elif defined( PLATFORM_HTML5 )
	VARIABLE_IS_UNUSED( extraData );
	s_activeAssetInterface = new StandardAssetInterface();
	s_rootAssetDirectory = "Data/";
#elif defined( PLATFORM_ANDROID )
	s_activeAssetInterface = new AndroidAssetInterface( extraData );
	s_rootAssetDirectory = "Data/";
#elif defined( PLATFORM_PS3 )
	VARIABLE_IS_UNUSED( extraData );
	s_activeAssetInterface = new StandardAssetInterface();
	s_rootAssetDirectory = "/app_home/";
#elif defined( PLATFORM_VITA )
	VARIABLE_IS_UNUSED( extraData );
	s_activeAssetInterface = new StandardAssetInterface();
	s_rootAssetDirectory = "app0:";
#else
	s_activeAssetInterface = new StandardAssetInterface();
	s_rootAssetDirectory = "";
#endif
}

//-----------------------------------------------------------------------------------------------
STATIC void AssetInterface::Shutdown()
{
	RECOVERABLE_ASSERTION( s_activeAssetInterface != nullptr, "Asset Interface Error",
		"Cannot shut down an Asset Interface that has not yet been started!" );

	delete s_activeAssetInterface;
	s_activeAssetInterface = nullptr;
}
#pragma endregion //Lifecycle
