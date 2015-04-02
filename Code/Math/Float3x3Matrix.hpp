#ifndef INCLUDED_FLOAT_3X3_MATRIX_HPP
#define INCLUDED_FLOAT_3X3_MATRIX_HPP
#pragma once

//-----------------------------------------------------------------------------------------------
#include "FloatVector3.hpp"
#include "Matrix.hpp"

//-----------------------------------------------------------------------------------------------
typedef Matrix< float, 3, 3 > Float3x3Matrix;

//-----------------------------------------------------------------------------------------------
static const float F3X3_IDENTITY_ARRAY[ 9 ] = {
	1.f, 0.f, 0.f, 
	0.f, 1.f, 0.f, 
	0.f, 0.f, 1.f };

static const Float3x3Matrix F3X3_IDENTITY_MATRIX = Float3x3Matrix( F3X3_IDENTITY_ARRAY );

//-----------------------------------------------------------------------------------------------
inline FloatVector3 operator*( const Float3x3Matrix& matrix, const FloatVector3& vector )
{
	FloatVector3 result;

	result.x = ( matrix( 1, 1 ) * vector.x ) + ( matrix( 1, 2 ) * vector.y ) + ( matrix( 1, 3 ) * vector.z );
	result.y = ( matrix( 2, 1 ) * vector.x ) + ( matrix( 2, 2 ) * vector.y ) + ( matrix( 2, 3 ) * vector.z );
	result.z = ( matrix( 3, 1 ) * vector.x ) + ( matrix( 3, 2 ) * vector.y ) + ( matrix( 3, 3 ) * vector.z );

	return result;
}

#endif //INCLUDED_FLOAT_3X3_MATRIX_HPP
