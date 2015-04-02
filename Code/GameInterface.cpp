#include "GameInterface.hpp"

#include "EntityManager.hpp"


//-----------------------------------------------------------------------------------------------
STATIC GameInterface* GameInterface::s_gameInstancePointer = nullptr;
STATIC bool GameInterface::s_engineShouldShutdown = false;



//-----------------------------------------------------------------------------------------------
GameInterface::GameInterface()
{
#if defined( PLATFORM_ANDROID )
	app_dummy();
#endif //defined( PLATFORM_ANDROID )

	s_gameInstancePointer = this;

	m_activeEntityManager = new EntityManager( 100 );
}

//-----------------------------------------------------------------------------------------------
VIRTUAL void GameInterface::DoAtEndOfFrame()
{
	m_activeEntityManager->DoAtEndOfFrame();
}

//-----------------------------------------------------------------------------------------------
VIRTUAL void GameInterface::DoBeforeEngineDestruction()
{
	delete m_activeEntityManager;

	EventCourier::UnsubscribeFromAllEvents( this );
}
