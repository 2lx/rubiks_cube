#ifndef RCUBEPARAMS_H
#define RCUBEPARAMS_H

#include <map>
#include <glm/gtc/type_ptr.hpp>

namespace RC
{
	namespace RAPar
	{
		glm::vec3 vec( const RotAxis ra );
		RotAxis closestRA( glm::vec3 vec );
	}

	namespace RTPar
	{
		glm::quat quat( const RotateType rt );

		RotateType equalRT( const RotAxis ra, const bool cw );
	}

	namespace MTPar
	{
		glm::vec3 vec( const MoveType mt );
		RotAxis axis( const MoveType mt );
		bool clockwise( const MoveType mt );
		int layer( const MoveType mt );

		MoveType equalMT( const RotAxis ra, const int lay, const bool cw );
	}
}
#endif // RCUBEPARAMS_H
