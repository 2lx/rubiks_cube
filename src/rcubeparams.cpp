#include "all.h"

#include "rcubeparams.h"

using namespace RC;

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
/*
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
*/
void AxisParams::cleanup()
{
	for ( auto it = m_p.begin(); it != m_p.end(); ++it )
		delete it->second;
}

std::map< RCMoveType, MoveParams::OneParam * > MoveParams::m_p = InitMap();

std::map< RCMoveType, MoveParams::OneParam * > MoveParams::InitMap()
{
	std::map< RCMoveType, OneParam * > mp;
	mp[ MT_FRONT ] 		= new OneParam(  0,  0,  1, AX_FRONT, true );
	mp[ MT_FRONTINV ] 	= new OneParam(  0,  0,  1, AX_FRONT, false );
	mp[ MT_BACK ] 		= new OneParam(  0,  0, -1, AX_BACK, true );
	mp[ MT_BACKINV ] 	= new OneParam(  0,  0, -1, AX_BACK, false );
	mp[ MT_RIGHT ] 		= new OneParam(  1,  0,  0, AX_RIGHT, true );
	mp[ MT_RIGHTINV ] 	= new OneParam(  1,  0,  0, AX_RIGHT, false );
	mp[ MT_LEFT ] 		= new OneParam( -1,  0,  0, AX_LEFT, true );
	mp[ MT_LEFTINV ] 	= new OneParam( -1,  0,  0, AX_LEFT, false );
	mp[ MT_UP ] 		= new OneParam(  0,  1,  0, AX_UP, true );
	mp[ MT_UPINV ] 		= new OneParam(  0,  1,  0, AX_UP, false );
	mp[ MT_DOWN ] 		= new OneParam(  0, -1,  0, AX_DOWN, true );
	mp[ MT_DOWNINV ] 	= new OneParam(  0, -1,  0, AX_DOWN, false );

	return mp;
}

RCMoveType MoveParams::getMTypeForPars( const glm::vec3 & vec, const bool cw )
{
	for ( int i = MT_FIRST; i < MT_COUNT; ++i )
	{
		const glm::vec3 vec2 = MoveParams::vec( RCMoveType( i ) );
		const bool cw2 = MoveParams::clockwise( RCMoveType( i ) );
		if ( std::abs( glm::length( vec2 - vec ) ) < 0.001 && cw2 == cw )
			return RCMoveType( i );
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
