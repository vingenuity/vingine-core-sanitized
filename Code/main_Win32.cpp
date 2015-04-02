#include "EngineMacros.hpp"

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#if defined( _DEBUG )
#include <vld.h>
#else
#include <windows.h>
#endif

#include <Windowsx.h>
#include <Xinput.h>

#include "Audio/AudioInterface.hpp"
#include "Events/EventCourier.hpp"
#include "Graphics/DebugDrawingSystem2D.hpp"
#include "Graphics/RendererInterface.hpp"
#include "Graphics/Texture.hpp"
#include "Input/PeripheralInterface.hpp"
#include "AssertionError.hpp"
#include "AssetInterface.hpp"
#include "BuildPreferences.hpp"
#include "CommandLineManager.hpp"
#include "DebuggerInterface.hpp"
#include "GameInterface.hpp"
#include "StringConversion.hpp"
#include "TimeInterface.hpp"

#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#pragma warning( disable : 4996 ) // Ignore warning about freopen (I prefer standard C++ libraries)


//-----------------------------------------------------------------------------------------------
bool g_isQuitting = false;
HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;
WINDOWPLACEMENT g_previousWindowPlacement;
bool g_openConsole = false;
static Gamepad::ID g_xinputToGamepadIDMapping[ 4 ];

unsigned int GAME_WINDOW_WIDTH = 720;
unsigned int GAME_WINDOW_HEIGHT = 720;
unsigned int GAME_WINDOW_SCREEN_X_POS = 50;
unsigned int GAME_WINDOW_SCREEN_Y_POS = 50;

unsigned int SCREEN_WIDTH = 0;
unsigned int SCREEN_HEIGHT = 0;

extern const double FRAME_RATE_LIMIT_FPS;
extern const double FRAME_RATE_LIMIT_SECONDS;

//-----------------------------------------------------------------------------------------------
//This code thanks to Raymond Chen:
//http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx
void ToggleFullscreen()
{
	long currentWindowStyle = GetWindowLong( g_hWnd, GWL_STYLE ); //Get data about our current window style

	if( currentWindowStyle & WS_OVERLAPPEDWINDOW ) //If we have a title bar
	{
		BOOL gotWindowData = true;
		gotWindowData &= GetWindowPlacement( g_hWnd, &g_previousWindowPlacement ); //Store our window placement for restoration later
		
		//Get either the monitor that contains most of the window, or the primary monitor if not sure
		MONITORINFO currentMonitor = { sizeof( currentMonitor ) };
		gotWindowData &= GetMonitorInfo( MonitorFromWindow( g_hWnd, MONITOR_DEFAULTTOPRIMARY ), &currentMonitor );

		if( gotWindowData )
		{
			//Turn off the border and make the window fill the monitor
			SetWindowLong( g_hWnd, GWL_STYLE, currentWindowStyle & ~WS_OVERLAPPEDWINDOW );
			SetWindowPos( g_hWnd, HWND_TOP,
				currentMonitor.rcMonitor.left, currentMonitor.rcMonitor.top,
				currentMonitor.rcMonitor.right - currentMonitor.rcMonitor.left,
				currentMonitor.rcMonitor.bottom - currentMonitor.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED );
		}
	}
	else //We are in fullscreen
	{
		SetWindowLong( g_hWnd, GWL_STYLE, currentWindowStyle | WS_OVERLAPPEDWINDOW );
		SetWindowPlacement( g_hWnd, &g_previousWindowPlacement );
		SetWindowPos( g_hWnd, nullptr, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED );
	}
}

//-----------------------------------------------------------------------------------------------
void HandleRawGamepadInput( RAWINPUT* gamepadInput )
{
	static const size_t STORAGE_BUFFER_SIZE = 64;
	static BYTE gamepadInfoBuffer[STORAGE_BUFFER_SIZE];

	size_t bufferSize;
	GetRawInputDeviceInfo( gamepadInput->header.hDevice, RIDI_DEVICEINFO, nullptr, &bufferSize );

	GetRawInputDeviceInfo( gamepadInput->header.hDevice, RIDI_DEVICEINFO, &gamepadInfoBuffer, &bufferSize );
	RID_DEVICE_INFO* gamepadInfo = reinterpret_cast<RID_DEVICE_INFO*>( gamepadInfoBuffer );
	if( gamepadInfo->dwType != RIM_TYPEHID )
	{
		WriteToDebuggerOutput( "Whoops.\n" );
		return;
	}
	WriteToDebuggerOutput( "Vendor ID: " );
	WriteToDebuggerOutput( ConvertIntegerToString( gamepadInfo->hid.dwVendorId ) );
	WriteToDebuggerOutput( "\n" );

	static BYTE hidClassBuffer[STORAGE_BUFFER_SIZE];

	GetRawInputDeviceInfo( gamepadInput->header.hDevice, RIDI_PREPARSEDDATA, nullptr, &bufferSize );
	WriteToDebuggerOutput( ConvertIntegerToString( bufferSize ) );

	GetRawInputDeviceInfo( gamepadInput->header.hDevice, RIDI_PREPARSEDDATA, &hidClassBuffer, &bufferSize );
	//HIDP_PREPARSED_DATA* hidClassData = reinterpret_cast<RID_DEVICE_INFO*>( gamepadInfoBuffer );
	if( gamepadInfo->dwType != RIM_TYPEHID )
	{
		WriteToDebuggerOutput( "Whoops.\n" );
		return;
	}
	WriteToDebuggerOutput( "Vendor ID: " );
	WriteToDebuggerOutput( ConvertIntegerToString( gamepadInfo->hid.dwVendorId ) );
	WriteToDebuggerOutput( "\n" );
}

//-----------------------------------------------------------------------------------------------
void HandleRawInputEvent( WPARAM /*wParam*/, LPARAM lParam )
{
	static const size_t STORAGE_BUFFER_SIZE = 64;
	static BYTE inputBuffer[STORAGE_BUFFER_SIZE];

	//Determine the size of the buffer we will receive
	size_t inputBufferSize;
	GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, nullptr, &inputBufferSize, sizeof( RAWINPUTHEADER ) );

	FATAL_ASSERTION( inputBufferSize <= STORAGE_BUFFER_SIZE, "Raw Input Error", 
		"Raw input buffer size insufficient to store input data!!" );
	GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, inputBuffer, &inputBufferSize, sizeof( RAWINPUTHEADER ) );

	RAWINPUT* input = reinterpret_cast<RAWINPUT*>( inputBuffer );
	switch( input->header.dwType )
	{
	case RIM_TYPEHID: //Gamepad
			HandleRawGamepadInput( input );
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------------------------
void UpdateXInput()
{
	static const float STICK_MAX_INVERSE	= 1.f / 32768.f;
	static const float TRIGGER_MAX_INVERSE	= 1.f / 255.f;

	static DWORD lastXInputPacketNumber = 0;
	static DWORD xInputResult = 0;  
	static XINPUT_STATE xInputState;  

	static const DWORD MAX_XINPUT_USERS = 4;
	for ( DWORD i = 0; i < MAX_XINPUT_USERS; ++i )
	{
		ZeroMemory( &xInputState, sizeof( XINPUT_STATE ) );

		// Simply get the state of the controller from XInput.
		xInputResult = XInputGetState( i, &xInputState );

		if( xInputResult == ERROR_SUCCESS ) //Controller is connected
		{
			if( g_xinputToGamepadIDMapping[ i ] == Gamepad::ID_Null )
			{
				WriteToDebuggerOutput( "New controller attached." );
				g_xinputToGamepadIDMapping[ i ] = PeripheralInterface::ConnectNewGamepad( 6, 16 );
			}
			else if( xInputState.dwPacketNumber == lastXInputPacketNumber )
				continue;

			Gamepad* const& gamepad = PeripheralInterface::GetGamepad( g_xinputToGamepadIDMapping[ i ] );

			gamepad->currAxisState[ 0 ] = xInputState.Gamepad.sThumbLX		* STICK_MAX_INVERSE;
			gamepad->currAxisState[ 1 ] = xInputState.Gamepad.sThumbLY		* STICK_MAX_INVERSE;
			gamepad->currAxisState[ 2 ] = xInputState.Gamepad.sThumbRX		* STICK_MAX_INVERSE;
			gamepad->currAxisState[ 3 ] = xInputState.Gamepad.sThumbRY		* STICK_MAX_INVERSE;
			gamepad->currAxisState[ 4 ] = xInputState.Gamepad.bLeftTrigger	* TRIGGER_MAX_INVERSE;
			gamepad->currAxisState[ 5 ] = xInputState.Gamepad.bRightTrigger	* TRIGGER_MAX_INVERSE;

			gamepad->currButtonState[  0 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_A,				xInputState.Gamepad.wButtons );
			gamepad->currButtonState[  1 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_B,				xInputState.Gamepad.wButtons );
			gamepad->currButtonState[  2 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_Y,				xInputState.Gamepad.wButtons );
			gamepad->currButtonState[  3 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_X,				xInputState.Gamepad.wButtons );
			gamepad->currButtonState[  4 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_DPAD_DOWN,		xInputState.Gamepad.wButtons );
			gamepad->currButtonState[  5 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_DPAD_RIGHT,		xInputState.Gamepad.wButtons );
			gamepad->currButtonState[  6 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_DPAD_UP,			xInputState.Gamepad.wButtons );
			gamepad->currButtonState[  7 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_DPAD_LEFT,		xInputState.Gamepad.wButtons );
			gamepad->currButtonState[  8 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_BACK,			xInputState.Gamepad.wButtons );
			gamepad->currButtonState[  9 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_START,			xInputState.Gamepad.wButtons );
			gamepad->currButtonState[ 10 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_LEFT_SHOULDER,	xInputState.Gamepad.wButtons );
			gamepad->currButtonState[ 11 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_RIGHT_SHOULDER,	xInputState.Gamepad.wButtons );
			gamepad->currButtonState[ 12 ] = xInputState.Gamepad.bLeftTrigger	* TRIGGER_MAX_INVERSE;
			gamepad->currButtonState[ 13 ] = xInputState.Gamepad.bRightTrigger	* TRIGGER_MAX_INVERSE;
			gamepad->currButtonState[ 14 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_LEFT_THUMB,		xInputState.Gamepad.wButtons );
			gamepad->currButtonState[ 15 ] = (float)IsBitSetInMask( (WORD)XINPUT_GAMEPAD_RIGHT_THUMB,		xInputState.Gamepad.wButtons );

			lastXInputPacketNumber = xInputState.dwPacketNumber;
		}
		else //Controller isn't connected 
		{
			if( g_xinputToGamepadIDMapping[ i ] != Gamepad::ID_Null )
			{
				WriteToDebuggerOutput( "Xbox Controller detached." );
				PeripheralInterface::DisconnectGamepad( g_xinputToGamepadIDMapping[ i ] );
				g_xinputToGamepadIDMapping[ i ] = Gamepad::ID_Null;
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
void UpdateMousePosition()
{
	static POINT mouseScreenLocation;

	GetCursorPos( &mouseScreenLocation );
	PeripheralInterface::GetMouse()->SetScreenPosition( mouseScreenLocation.x, SCREEN_HEIGHT - mouseScreenLocation.y );
	ScreenToClient( g_hWnd, &mouseScreenLocation );
	PeripheralInterface::GetMouse()->SetWindowPosition( mouseScreenLocation.x, GAME_WINDOW_HEIGHT - mouseScreenLocation.y );
}

//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowsMessageHandlingProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	static const float MOUSE_WHEEL_SCALE = 0.00833f;
	bool wasProcessed = false;
	unsigned char asKey = (unsigned char) wParam;

	switch( wmMessageCode )
	{
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			g_isQuitting = true;
			return 0;

		
		//Keyboard
		case WM_KEYDOWN:
			if( asKey == VK_ESCAPE )
			{
				g_isQuitting = true;
				return 0;
			}
			else if( asKey == VK_F11 )
			{
				ToggleFullscreen();
			}
			else if( asKey == VK_F9 )
			{
				if( PeripheralInterface::GetMouse()->IsLockedToWindow() )
					PeripheralInterface::GetMouse()->UnlockFromWindow();
				else
					PeripheralInterface::GetMouse()->LockToWindow();
			}
			else
			{
				PeripheralInterface::GetKeyboard()->SetKeyDown( asKey );
				return wasProcessed;
			}
			break;
		case WM_KEYUP:
			PeripheralInterface::GetKeyboard()->SetKeyUp( asKey );
			return wasProcessed;
		

		//Mouse
		case WM_MOUSEWHEEL:
			PeripheralInterface::GetMouse()->SetWheelDeltaX( GET_WHEEL_DELTA_WPARAM( wParam ) * MOUSE_WHEEL_SCALE );
			return true;
		case WM_MOUSEHWHEEL:
			PeripheralInterface::GetMouse()->SetWheelDeltaY( GET_WHEEL_DELTA_WPARAM( wParam ) * MOUSE_WHEEL_SCALE );
			return true;
		case WM_LBUTTONDOWN:
			PeripheralInterface::GetMouse()->SetButtonState( Mouse::BUTTON_LEFT, true );
			return true;
		case WM_LBUTTONUP:
			PeripheralInterface::GetMouse()->SetButtonState( Mouse::BUTTON_LEFT, false );
			return true;
		case WM_MBUTTONDOWN:
			PeripheralInterface::GetMouse()->SetButtonState( Mouse::BUTTON_MIDDLE, true );
			return true;
		case WM_MBUTTONUP:
			PeripheralInterface::GetMouse()->SetButtonState( Mouse::BUTTON_MIDDLE, false );
			return true;
		case WM_RBUTTONDOWN:
			PeripheralInterface::GetMouse()->SetButtonState( Mouse::BUTTON_RIGHT, true );
			return true;
		case WM_RBUTTONUP:
			PeripheralInterface::GetMouse()->SetButtonState( Mouse::BUTTON_RIGHT, false );
			return true;
		case WM_XBUTTONDOWN:
			PeripheralInterface::GetMouse()->SetButtonState( static_cast< Mouse::Button >( GET_XBUTTON_WPARAM( wParam ) ), true );
			return true;
		case WM_XBUTTONUP:
			PeripheralInterface::GetMouse()->SetButtonState( static_cast< Mouse::Button >( GET_XBUTTON_WPARAM( wParam ) ), false );
			return true;
		

		//Raw Input
// 		case WM_INPUT:
// 			WriteToDebuggerOutput( "Received raw input.\n" );
// 			HandleRawInputEvent( wParam, lParam );
// 			return true;
// 		case WM_INPUT_DEVICE_CHANGE:
// 			WriteToDebuggerOutput( "A raw input device's state changed.\n" );
// 			return true;


		//Other
		default:
			break;
	}

	return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
}

//-----------------------------------------------------------------------------------------------
HWND CreateConsoleWindow( const char* windowTitle )
{
	AllocConsole();
	AttachConsole( GetCurrentProcessId() );
	freopen( "CON", "w", stdout );
	SetConsoleTitle( windowTitle );
	HWND consoleWindow = FindWindow( nullptr, windowTitle );

	SetWindowPos( consoleWindow, nullptr, 50, 50, 1000, 600, SWP_NOZORDER );
	return consoleWindow;
}

//-----------------------------------------------------------------------------------------------
void CreateOpenGLWindow( const char* windowTitle, HINSTANCE applicationInstanceHandle )
{
	// Define a window class
	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast< WNDPROC >( WindowsMessageHandlingProcedure ); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT( "Simple Window Class" );
	RegisterClassEx( &windowClassDescription );

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect( desktopWindowHandle, &desktopRect );

	RECT windowRect = { GAME_WINDOW_SCREEN_X_POS, GAME_WINDOW_SCREEN_Y_POS, 
						GAME_WINDOW_SCREEN_X_POS + GAME_WINDOW_WIDTH, GAME_WINDOW_SCREEN_X_POS + GAME_WINDOW_HEIGHT };
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );

	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		TEXT( windowTitle ),
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL );

	ShowWindow( g_hWnd, SW_SHOW );
	SetForegroundWindow( g_hWnd );
	SetFocus( g_hWnd );

	g_displayDeviceContext = GetDC( g_hWnd );

	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	SetCursor( cursor );

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset( &pixelFormatDescriptor, 0, sizeof( pixelFormatDescriptor ) );
	pixelFormatDescriptor.nSize			= sizeof( pixelFormatDescriptor );
	pixelFormatDescriptor.nVersion		= 1;
	pixelFormatDescriptor.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType	= PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits	= 24;
	pixelFormatDescriptor.cDepthBits	= 24;
	pixelFormatDescriptor.cAccumBits	= 0;
	pixelFormatDescriptor.cStencilBits	= 8;

	int pixelFormatCode = ChoosePixelFormat( g_displayDeviceContext, &pixelFormatDescriptor );
	SetPixelFormat( g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor );
	g_openGLRenderingContext = wglCreateContext( g_displayDeviceContext );
	wglMakeCurrent( g_displayDeviceContext, g_openGLRenderingContext );
}

//-----------------------------------------------------------------------------------------------
void RegisterForRawGamepadInput()
{
	static const unsigned int NUMBER_OF_RAW_INPUT_DEVICES = 2;
	RAWINPUTDEVICE rawInputDevices[NUMBER_OF_RAW_INPUT_DEVICES];

	rawInputDevices[0].usUsagePage = 0x01; //Generic Controls Page
	rawInputDevices[0].usUsage = 0x05; //Gamepad
	rawInputDevices[0].dwFlags = RIDEV_DEVNOTIFY; //Allows the receiving of INPUT_DEVICE_CHANGE events (connection/disconnection)
	rawInputDevices[0].hwndTarget = 0;

	rawInputDevices[1].usUsagePage = 0x01;
	rawInputDevices[1].usUsage = 0x04; //Joystick
	rawInputDevices[1].dwFlags = RIDEV_DEVNOTIFY;
	rawInputDevices[1].hwndTarget = 0;

	BOOL rawInputRegistrationResult = RegisterRawInputDevices( rawInputDevices, NUMBER_OF_RAW_INPUT_DEVICES, sizeof(rawInputDevices[0] ) );
	if( rawInputRegistrationResult != TRUE )
	{
		std::string rawInputErrorText( "Unable to register for raw input.\n Error Information: " );
		rawInputErrorText.append( "\nError Code: " );
		rawInputErrorText.append( ConvertIntegerToString( GetLastError() ) );
		rawInputErrorText.append( "\nError Text: " );
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
		rawInputErrorText.append( std::string( messageBuffer, size ) );
		RECOVERABLE_ERROR( "Raw Input Error", rawInputErrorText );
	}
};

//-----------------------------------------------------------------------------------------------
void RunMessagePump()
{
	MSG queuedMessage;
	for( ;; )
	{
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage );
	}
}

//-----------------------------------------------------------------------------------------------
void Update( double timeSpentLastFrameSeconds )
{
	static float deltaSeconds = 0.f;
	deltaSeconds = static_cast< float >( timeSpentLastFrameSeconds );

	UpdateMousePosition();
	UpdateXInput();
	GameInterface::Update( deltaSeconds );

	AudioInterface::Update( deltaSeconds );
	PeripheralInterface::Update( deltaSeconds );
}

//-----------------------------------------------------------------------------------------------
void Render()
{
	GameInterface::Render();

	SwapBuffers( g_displayDeviceContext );
}

//-----------------------------------------------------------------------------------------------
double WaitUntilDesiredFrameLengthThenReturnActualFrameLength( double frameStartTimeSeconds, double desiredFrameLengthSeconds )
{
	static double targetTime = 0;
	targetTime = frameStartTimeSeconds + desiredFrameLengthSeconds;
	double timeNow = GetCurrentTimeSeconds();

	while( timeNow < targetTime )
	{
		timeNow = GetCurrentTimeSeconds();
	}

	return timeNow - frameStartTimeSeconds;
}

//-----------------------------------------------------------------------------------------------
void RunFrame()
{
	static double frameStartTime = 0.0;
	static double timeSpentLastFrameSeconds = 0.0;

	frameStartTime = GetCurrentTimeSeconds();
	RunMessagePump();
	Update( timeSpentLastFrameSeconds );
	Render();
	GameInterface::EndOfFrame();

	timeSpentLastFrameSeconds = WaitUntilDesiredFrameLengthThenReturnActualFrameLength( frameStartTime, FRAME_RATE_LIMIT_SECONDS );
}

//-----------------------------------------------------------------------------------------------
void HandleCommandLineOptions( CommandLine::OptionList options )
{
	for( unsigned int i = 0; i < options.size(); ++i )
	{
		CommandLine::Option& option = options[ i ];
		if( option.option == "gw" || option.option == "gamewindow" )
		{
			if( option.arguments.size() != 4 )
			{
				CommandLine::Manager::ReportCommandError( "Incorrect number of arguments to gamewindow option.\nUsage: --gamewindow <Screen X Position> <Screen Y Position> <Window Width> <Window Height>\n" );
				return;
			}
			
			GAME_WINDOW_SCREEN_X_POS = ConvertStringToUnsignedInt( option.arguments[ 0 ] );
			GAME_WINDOW_SCREEN_Y_POS = ConvertStringToUnsignedInt( option.arguments[ 1 ] );
			GAME_WINDOW_WIDTH = ConvertStringToUnsignedInt( option.arguments[ 2 ] );
			GAME_WINDOW_HEIGHT = ConvertStringToUnsignedInt( option.arguments[ 3 ] );
		}
		else if( option.option == "help" || option.option == "h" || option.option == "?" )
		{
			printf( "-gw\t--gamewindow\t<ScreenXPos> <ScreenYPos> <WindowWidth> <WindowHeight>");
		}
	}
}

//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int )
{
	InitializeTimer();

	//Console window is created first so that command line errors may be written to it
	CreateConsoleWindow( "Vingine Console" );
	RegisterForRawGamepadInput();

	CommandLine::Manager::Create();

	CommandLine::Delegate mainCommandLineDelegate;
	mainCommandLineDelegate.Bind< &HandleCommandLineOptions >();
	CommandLine::Manager::AddObserver( mainCommandLineDelegate );
	CommandLine::Manager::RunCommandLine( commandLineString );

	if( !g_openConsole )
		FreeConsole();

	SCREEN_WIDTH = GetSystemMetrics( SM_CXVIRTUALSCREEN );
	SCREEN_HEIGHT = GetSystemMetrics( SM_CYVIRTUALSCREEN );

	GameInterface::BeforeEngineInitialization();

	CreateOpenGLWindow( APP_NAME, applicationInstanceHandle );

	EventCourier::Startup();
	RendererInterface::Startup();
	AudioInterface::Startup();
	PeripheralInterface::Startup();
	g_xinputToGamepadIDMapping[ 0 ] = Gamepad::ID_Null;
	g_xinputToGamepadIDMapping[ 1 ] = Gamepad::ID_Null;
	g_xinputToGamepadIDMapping[ 2 ] = Gamepad::ID_Null;
	g_xinputToGamepadIDMapping[ 3 ] = Gamepad::ID_Null;
	AssetInterface::Startup();

	GameInterface::BeforeFirstFrame( GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT );

	while( !g_isQuitting )	
	{
		RunFrame();
	}
	
	GameInterface::BeforeEngineDestruction();

	AssetInterface::Shutdown();
	PeripheralInterface::Shutdown();
	AudioInterface::Shutdown();
	RendererInterface::Shutdown();

	//TempClearShaderPrograms();

	EventCourier::Shutdown();
	CommandLine::Manager::Destroy();

	GameInterface::AfterEngineDestruction();

	if( g_openConsole )
		FreeConsole();

// #if defined( _WIN32 ) && defined( _DEBUG )
// 	assert( _CrtCheckMemory() );
// 	_CrtDumpMemoryLeaks();
// #endif

	return 0;
}
#endif
