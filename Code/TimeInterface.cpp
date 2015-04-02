#include "TimeInterface.hpp"

#include "AssertionError.hpp"
#include "EngineMacros.hpp"
#include "PlatformSpecificHeaders.hpp"


#pragma region Windows Time Functions
#if defined( PLATFORM_WINDOWS )
//----------------------------------------------------------------------------------------------------
static double g_secondsPerCount = 0.0;

//----------------------------------------------------------------------------------------------------
double GetCurrentTimeSeconds()
{
	FATAL_ASSERTION( g_secondsPerCount != 0.0, "Time Interface Error",
		"Unable to determine the relationship between the system clock and real time.\nThis can occur if InitializeTimer() hasn't been called.\n" );

	LARGE_INTEGER performanceCount;
	QueryPerformanceCounter( &performanceCount );

	double timeSeconds = static_cast< double >( performanceCount.QuadPart ) * g_secondsPerCount;
	return timeSeconds;
}

//----------------------------------------------------------------------------------------------------
void InitializeTimer()
{
	if( g_secondsPerCount != 0.0 )
		return;

	LARGE_INTEGER countsPerSecond;
	QueryPerformanceFrequency( &countsPerSecond );
	g_secondsPerCount = 1.0 / static_cast< double >( countsPerSecond.QuadPart );
}
#endif //defined( PLATFORM_WINDOWS )
#pragma endregion



#pragma region Linux-Style Time Functions
#if defined( PLATFORM_ANDROID ) || defined( PLATFORM_HTML5 )
//----------------------------------------------------------------------------------------------------
#include <time.h>

//----------------------------------------------------------------------------------------------------
double GetCurrentTimeSeconds()
{
	static const double NANOSECONDS_TO_SECONDS = 0.000000001;
	timespec currentTime;
	clock_gettime( CLOCK_MONOTONIC, &currentTime );

	double timeSeconds = static_cast< double >( currentTime.tv_sec ) + 
		( static_cast< double >( currentTime.tv_nsec ) * NANOSECONDS_TO_SECONDS );
	return timeSeconds;
}

//----------------------------------------------------------------------------------------------------
void InitializeTimer()
{
	//No special conversions are needed for clock_gettime
}
#endif // defined( PLATFORM_ANDROID ) || defined( PLATFORM_HTML5 )
#pragma endregion // Linux-Style Time Functions



#pragma region PS3 Time Functions
#if defined( PLATFORM_PS3 )
//----------------------------------------------------------------------------------------------------
#include <time.h> //this is mostly here just for NULL
#include <sys/sys_time.h>

//----------------------------------------------------------------------------------------------------
double GetCurrentTimeSeconds()
{
	static const double NANOSECONDS_TO_SECONDS = 0.000000001;
	static sys_time_sec_t secondsSinceEpoch = 0;
	static sys_time_nsec_t nanosecondsSinceLastSecond = 0;

	sys_time_get_current_time( &secondsSinceEpoch, &nanosecondsSinceLastSecond );

	double timeSeconds = static_cast< double >( secondsSinceEpoch ) + 
		( static_cast< double >( nanosecondsSinceLastSecond ) * NANOSECONDS_TO_SECONDS );
	return timeSeconds;
}

//----------------------------------------------------------------------------------------------------
void InitializeTimer()
{
	//No special conversions are needed for clock_gettime
}
#endif // defined( PLATFORM_PS3 )
#pragma endregion // PS3 Time Functions



#pragma region Vita Time Functions
#if defined( PLATFORM_VITA )
//----------------------------------------------------------------------------------------------------
#include <kernel.h>

//----------------------------------------------------------------------------------------------------
static SceUID g_performanceTimerID = 0;

//----------------------------------------------------------------------------------------------------
double GetCurrentTimeSeconds()
{
	static SceKernelSysClock performanceCount;
	sceKernelGetTimerTime( g_performanceTimerID, &performanceCount );

	static const double SECONDS_PER_COUNT = 0.000001; //The SDK claims that quad is in microseconds
	double timeSeconds = static_cast< double >( performanceCount.quad ) * SECONDS_PER_COUNT;
	return timeSeconds;
}

//----------------------------------------------------------------------------------------------------
void InitializeTimer()
{
	// None of these parameters really matter because we are not using this timer to sync threads.
	g_performanceTimerID = sceKernelCreateTimer( "vingine-performance-timer", SCE_KERNEL_ATTR_TH_FIFO, nullptr );
	FATAL_ASSERTION( g_performanceTimerID >= 0, "Time Interface Error", "Unable to create performance timer.\n" );

	int returnValue = sceKernelStartTimer( g_performanceTimerID );
	FATAL_ASSERTION( returnValue == 0, "Time Interface Error", "Unable to start performance timer.\n" );
}
#endif // defined( PLATFORM_VITA )
#pragma endregion // Vita Time Functions
