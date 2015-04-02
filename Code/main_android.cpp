#include "EngineMacros.hpp"

#ifdef PLATFORM_ANDROID

#include "PlatformSpecificHeaders.hpp"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/asset_manager.h>
#include "Android/android_native_app_glue.h"
#include "Input/nv-ndk-gamepad/nv_gamepad_jni.h"

#include "Math/FloatVector2.hpp"
#include "Bitmasking.hpp"
#include "DebuggerInterface.hpp"
#include "StringConversion.hpp"

#include "Audio/AudioInterface.hpp"
#include "Events/EventCourier.hpp"
#include "Graphics/RendererInterface.hpp"
#include "Input/PeripheralInterface.hpp"
#include "AssetInterface.hpp"
#include "GameInterface.hpp"
#include "TimeInterface.hpp"



static const double LOCKED_FRAME_RATE_SECONDS = 1.0 / 60.0;

/**
 * Shared state for our app.
 */
struct engine {
    struct android_app* app;
	JNIEnv* appThreadEnv;
	jobject appThreadThis;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
	bool isLoaded;

	//Gamepad stuff
	NvGamepadAxis* m_axes;
	int m_axisCount;

	int* m_buttons;
	int m_buttonCount;
};

//This function is taken from the android JNI samples
//For some odd reason, this isn't in an includable directory
const char* EGLstrerror(EGLint err)
{
	switch (err)
	{
	case EGL_SUCCESS:           return "EGL_SUCCESS";
	case EGL_NOT_INITIALIZED:   return "EGL_NOT_INITIALIZED";
	case EGL_BAD_ACCESS:        return "EGL_BAD_ACCESS";
	case EGL_BAD_ALLOC:         return "EGL_BAD_ALLOC";
	case EGL_BAD_ATTRIBUTE:     return "EGL_BAD_ATTRIBUTE";
	case EGL_BAD_CONFIG:        return "EGL_BAD_CONFIG";
	case EGL_BAD_CONTEXT:       return "EGL_BAD_CONTEXT";
	case EGL_BAD_CURRENT_SURFACE: return "EGL_BAD_CURRENT_SURFACE";
	case EGL_BAD_DISPLAY:       return "EGL_BAD_DISPLAY";
	case EGL_BAD_MATCH:         return "EGL_BAD_MATCH";
	case EGL_BAD_NATIVE_PIXMAP: return "EGL_BAD_NATIVE_PIXMAP";
	case EGL_BAD_NATIVE_WINDOW: return "EGL_BAD_NATIVE_WINDOW";
	case EGL_BAD_PARAMETER:     return "EGL_BAD_PARAMETER";
	case EGL_BAD_SURFACE:       return "EGL_BAD_SURFACE";
	case EGL_CONTEXT_LOST:      return "EGL_CONTEXT_LOST";
	default: return "UNKNOWN";
	}
}

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
	const EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	if( surface == EGL_NO_SURFACE )
	{
		EGLint errorCode = eglGetError();
		std::string errorText( "Unable to create EGL surface.\nError Code: " );
		errorText.append( ConvertIntegerToString( errorCode ) );
		errorText.append( "\nError Text: " );
		errorText.append( EGLstrerror( errorCode ) );
		FATAL_ERROR( "Window Creation Error", errorText );
	}

	const EGLint ctx_attribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	context = eglCreateContext( display, config, EGL_NO_CONTEXT, ctx_attribs );
	if( context == EGL_NO_CONTEXT )
	{
		EGLint errorCode = eglGetError();
		std::string errorText( "Unable to create EGL context.\nError Code: " );
		errorText.append( ConvertIntegerToString( errorCode ) );
		errorText.append( "\nError Text: " );
		errorText.append( EGLstrerror( errorCode ) );
		FATAL_ERROR( "Window Creation Error", errorText );
	}

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        WriteToDebuggerOutput( "Unable to eglMakeCurrent" );
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    return 0;
}

//-----------------------------------------------------------------------------------------------
void StartUpVingine( struct engine& engine )
{
	GameInterface::BeforeEngineInitialization();

	EventCourier::Startup();
	RendererInterface::Startup();
	AudioInterface::Startup();
	PeripheralInterface::Startup();

	PeripheralInterface::ConnectNewGamepad( 4, 16 );

	//engine.m_axisCount = 0;
	//engine.m_buttonCount = 0;
	//engine.m_axes = NvGetGamepadAxes( engine.appThreadEnv, engine.appThreadThis, engine.m_axisCount);
	//engine.m_buttons = NvGetGamepadButtons( engine.appThreadEnv, engine.appThreadThis, engine.m_buttonCount);

	AssetInterface::Startup( static_cast< void* >( engine.app->activity->assetManager ) );

	GameInterface::BeforeFirstFrame( engine.width, engine.height );
}

//-----------------------------------------------------------------------------------------------
void ShutDownVingine()
{
	GameInterface::BeforeEngineDestruction();

	AssetInterface::Shutdown();
	PeripheralInterface::Shutdown();
	AudioInterface::Shutdown();
	RendererInterface::Shutdown();
	EventCourier::Shutdown();

	GameInterface::AfterEngineDestruction();
}

//-----------------------------------------------------------------------------------------------
static void HandleApplicationEvent( struct android_app* app, int32_t eventType )
{
	struct engine* engine = (struct engine*)app->userData;

    switch ( eventType )
	{
		case APP_CMD_START:
			break;

		case APP_CMD_RESUME:
			break;

		//We are about to show the window for the first time
		case APP_CMD_INIT_WINDOW:
			if (engine->app->window != NULL)
			{
				engine_init_display( engine );
			}
            break;

		//Our window has received direct focus
		case APP_CMD_GAINED_FOCUS:
			StartUpVingine( *engine );
			engine->isLoaded = true;
			break;

			//Save our state for later (this means we are probably about to suspend)
		case APP_CMD_SAVE_STATE:
			break;

		//Our window has lost direct focus
		case APP_CMD_LOST_FOCUS:
			ShutDownVingine();
			engine->isLoaded = false;
			break;

			//We are about to lose the window (due to a suspend or close)
		case APP_CMD_TERM_WINDOW:
			break;

		case APP_CMD_PAUSE:
			break;

		case APP_CMD_STOP:
			break;
    }
}

//-----------------------------------------------------------------------------------------------
static void HandleGamepadAxisEvent( AInputEvent* eventData, struct engine* engine )
{
	/*AFAIK, the Android NDK doesn't tell us what axis has been changed.
	 * Instead, we have to just refresh all of them. WHEE!!!!
	 */
	Gamepad* gamepad = PeripheralInterface::GetGamepadAtIndex( 0 );

	gamepad->currAxisState[ 0 ] = -AMotionEvent_getAxisValue( eventData, AMOTION_EVENT_AXIS_X, 0 );
	//X value is Android is inverted from how Vingine likes it
	gamepad->currAxisState[ 1 ] = AMotionEvent_getAxisValue( eventData, AMOTION_EVENT_AXIS_Y, 0 );

	gamepad->currAxisState[ 2 ] = -AMotionEvent_getAxisValue( eventData, AMOTION_EVENT_AXIS_RX, 0 );
	//X value is Android is inverted from how Vingine likes it
	gamepad->currAxisState[ 3 ] = AMotionEvent_getAxisValue( eventData, AMOTION_EVENT_AXIS_RY, 0 );

	WriteToDebuggerOutput( "Gamepad Axis Event!" );
}

//-----------------------------------------------------------------------------------------------
static void HandleGamepadButtonEvent( AInputEvent* eventData, struct engine* engine )
{
	Gamepad* gamepad = PeripheralInterface::GetGamepadAtIndex( 0 );
	bool isButtonDownEvent = ( AKeyEvent_getAction( eventData ) == AKEY_EVENT_ACTION_DOWN );

	int32_t androidButtonID = AKeyEvent_getKeyCode( eventData );
	Gamepad::ButtonID vingineButtonID = 0;

	switch( androidButtonID )
	{
	case AKEYCODE_BUTTON_A:
		vingineButtonID = 0;
		break;
	case AKEYCODE_BUTTON_B:
		vingineButtonID = 1;
		break;
	case AKEYCODE_BUTTON_Y:
		vingineButtonID = 2;
		break;
	case AKEYCODE_BUTTON_X:
		vingineButtonID = 3;
		break;
	case AKEYCODE_DPAD_DOWN:
		vingineButtonID = 4;
		break;
	case AKEYCODE_DPAD_RIGHT:
		vingineButtonID = 5;
		break;
	case AKEYCODE_DPAD_UP:
		vingineButtonID = 6;
		break;
	case AKEYCODE_DPAD_LEFT:
		vingineButtonID = 7;
		break;
	case AKEYCODE_BUTTON_SELECT:
		vingineButtonID = 8;
		break;
	case AKEYCODE_BUTTON_START:
		vingineButtonID = 9;
		break;
	case AKEYCODE_BUTTON_L1:
		vingineButtonID = 10;
		break;
	case AKEYCODE_BUTTON_R1:
		vingineButtonID = 11;
		break;
	case AKEYCODE_BUTTON_L2:
		vingineButtonID = 12;
		// On some platforms, the 2nd level "triggers" are also axes.
		gamepad->currAxisState[ 5 ] = (float)isButtonDownEvent;
		break;
	case AKEYCODE_BUTTON_R2:
		vingineButtonID = 13;
		gamepad->currAxisState[ 6 ] = (float)isButtonDownEvent;
		break;
	case AKEYCODE_BUTTON_THUMBL:
		vingineButtonID = 14;
		break;
	case AKEYCODE_BUTTON_THUMBR:
		vingineButtonID = 15;
		break;
	default:
		return;
	}

	gamepad->currButtonState[ vingineButtonID ] = (float)isButtonDownEvent;
}

//-----------------------------------------------------------------------------------------------
static void HandleKeyboardEvent( AInputEvent* eventData, struct engine* engine )
{
	if( AKeyEvent_getAction( eventData ) == AKEY_EVENT_ACTION_DOWN )
	{
		int32_t key_val = AKeyEvent_getKeyCode( eventData );
		PeripheralInterface::GetKeyboard()->SetKeyDown( key_val );
	}
	else if( AKeyEvent_getAction( eventData ) == AKEY_EVENT_ACTION_UP )
	{
		int32_t key_val = AKeyEvent_getKeyCode( eventData );
		PeripheralInterface::GetKeyboard()->SetKeyUp( key_val );
	}
}

//-----------------------------------------------------------------------------------------------
static void HandleTouchEvent( AInputEvent* eventData, struct engine* engine )
{
	unsigned int numberOfTouches = AMotionEvent_getPointerCount( eventData );

	std::string touchEvent( "Received Touch Event. Number of touches: " );
	touchEvent.append( ConvertIntegerToString( numberOfTouches ) );
	WriteToDebuggerOutput( touchEvent );

	TouchScreen* primaryTouchscreen = PeripheralInterface::GetTouchscreen();
	int systemTouchID = -1;
	float touchX = 0.f;
	float touchY = 0.f;
	int androidTouchType = -1;
	TouchType vingineTouchType = TOUCH_None;

	for( unsigned int touchIndex = 0; touchIndex < numberOfTouches; ++touchIndex )
	{
		systemTouchID = AMotionEvent_getPointerId( eventData, touchIndex );
		touchX = AMotionEvent_getX( eventData, touchIndex );
		touchY = engine->height - AMotionEvent_getY( eventData, touchIndex );

		//The android touch type is masked in the same integer as the index
		androidTouchType = AMotionEvent_getAction( eventData ) && AMOTION_EVENT_ACTION_MASK;

		switch( androidTouchType )
		{
		case AMOTION_EVENT_ACTION_DOWN:
			vingineTouchType = TOUCH_Began;
			break;
		case AMOTION_EVENT_ACTION_UP:
			vingineTouchType = TOUCH_Ended;
			break;
		case AMOTION_EVENT_ACTION_MOVE:
			vingineTouchType = TOUCH_Moved;
			break;
		case AMOTION_EVENT_ACTION_CANCEL:
			vingineTouchType = TOUCH_Cancelled;
			break;
		default:
			vingineTouchType = TOUCH_None;
			break;
		}
		primaryTouchscreen->AddOrUpdateTouchPoint( systemTouchID, touchX, touchY, vingineTouchType );
	}
}



//-----------------------------------------------------------------------------------------------
static int32_t HandleInputEvent( struct android_app* app, AInputEvent* eventData )
{
	struct engine* engine = (struct engine*)app->userData;

	// A "motion" event is any event that is analog -- touchscreen, gamepad axis, etc.
	if ( AInputEvent_getType( eventData ) == AINPUT_EVENT_TYPE_MOTION )
	{
		switch( AInputEvent_getSource( eventData ) )
		{
		case AINPUT_SOURCE_TOUCHSCREEN:
			HandleTouchEvent( eventData, engine );
			return 1;
		case AINPUT_SOURCE_GAMEPAD:
		case AINPUT_SOURCE_JOYSTICK:
			HandleGamepadAxisEvent( eventData, engine );
			return 1;
		default:
			break;
		}
	}
	else if ( AInputEvent_getType( eventData ) == AINPUT_EVENT_TYPE_KEY )
	{
		int32_t sourceType = AInputEvent_getSource( eventData );

		switch( AInputEvent_getSource( eventData ) )
		{
		case AINPUT_SOURCE_KEYBOARD:
			HandleKeyboardEvent( eventData, engine );
			return 1;
		case AINPUT_SOURCE_GAMEPAD:
		case AINPUT_SOURCE_JOYSTICK:
		case 0x501: // AINPUT_SOURCE_JOYSTICK and AINPUT_SOURCE_KEYBOARD and AINPUT_SOURCE_CLASS_BUTTON
			HandleGamepadButtonEvent( eventData, engine );
			return 1;
		default:
			break;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------------------------
void PumpMessageQueue( struct android_app* state )
{
	int looperID;
	int fdesc;
	int events;
	struct android_poll_source* source;

	static const unsigned int DO_NOT_WAIT_FOR_EVENTS = 0;
	while( ( looperID = ALooper_pollAll( DO_NOT_WAIT_FOR_EVENTS, &fdesc, &events, (void**)&source ) ) >= 0 )
	{
		// process this event
		if ( source )
			source->process( state, source );
	}
}

//-----------------------------------------------------------------------------------------------
void Update( double timeSpentLastFrameSeconds )
{
	static float deltaSeconds = 0.f;
	deltaSeconds = static_cast< float >( timeSpentLastFrameSeconds );

	GameInterface::Update( deltaSeconds );
	AudioInterface::Update( deltaSeconds );
	PeripheralInterface::Update( deltaSeconds );
}

//-----------------------------------------------------------------------------------------------
void Render( struct engine& engine )
{
	GameInterface::Render();

	eglSwapBuffers( engine.display, engine.surface );
}

//-----------------------------------------------------------------------------------------------
double WaitUntilNextFrameThenGiveFrameTime()
{
	static double targetTime = 0;
	targetTime = GetCurrentTimeSeconds() + LOCKED_FRAME_RATE_SECONDS;
	double timeNow = GetCurrentTimeSeconds();

	while( timeNow < targetTime )
	{
		timeNow = GetCurrentTimeSeconds();
	}

	return LOCKED_FRAME_RATE_SECONDS;
}

//-----------------------------------------------------------------------------------------------
void RunFrame( struct engine& engine )
{
	static double timeSpentLastFrameSeconds = 0.0;

	PumpMessageQueue( engine.app );
	if( engine.isLoaded )
	{
		Update( timeSpentLastFrameSeconds );
		Render( engine );
		GameInterface::EndOfFrame();
	}

	timeSpentLastFrameSeconds = WaitUntilNextFrameThenGiveFrameTime();
}

//-----------------------------------------------------------------------------------------------
/* android_main is the defined entry point for Android NDK apps that are using the glue. */
void android_main( struct android_app* state )
{
	app_dummy(); //Prevents the linker from discarding the native app glue

	state->onAppCmd = HandleApplicationEvent;
	state->onInputEvent = HandleInputEvent;

	struct engine engine;
	engine.isLoaded = false;
	state->userData = &engine;
	engine.app = state;

//	engine.appThreadThis = (state->activity->env)->NewGlobalRef( state->activity->clazz );
//
//	int error = (state->activity->vm)->AttachCurrentThread(&engine.appThreadEnv, nullptr);
//	if (error)
//		engine.appThreadEnv = nullptr;

	while( true )
	{
		RunFrame( engine );
	}
}
#endif //PLATFORM_ANDROID
