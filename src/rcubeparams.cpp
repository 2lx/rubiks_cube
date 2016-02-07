#include "all.h"

#include "rcubeparams.h"
#include <map>
#include <tuple>

using namespace RC;

// RotAxis params
const std::map< const RotAxis, const glm::vec3 > m_RAPar {
	{ RA_X, glm::vec3( 1.0f, 0.0f, 0.0f ) },
	{ RA_Y, glm::vec3( 0.0f, 1.0f, 0.0f ) },
	{ RA_Z, glm::vec3( 0.0f, 0.0f, 1.0f ) }
};

glm::vec3 RC::RAPar::vec( const RotAxis ra )
{
	auto src = m_RAPar.find( ra );

	if ( src != m_RAPar.end() )
		return src->second;

	std::cout << "RAPar: error vec()" << std::endl;
	return glm::vec3( 0.0f );
}

RotAxis RC::RAPar::closestRA( glm::vec3 vec )
{
    const float aX = std::abs( vec.x );
	const float aY = std::abs( vec.y );
	const float aZ = std::abs( vec.z );

    if ( aX > aY && aX > aZ )
		return RA_X;
	else if ( aY > aX && aY > aZ )
		return RA_Y;
	else if ( aZ > aX && aZ > aY )
		return RA_Z;

	std::cout << "RAPar: error closestRA()" << std::endl;
	return RA_NONE;
}

// MoveType params
const int ll = CUBIE_COUNT - 1;
typedef std::tuple< const RotAxis, const int, const bool > MTTuple;

const std::map< const MoveType, const MTTuple > p_MTPar {
	{ MT_FRONT, 		std::make_tuple( RA_Z, ll, true ) },
	{ MT_FRONTINV, 		std::make_tuple( RA_Z, ll, false ) },
	{ MT_BACK, 			std::make_tuple( RA_Z, 0, false ) },
	{ MT_BACKINV, 		std::make_tuple( RA_Z, 0, true ) },
	{ MT_RIGHT, 		std::make_tuple( RA_X, ll, true ) },
	{ MT_RIGHTINV, 		std::make_tuple( RA_X, ll, false ) },
	{ MT_LEFT, 			std::make_tuple( RA_X, 0, false ) },
	{ MT_LEFTINV, 		std::make_tuple( RA_X, 0, true ) },
	{ MT_UP, 			std::make_tuple( RA_Y, ll, true ) },
	{ MT_UPINV, 		std::make_tuple( RA_Y, ll, false ) },
	{ MT_DOWN, 			std::make_tuple( RA_Y, 0, false ) },
	{ MT_DOWNINV, 		std::make_tuple( RA_Y, 0, true ) },
	{ MT_FRONTMID, 		std::make_tuple( RA_Z, -1, true ) },
	{ MT_FRONTMIDINV, 	std::make_tuple( RA_Z, -1, false ) },
	{ MT_UPMID, 		std::make_tuple( RA_Y, -1, true ) },
	{ MT_UPMIDINV, 		std::make_tuple( RA_Y, -1, false ) },
	{ MT_RIGHTMID, 		std::make_tuple( RA_X, -1, true ) },
	{ MT_RIGHTMIDINV, 	std::make_tuple( RA_X, -1, false ) }
};

glm::vec3 RC::MTPar::vec( const MoveType mt )
{
	auto src = p_MTPar.find( mt );

	if ( src != p_MTPar.end() )
		return RAPar::vec( std::get< 0 >( src->second ) );

	std::cout << "MTPar: error vec()" << std::endl;
	return glm::vec3( 0.0f );
}

RotAxis RC::MTPar::axis( const MoveType mt )
{
	auto src = p_MTPar.find( mt );

	if ( src != p_MTPar.end() )
		return std::get< 0 >( src->second );

	std::cout << "MTPar: error axis()" << std::endl;
	return RA_NONE;
}

int RC::MTPar::layer( const MoveType mt )
{
	auto src = p_MTPar.find( mt );

	if ( src != p_MTPar.end() )
		return std::get< 1 >( src->second );

	std::cout << "MTPar: error layer()" << std::endl;
	return -1;
}

bool RC::MTPar::clockwise( const MoveType mt )
{
	auto src = p_MTPar.find( mt );

	if ( src != p_MTPar.end() )
		return std::get< 2 >( src->second );

	std::cout << "MTPar: error clockwise()" << std::endl;
	return false;
}

MoveType RC::MTPar::equalMT( const RotAxis ra, const int lay, const bool cw )
{
	const int lay1 = ( 0 < lay && lay < CUBIE_COUNT - 1 ) ? -1 : lay;
	const MTTuple pars{ ra, lay1, cw };

	for ( auto it : p_MTPar )
		if ( it.second == pars )
			return it.first;

	std::cout << "MTPar: error equalMT()" << std::endl;
	return MT_NONE;
}
