#pragma once
#ifndef INCLUDED_COLOR_CONSTANTS_HPP
#define INCLUDED_COLOR_CONSTANTS_HPP

//-----------------------------------------------------------------------------------------------
/*
	The purpose of this file is to provide a set of common color constants that seem to appear often in the codebase.
	At the moment, the standard RGB and CMYK colors are here, as well as a transparent value.
*/
//-----------------------------------------------------------------------------------------------
#include "Color.hpp"

//-----------------------------------------------------------------------------------------------
static const Color COLOR_Black		(   0,   0,   0, 255 );
static const Color COLOR_Blue		(   0,   0, 255, 255 );
static const Color COLOR_Clear		(   0,   0,   0,   0 );
static const Color COLOR_Cyan		(   0, 255, 255, 255 );
static const Color COLOR_Green		(   0, 255,   0, 255 );
static const Color COLOR_Magenta	( 255,   0, 255, 255 );
static const Color COLOR_Red		( 255,   0,   0, 255 );
static const Color COLOR_Yellow		( 255, 255,   0, 255 );
static const Color COLOR_White		( 255, 255, 255, 255 );

#endif // INCLUDED_COLOR_CONSTANTS_HPP
