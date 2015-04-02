#ifndef INCLUDED_ENGINE_MACROS_HPP
#define INCLUDED_ENGINE_MACROS_HPP

#define VARIABLE_IS_UNUSED(x) (void)(x);



//Keywords
#define ABSTRACT
#define SINGLETON
#define STATIC 
#define VIRTUAL




//Conditional Execution
//#define MATH_DEBUG
#ifdef MATH_DEBUG
#define ONLY_DURING_MATH_DEBUGGING( x ) x
#else
#define ONLY_DURING_MATH_DEBUGGING( x )
#endif

//#define DEBUG_PHYSICS
#ifdef DEBUG_PHYSICS
#define ONLY_DURING_PHYSICS_DEBUGGING( x ) x
#else
#define ONLY_DURING_PHYSICS_DEBUGGING( x )
#endif



//Platform Specific
#if defined( __EMSCRIPTEN__ )
	#define ARCHITECTURE_32BIT
	#define PLATFORM_HTML5
#elif defined( __PS3__ )
	#define ARCHITECTURE_32BIT
	#define PLATFORM_PS3
#elif defined( __psp2__ )
	#define ARCHITECTURE_32BIT
	#define PLATFORM_VITA
#elif defined( ANDROID ) || defined( __ANDROID__ )
	#define ARCHITECTURE_32BIT
	#define PLATFORM_ANDROID
#elif defined( _WIN32 )
	#define ARCHITECTURE_32BIT
	#define PLATFORM_WINDOWS
#elif defined( _WIN64 )
	#define ARCHITECTURE_64BIT
	#define PLATFORM_WINDOWS
#endif

#if defined( PLATFORM_ANDROID ) || defined( PLATFORM_VITA )
#define NO_RETURN __attribute__ ((noreturn))
#elif defined( PLATFORM_PS3 )
#define NO_RETURN __attribute__ ((noreturn))
#define nullptr NULL
#else
#define NO_RETURN __declspec( noreturn )
#endif



//Compiler Specific
#if   defined( _MSC_VER )
	#define COMPILER_MICROSOFT_C
	#define COMPILER_MICROSOFT_C_VERSION _MSC_VER
	#define FORCED_INLINE __forceinline
#elif defined( __clang__ )
	#define COMPILER_CLANG
	#define FORCED_INLINE __attribute__((always_inline))
#else
	#define COMPILER_UNKNOWN
	#define FORCED_INLINE
#endif


//Debugger Specific
#if defined( PLATFORM_PS3 ) && defined( SN_TARGET_PS3 )
	#define DEBUGGER_PS3_PRODG
#endif

#endif //INCLUDED_ENGINE_MACROS_HPP
