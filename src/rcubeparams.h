#ifndef RCUBEPARAMS_H
#define RCUBEPARAMS_H

#include <glm/gtc/type_ptr.hpp>

namespace RC
{
	namespace RAPar
	{
		glm::vec3 vec( const RA ra );
		RA closestRA( glm::vec3 vec );
	}

	namespace RTPar
	{
		glm::quat quat( const RT rt );

		RT equalRT( const RA ra, const bool cw );
	}

	namespace MTPar
	{
		inline bool isMT( const int val ) {
			if( 0 < val && val < ( int ) RC::MT::COUNT )
				return true;
			else return false;
		};

		glm::vec3 vec( const MT mt );
		RA axis( const MT mt );
		bool clockwise( const MT mt );
		int layer( const MT mt );

		MT equalMT( const RA ra, const int lay, const bool cw );
	}

	namespace GKPar
	{
		MT toMT( const GK gk );
		RT toRT( const GK gk );
		GK fromMT( const MT mt );
		GK fromRT( const RT rt );

		inline bool enableWithMove( const RC::GK gk )
		{
			if ( ( RC::GKPar::toMT( gk ) == RC::MT::NONE )
					&& gk != RC::GK::GAMEMIX && gk != RC::GK::GAMERESET && gk != RC::GK::GAMEUNDO )
				return true;
			else return false;
		}

		RC::GK prevGK( const RC::GK gk );
	}
}
#endif // RCUBEPARAMS_H
