#ifndef INCLUDED_FLOAT_VECTOR_4_HPP
#define INCLUDED_FLOAT_VECTOR_4_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "Vector4.hpp"

//-----------------------------------------------------------------------------------------------
typedef Vector4< float > FloatVector4;

//-----------------------------------------------------------------------------------------------
template<>
inline float FloatVector4::CalculateNorm() const
{
	return sqrt( CalculateSquaredNorm() );
}

//-----------------------------------------------------------------------------------------------
template<>
inline void FloatVector4::Normalize()
{
	float norm = this->CalculateNorm();
	if( norm == 0.f )
		return;

	x /= norm;
	y /= norm;
	z /= norm;

	ONLY_DURING_MATH_DEBUGGING( assert( AreAlmostEqual( this->CalculateNorm(), 1.f ) ) );
}

#endif //INCLUDED_FLOAT_VECTOR_4_HPP
