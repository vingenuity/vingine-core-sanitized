#ifndef INCLUDED_TEXTURE_HPP
#define INCLUDED_TEXTURE_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include <string>
#include "../Math/FloatVector2.hpp"
#include "../Color.hpp"


//-----------------------------------------------------------------------------------------------
struct Texture
{
	//Texture Settings
	typedef unsigned short FilteringMethod;
	static const FilteringMethod FILTER_nearestNeighbor;
	static const FilteringMethod FILTER_linearInterpolation;

	typedef unsigned short WrappingMode;
	static const WrappingMode WRAP_None;
	static const WrappingMode WRAP_Repeat;
	static const WrappingMode WRAP_RepeatFlipped;


	//Constructor/Destructor
	Texture();
	~Texture();


	//Data Members
	std::string fileLocation;
	int widthPixels, heightPixels;
	unsigned int textureIDOnCard;
};



//-----------------------------------------------------------------------------------------------
inline Texture::Texture()
	: widthPixels( 0 )
	, heightPixels( 0 )
	, textureIDOnCard( 0 )
{ }

//-----------------------------------------------------------------------------------------------
inline Texture::~Texture()
{ }
#endif //INCLUDED_TEXTURE_HPP
