#include "DebuggerInterface.hpp"

#include <cstdarg>
#include <stdio.h>

#include "PlatformSpecificHeaders.hpp"
#if defined( DEBUGGER_PS3_PRODG )
	#include <sn/LibSN.h>
#endif 


//-----------------------------------------------------------------------------------------------
bool DebuggerIsAttached()
{
	//-----------------------------------------------------------------------------------------------
	//Windows debugger detection code written by Squirrel Eiserloh
#if defined( PLATFORM_WINDOWS )
	typedef BOOL (CALLBACK IsDebugFuncType)();

	// Get a handle to KERNEL32.DLL
	HINSTANCE hInstanceKernel32 = GetModuleHandle( TEXT( "KERNEL32" ) );
	if( !hInstanceKernel32 )
		return false;

	// Get a handle to the IsDebuggerPresent() function in KERNEL32.DLL
	IsDebugFuncType* isDebuggerPresentFunc = (IsDebugFuncType*) GetProcAddress( hInstanceKernel32, "IsDebuggerPresent" );
	if( !isDebuggerPresentFunc )
		return false;

	// Now CALL that function and return its result
	BOOL isDebuggerAvailable = isDebuggerPresentFunc();
	return( isDebuggerAvailable == TRUE );



	//-----------------------------------------------------------------------------------------------
#elif defined( PLATFORM_HTML5 )
	// This works by trying to define the debugger symbol; if we can't define it, that means that a debugger must exist.
	if( EM_ASM_INT_V( { var debuggerIsKeyword = false; try { new Function("var debugger;"); } catch(e) { debuggerIsKeyword = true; } return debuggerIsKeyword;} ) == 1 )
		return true;
	return false;



	//-----------------------------------------------------------------------------------------------
#elif defined( PLATFORM_ANDROID )
	// Haven't found a way to do this yet.
	return false;



	//-----------------------------------------------------------------------------------------------
#elif defined( PLATFORM_PS3 )
	#if defined( DEBUGGER_PS3_PRODG )
		return ( snIsDebuggerPresent() == 1 );
	#endif



	//-----------------------------------------------------------------------------------------------
#elif defined( PLATFORM_VITA )
	// Haven't found a way to do this yet.
	return false;



	//-----------------------------------------------------------------------------------------------
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------------------------
int PrintfToDebuggerOutput( const char* format, ... )
{
#if defined( PLATFORM_PS3 ) || defined( PLATFORM_VITA )
	int charactersPrinted = 0;

	va_list variableArgumentPointer;
	va_start( variableArgumentPointer, format );
	charactersPrinted = vprintf( format, variableArgumentPointer );
	va_end( variableArgumentPointer );

	return charactersPrinted;
#else
	VARIABLE_IS_UNUSED( format );
	return 0;
#endif
}

//-----------------------------------------------------------------------------------------------
void WriteToDebuggerOutput( const std::string& string )
{
#if defined( PLATFORM_WINDOWS )
	OutputDebugStringA( string.c_str() );
#elif defined( PLATFORM_HTML5 )
	emscripten_log( EM_LOG_CONSOLE, "%s", string.c_str() );
#elif defined( PLATFORM_ANDROID )
	__android_log_print( ANDROID_LOG_INFO, "vingine-game", string.c_str() );
#elif defined( PLATFORM_PS3 ) || defined( PLATFORM_VITA )
	printf( string.c_str() );
#else
	return;
#endif
}

//-----------------------------------------------------------------------------------------------
void SetBreakpoint()
{
#if defined( PLATFORM_WINDOWS )
	__debugbreak();
#elif defined( PLATFORM_HTML5 )
	EM_ASM({debugger;});
#elif defined( PLATFORM_ANDROID ) || defined( PLATFORM_PS3 ) || defined( PLATFORM_VITA )
	__builtin_trap();
#else
	return;
#endif
}

//-----------------------------------------------------------------------------------------------
std::string BuildVisualStudioClickableString( const std::string& filename, const std::string& lineNumber, const std::string& errorText )
{
	std::string visualStudioString( filename );
	visualStudioString += "(";
	visualStudioString += lineNumber;
	visualStudioString += ") : ";
	visualStudioString += errorText;
	visualStudioString += "\n";

	return visualStudioString;
}
