#include "all.h"

#include "rcubeparams.h"

using namespace RC;

const glm::vec3 RC::RCDetail::m_RAPar[ RA_COUNT ] = {
	glm::vec3( 1.0f, 0.0f, 0.0f ),
	glm::vec3( 0.0f, 1.0f, 0.0f ),
	glm::vec3( 0.0f, 0.0f, 1.0f ) };

glm::vec3 RC::RAPar::getVecForRA( const RotAxis ra )
{
	return RCDetail::m_RAPar[ ra ];
}

RotAxis RC::RAPar::getClosestAxis( glm::vec3 vec )
{
    const float aX = std::abs( vec.x );
	const float aY = std::abs( vec.y );
	const float aZ = std::abs( vec.z );

    if ( aX > aY && aX > aZ )
		return RC::RA_X;
	else if ( aY > aX && aY > aZ )
		return RC::RA_Y;
	else if ( aZ > aX && aZ > aY )
		return RC::RA_Z;
	else return RC::RA_NONE;
}

// MoveParams
std::map< MoveType, MoveParams::OneParam * > MoveParams::m_p = InitMap();

std::map< MoveType, MoveParams::OneParam * > MoveParams::InitMap()
{
	const int ll = CUBIE_COUNT - 1;
	std::map< MoveType, OneParam * > mp;

	mp[ MT_FRONT ] 		= new OneParam( RA_Z, ll, true );
	mp[ MT_FRONTINV ] 	= new OneParam( RA_Z, ll, false );
	mp[ MT_BACK ] 		= new OneParam( RA_Z, 0, false );
	mp[ MT_BACKINV ] 	= new OneParam( RA_Z, 0, true );
	mp[ MT_RIGHT ] 		= new OneParam( RA_X, ll, true );
	mp[ MT_RIGHTINV ] 	= new OneParam( RA_X, ll, false );
	mp[ MT_LEFT ] 		= new OneParam( RA_X, 0, false );
	mp[ MT_LEFTINV ] 	= new OneParam( RA_X, 0, true );
	mp[ MT_UP ] 		= new OneParam( RA_Y, ll, true );
	mp[ MT_UPINV ] 		= new OneParam( RA_Y, ll, false );
	mp[ MT_DOWN ] 		= new OneParam( RA_Y, 0, false );
	mp[ MT_DOWNINV ] 	= new OneParam( RA_Y, 0, true );
	mp[ MT_FRONTMID ] 	= new OneParam( RA_Z, -1, true );
	mp[ MT_FRONTMIDINV ]= new OneParam( RA_Z, -1, false );
	mp[ MT_UPMID ]		= new OneParam( RA_Y, -1, true );
	mp[ MT_UPMIDINV ]	= new OneParam( RA_Y, -1, false );
	mp[ MT_RIGHTMID ] 	= new OneParam( RA_X, -1, true );
	mp[ MT_RIGHTMIDINV ]= new OneParam( RA_X, -1, false );

	return mp;
}

MoveType MoveParams::getMTypeForPars( const glm::vec3 & vec, const bool cw, const int lay )
{
	const RotAxis ra = RAPar::getClosestAxis( vec );
	const int lay1 = ( 0 < lay && lay < CUBIE_COUNT - 1 ) ? -1 : lay;

	for ( int i = MT_FIRST; i < MT_LAST + 1; ++i )
	{
		const RotAxis ra2 = m_p[ MoveType( i ) ]->m_RA;
		const bool cw2 = m_p[ MoveType( i ) ]->m_clockwise;
		const int lay2 = m_p[ MoveType( i ) ]->m_layer;

		if ( ra2 == ra && cw2 == cw && lay2 == lay1 )
			return MoveType( i );
	}

	std::cout << "error MT";

	return MT_NONE;
}

/*
RCMoveType MoveParams::getMTypeForPars( const RCAxis ax, const bool cw )
{
	for ( int i = MT_FIRST; i < MT_COUNT; ++i )
	{
		if ( m_p[ RCMoveType( i ) ]->m_axis == ax && m_p[ RCMoveType( i ) ]->m_clockwise == cw )
			return RCMoveType( i );
	}

	std::cout << "error MP";
	return MT_NONE;
}
*/
void MoveParams::cleanup()
{
	for ( auto it = m_p.begin(); it != m_p.end(); ++it )
		delete it->second;
}
