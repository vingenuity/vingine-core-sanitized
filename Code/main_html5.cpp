#include "EngineMacros.hpp"
#ifdef PLATFORM_HTML5

#include "PlatformSpecificHeaders.hpp"
#include <SDL/SDL.h>

#include <vector>

#include "Audio/AudioInterface.hpp"

#include "Events/EventCourier.hpp"

#include "Graphics/DebugDrawingSystem2D.hpp"
#include "Graphics/RendererInterface.hpp"

#include "Input/PeripheralInterface.hpp"

#include "AssertionError.hpp"
#include "AssetInterface.hpp"
#include "BuildPreferences.hpp"
#include "Bitmasking.hpp"
#include "DebuggerInterface.hpp"
#include "GameInterface.hpp"
#include "StringConversion.hpp"


//-----------------------------------------------------------------------------------------------
unsigned int GAME_WINDOW_WIDTH = 1280;
unsigned int GAME_WINDOW_HEIGHT = 720;

extern const double FRAME_RATE_LIMIT_FPS;
extern const double FRAME_RATE_LIMIT_SECONDS;

SDL_Surface* g_canvas;

//-----------------------------------------------------------------------------------------------
SDL_Surface* CreateOpenGLCanvas()
{
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	return SDL_SetVideoMode( GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, 32, SDL_OPENGL );
}

//-----------------------------------------------------------------------------------------------
void ToggleFullscreen()
{
	EmscriptenFullscreenChangeEvent fullscreenData;
	EMSCRIPTEN_RESULT emResult = emscripten_get_fullscreen_status( &fullscreenData );

	if( !fullscreenData.fullscreenEnabled )
		return; //the browser doesn't support full screen.

	if( fullscreenData.isFullscreen )
	{
		emscripten_exit_fullscreen();
	}
	else
	{
		 //This propagates the fullscreenrequest to the next user input event.
		emResult = emscripten_request_fullscreen( 0, true );
	}
}



#pragma region Gamepad Handling
//-----------------------------------------------------------------------------------------------
static std::vector<Gamepad::ID> g_HTML5IndexToGamepadIDMapping;

//-----------------------------------------------------------------------------------------------
EM_BOOL HandleGamepadConnection( int /*eventType*/, const EmscriptenGamepadEvent* gamepadEvent, void* /*userData*/ )
{
	WriteToDebuggerOutput( "Gamepad Connected." );
	WriteToDebuggerOutput( ConvertIntegerToString( gamepadEvent->index ) );
	g_HTML5IndexToGamepadIDMapping.resize( gamepadEvent->index + 1, Gamepad::ID_Null );
	g_HTML5IndexToGamepadIDMapping[ gamepadEvent->index ] = PeripheralInterface::ConnectNewGamepad( gamepadEvent->numAxes,
																									gamepadEvent->numButtons,
																									0.25f );
	return true;
}

//-----------------------------------------------------------------------------------------------
EM_BOOL HandleGamepadDisconnection( int /*eventType*/, const EmscriptenGamepadEvent* gamepadEvent, void* /*userData*/ )
{
	WriteToDebuggerOutput( "Gamepad Disconnected." );
	WriteToDebuggerOutput( ConvertIntegerToString( gamepadEvent->index ) );
	PeripheralInterface::DisconnectGamepad( g_HTML5IndexToGamepadIDMapping[ gamepadEvent->index ] );
	g_HTML5IndexToGamepadIDMapping[ gamepadEvent->index ] = Gamepad::ID_Null;
	return true;
}

//-----------------------------------------------------------------------------------------------
EM_BOOL PollGamepads( float /*deltaSeconds*/ )
{
	static EmscriptenGamepadEvent gamepadEvent;

	int numberOfConnectedGamepads = emscripten_get_num_gamepads();
	if( numberOfConnectedGamepads == EMSCRIPTEN_RESULT_NOT_SUPPORTED )
		return false;

	for( int i = 0; i < numberOfConnectedGamepads; ++i )
	{
		if( g_HTML5IndexToGamepadIDMapping[ i ] == Gamepad::ID_Null )
			continue;

		Gamepad* const& gamepad = PeripheralInterface::GetGamepad( g_HTML5IndexToGamepadIDMapping[ i ] );
		emscripten_get_gamepad_status( i, &gamepadEvent );

		gamepad->currAxisState[ 0 ] = static_cast<float>( gamepadEvent.axis[ 0 ] );
		gamepad->currAxisState[ 1 ] = -static_cast<float>( gamepadEvent.axis[ 1 ] );
		for( unsigned int axisID = 2; axisID < gamepad->numberOfAxes; ++axisID )
		{
			gamepad->currAxisState[ axisID ] = static_cast< float >( gamepadEvent.axis[ axisID ] );
		}

		for( unsigned int buttonID = 0; buttonID < gamepadEvent.numButtons; ++buttonID )
		{
			gamepad->currButtonState[ buttonID ] = static_cast< float >( gamepadEvent.digitalButton[ buttonID ] );
		}
	}
	return true;
}
#pragma endregion //Gamepad Handling



#pragma region Mouse Handling
//-----------------------------------------------------------------------------------------------
EM_BOOL HandleMouseDownEvent( int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData )
{
	PeripheralInterface::GetMouse()->SetButtonState( mouseEvent->button, true );
	return true;
}

//-----------------------------------------------------------------------------------------------
EM_BOOL HandleMouseMoveEvent( int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData )
{
	Mouse* mouse = PeripheralInterface::GetMouse();
	mouse->SetScreenPosition( mouseEvent->screenX, mouseEvent->screenY );
	mouse->SetWindowPosition( mouseEvent->canvasX, GAME_WINDOW_HEIGHT - mouseEvent->canvasY );
	mouse->SetPositionDelta( -mouseEvent->movementX, mouseEvent->movementY );
	return true;
}

//-----------------------------------------------------------------------------------------------
EM_BOOL HandleMouseUpEvent( int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData )
{
	PeripheralInterface::GetMouse()->SetButtonState( mouseEvent->button, false );
	return true;
}

//-----------------------------------------------------------------------------------------------
EM_BOOL HandleMouseWheelEvent( int eventType, const EmscriptenWheelEvent* wheelEvent, void* userData )
{
	static const float MOUSE_WHEEL_SCALE = 0.333f;
	PeripheralInterface::GetMouse()->SetWheelDelta( -wheelEvent->deltaY * MOUSE_WHEEL_SCALE, 
													-wheelEvent->deltaX * MOUSE_WHEEL_SCALE );
	return true;
}
#pragma endregion //Mouse Handling



#pragma region Keyboard/SDL Handling
//-----------------------------------------------------------------------------------------------
// At this point, this function is only here so that the security requests will respond on key up
EM_BOOL HandleKeyUpEvent( int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData )
{
	return true;
}

//-----------------------------------------------------------------------------------------------
void HandleSDLEvent( const SDL_Event& event )
{
	switch( event.type )
	{
	case SDL_KEYDOWN:
		if( event.key.keysym.scancode == SDL_SCANCODE_F11 )
			ToggleFullscreen();
		else if( event.key.keysym.scancode == SDL_SCANCODE_F9 )
		{
			if( PeripheralInterface::GetMouse()->IsLockedToWindow() )
				PeripheralInterface::GetMouse()->UnlockFromWindow();
			else
				PeripheralInterface::GetMouse()->LockToWindow();
		}
		else
		{
			PeripheralInterface::GetKeyboard()->SetKeyDown( event.key.keysym.scancode );
		}
		break;
	case SDL_KEYUP:
		PeripheralInterface::GetKeyboard()->SetKeyUp( event.key.keysym.scancode );
		break;
	case SDL_QUIT:
		ImmediatelyExit( 0 );
		break;
	default:
		break;
	}
}
#pragma endregion //Keyboard/SDL Handling



//-----------------------------------------------------------------------------------------------
void RegisterEventCallbacks()
{
	//A couple of quick notes about emscripten callbacks:
	// The first argument refers to the canvas element to watch (nullptr chooses the default, which should be the whole window)
	// The second argument allows the sending of user data that will be sent back with each event.
	// The third tells whether to fire this event during the capture phase of event bubbling.


	//Gamepad
	emscripten_set_gamepadconnected_callback( nullptr, true, HandleGamepadConnection );
	emscripten_set_gamepaddisconnected_callback( nullptr, true, HandleGamepadDisconnection );

	//Keyboard
	emscripten_set_keyup_callback( nullptr, nullptr, true, HandleKeyUpEvent );

	//Mouse
	emscripten_set_mousedown_callback( nullptr, nullptr, true, HandleMouseDownEvent );
	emscripten_set_mousemove_callback( nullptr, nullptr, true, HandleMouseMoveEvent );
	emscripten_set_mouseup_callback( nullptr, nullptr, true, HandleMouseUpEvent );
	emscripten_set_wheel_callback( nullptr, nullptr, true, HandleMouseWheelEvent );
}

//-----------------------------------------------------------------------------------------------
void PumpEventQueue()
{
	SDL_Event event;
	while( SDL_PollEvent(&event) )
	{
		HandleSDLEvent( event );
	}
}

//-----------------------------------------------------------------------------------------------
void Update( double deltaSeconds )
{
	PollGamepads( deltaSeconds );
	GameInterface::Update( deltaSeconds );
	AudioInterface::Update( deltaSeconds );
	PeripheralInterface::Update( deltaSeconds );
}

//-----------------------------------------------------------------------------------------------
void Render()
{
	GameInterface::Render();

	SDL_Flip( g_canvas ); //Strange name; swaps the two render buffers.
}

//-----------------------------------------------------------------------------------------------
void GameLoop()
{
	static double frameStartTime = GetCurrentTimeSeconds();
	static double timeSpentLastFrameSeconds = 0.0;

	timeSpentLastFrameSeconds = GetCurrentTimeSeconds() - frameStartTime;

	frameStartTime = GetCurrentTimeSeconds();

	PumpEventQueue();
	Update( timeSpentLastFrameSeconds );
	Render();
	GameInterface::EndOfFrame();
}

//-----------------------------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
	SDL_Init( SDL_INIT_VIDEO );
	atexit( SDL_Quit );

	g_canvas = CreateOpenGLCanvas();
	if ( g_canvas == NULL )
	{
		WriteToDebuggerOutput( "Couldn't set OpenGL mode." );
		ImmediatelyExit( 1 );
	}

	RegisterEventCallbacks();

	GameInterface::BeforeEngineInitialization();

	EventCourier::Startup();
	RendererInterface::Startup();
	AudioInterface::Startup();
	PeripheralInterface::Startup();
	AssetInterface::Startup();

	GameInterface::BeforeFirstFrame( GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT );

	emscripten_set_main_loop( GameLoop, FRAME_RATE_LIMIT_SECONDS, false );

	atexit( GameInterface::BeforeEngineDestruction );
	atexit( AssetInterface::Shutdown );
	atexit( PeripheralInterface::Shutdown );
	atexit( AudioInterface::Shutdown );
	atexit( RendererInterface::Shutdown );
	atexit( EventCourier::Shutdown );
	atexit( GameInterface::AfterEngineDestruction );
}

#endif