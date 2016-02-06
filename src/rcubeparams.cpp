#include "all.h"

#include "rcubeparams.h"

using namespace RC;

const glm::vec3 RC::RAPar[ RA_COUNT ] = { glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f } };

glm::vec3 getVecForRA( const RotAxis ra )
{
	return RAPar[ ra ];
//	return glm::vec3( 1.0f );
}

/*
std::map< RCAxis, AxisParams::AxisParam * > AxisParams::m_p = InitMap();

std::map< RCAxis, AxisParams::AxisParam * > AxisParams::InitMap()
{
	std::map< RCAxis, AxisParam * > mp;

	mp[ AX_FRONT ]	= new AxisParam(  0,  0,  1 );
	mp[ AX_BACK ] 	= new AxisParam(  0,  0, -1 );
	mp[ AX_UP ] 	= new AxisParam(  0,  1,  0 );
	mp[ AX_DOWN ] 	= new AxisParam(  0, -1,  0 );
	mp[ AX_RIGHT ] 	= new AxisParam(  1,  0,  0 );
	mp[ AX_LEFT ] 	= new AxisParam( -1,  0,  0 );

	return mp;
}

RCAxis AxisParams::getAxisForVector( const glm::vec3 & vec )
{
	for ( int i = AX_FIRST; i < AX_COUNT; ++i )
		if ( m_p[ RCAxis( i ) ]->m_vec == vec )
		{
			return RCAxis( i );
			break;
		}

	std::cout << "error AP vec";
	return AX_NONE;
}

void AxisParams::cleanup()
{
	for ( auto it = m_p.begin(); it != m_p.end(); ++it )
		delete it->second;
}
*/
std::map< MoveType, MoveParams::OneParam * > MoveParams::m_p = InitMap();

std::map< MoveType, MoveParams::OneParam * > MoveParams::InitMap()
{
	std::map< MoveType, OneParam * > mp;
	mp[ MT_FRONT ] 		= new OneParam(  0,  0,  1, true );
	mp[ MT_FRONTINV ] 	= new OneParam(  0,  0,  1, false );
	mp[ MT_BACK ] 		= new OneParam(  0,  0, -1, true );
	mp[ MT_BACKINV ] 	= new OneParam(  0,  0, -1, false );
	mp[ MT_RIGHT ] 		= new OneParam(  1,  0,  0, true );
	mp[ MT_RIGHTINV ] 	= new OneParam(  1,  0,  0, false );
	mp[ MT_LEFT ] 		= new OneParam( -1,  0,  0, true );
	mp[ MT_LEFTINV ] 	= new OneParam( -1,  0,  0, false );
	mp[ MT_UP ] 		= new OneParam(  0,  1,  0, true );
	mp[ MT_UPINV ] 		= new OneParam(  0,  1,  0, false );
	mp[ MT_DOWN ] 		= new OneParam(  0, -1,  0, true );
	mp[ MT_DOWNINV ] 	= new OneParam(  0, -1,  0, false );

	return mp;
}

MoveType MoveParams::getMTypeForPars( const glm::vec3 & vec, const bool cw )
{
	for ( int i = MT_FIRST; i < MT_LAST + 1; ++i )
	{
		const glm::vec3 vec2 = MoveParams::vec( MoveType( i ) );
		const bool cw2 = MoveParams::clockwise( MoveType( i ) );
		if ( std::abs( glm::length( vec2 - vec ) ) < 0.001 && cw2 == cw )
			return MoveType( i );
	}

	std::cout << "error MP";
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
