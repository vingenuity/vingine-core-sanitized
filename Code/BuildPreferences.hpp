#pragma once
#ifndef INCLUDED_BUILD_PREFERENCES_HPP
#define INCLUDED_BUILD_PREFERENCES_HPP

#include "EngineMacros.hpp"


#pragma region Required Engine Preferences
//-----------------------------------------------------------------------------------------------
// 0.0 means no limit
static const double FRAME_RATE_LIMIT_FPS = 0.0;

// APP_NAME is used for things like title bars on some platforms
static const char* APP_NAME = "Artifact";
#pragma endregion // Required Engine Preferences



#pragma region Platform Interface Preferences
//-----------------------------------------------------------------------------------------------
//Audio Interface Options:
//#define AUDIO_INTERFACE_USE_NULL
//#define AUDIO_INTERFACE_USE_FMOD
//#define AUDIO_INTERFACE_USE_OPENAL
//#define AUDIO_INTERFACE_USE_OPENSL_ES10

//Renderer Interface Options:
//#define RENDERER_INTERFACE_USE_NULL
//#define RENDERER_INTERFACE_USE_OPENGL
//#define RENDERER_INTERFACE_USE_OPENGL_ES2
//#define RENDERER_INTERFACE_USE_WEBGL

//Shader Loader Options:
//#define SHADER_LOADER_USING_CG
//#define SHADER_LOADER_USING_GLSL
//#define SHADER_LOADER_USING_HLSL

#if defined( PLATFORM_ANDROID )
	#define AUDIO_INTERFACE_USE_OPENSL_ES10
	#define RENDERER_INTERFACE_USE_OPENGL_ES2
	#define SHADER_LOADER_USING_GLSL
#elif defined( PLATFORM_HTML5 )
	#define AUDIO_INTERFACE_USE_OPENAL
	#define RENDERER_INTERFACE_USE_OPENGL_ES2
	#define SHADER_LOADER_USING_GLSL
#elif defined( PLATFORM_WINDOWS )
	#define AUDIO_INTERFACE_USE_FMOD
	#define RENDERER_INTERFACE_USE_OPENGL
	#define SHADER_LOADER_USING_CG
#elif defined( PLATFORM_PS3 )
	#define AUDIO_INTERFACE_USE_FMOD
	//#define RENDERER_INTERFACE_USE_NULL
	#define SHADER_LOADER_USING_CG
#elif defined( PLATFORM_VITA )
	#define AUDIO_INTERFACE_USE_FMOD
	#define RENDERER_INTERFACE_USE_NULL
	#define SHADER_LOADER_USING_NULL
#endif // defined( PLATFORM_MACROS )
#pragma endregion // Platform Interface Preferences

// Have to use conditional operator here for this to remain static const...Oh well.
static const double FRAME_RATE_LIMIT_SECONDS = ( FRAME_RATE_LIMIT_FPS == 0.0 ) ? 0.0 : 1.0 / FRAME_RATE_LIMIT_FPS;
#endif //INCLUDED_BUILD_PREFERENCES_HPP
