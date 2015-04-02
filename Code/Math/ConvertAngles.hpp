#pragma once
#ifndef INCLUDED_CONVERT_ANGLES_HPP
#define INCLUDED_CONVERT_ANGLES_HPP

#include "EulerAngles.hpp"
#include "FloatVector3.hpp"

#pragma region Euler Angles
//-----------------------------------------------------------------------------------------------
inline void ConvertEulerAnglesToVector( const EulerAngles& angles, FloatVector3& out_vector )
{
	float pitchRadians = ConvertDegreesToRadians( angles.pitchDegreesAboutY );
	float yawRadians = ConvertDegreesToRadians( angles.yawDegreesAboutZ );
	out_vector.x = cos( yawRadians ) * cos( pitchRadians );
	out_vector.y = sin( yawRadians ) * cos( pitchRadians );
	out_vector.z = sin( pitchRadians );
}
#pragma endregion //Euler Angles

#endif //INCLUDED_CONVERT_ANGLES_HPP