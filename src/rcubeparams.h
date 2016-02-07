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

		inline bool enableWithMove( const RC::GK gk )
		{
			if ( ( RC::GKPar::toMT( gk ) == RC::MT::NONE )
					&& gk != RC::GK::CUBEMIX && gk != RC::GK::CUBERESET && gk != RC::GK::CUBEUNDO )
				return true;
			else return false;
		}

		inline RC::GK prevGK( const RC::GK gk )
		{
			switch( gk )
			{
			case RC::GK::ROTATEDOWN: 	return RC::GK::ROTATEUP;
			case RC::GK::ROTATEUP: 		return RC::GK::ROTATEDOWN;
			case RC::GK::ROTATELEFT: 	return RC::GK::ROTATERIGHT;
			case RC::GK::ROTATERIGHT: 	return RC::GK::ROTATELEFT;
			case RC::GK::ROTATECW: 		return RC::GK::ROTATEACW;
			case RC::GK::ROTATEACW: 	return RC::GK::ROTATECW;

			case RC::GK::MOVEB: 	return RC::GK::MOVEBI;
			case RC::GK::MOVEBI: 	return RC::GK::MOVEB;
			case RC::GK::MOVED: 	return RC::GK::MOVEDI;
			case RC::GK::MOVEDI: 	return RC::GK::MOVED;
			case RC::GK::MOVEF: 	return RC::GK::MOVEFI;
			case RC::GK::MOVEFI: 	return RC::GK::MOVEF;
			case RC::GK::MOVEL: 	return RC::GK::MOVELI;
			case RC::GK::MOVELI: 	return RC::GK::MOVEL;
			case RC::GK::MOVER: 	return RC::GK::MOVERI;
			case RC::GK::MOVERI: 	return RC::GK::MOVER;
			case RC::GK::MOVEU:		return RC::GK::MOVEUI;
			case RC::GK::MOVEUI:	return RC::GK::MOVEU;

			default: return RC::GK::NONE;
			}
		}
	}

/*	class MTPrev
	{
	public:
		typedef std::
	private:


		std::stack<   > m_stack;	// previous keys
	}*/
}
#endif // RCUBEPARAMS_H
