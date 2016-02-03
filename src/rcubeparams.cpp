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
/*
RCColorScheme Colors::m_schemeId = RCS_S1;
std::map< RCColorScheme, std::map< RCAxis, Colors::OneColor * > > Colors::m_ax = InitMapAX();
std::map< RCColorScheme, std::map< RCColor, Colors::OneColor * > > Colors::m_c = InitMapRC();

std::map< RCColorScheme, std::map< RCAxis, Colors::OneColor * > > Colors::InitMapAX()
{
	std::map< RCColorScheme, std::map< RCAxis, Colors::OneColor * > > cp;

	cp[ RCS_S1 ][ AX_FRONT ]	= new OneColor(  52, 152, 219 ); // Blue - Peter river
	cp[ RCS_S1 ][ AX_UP ]		= new OneColor( 236, 240, 241 ); // White - Clouds
	cp[ RCS_S1 ][ AX_BACK ]		= new OneColor(  46, 204, 113 ); // Green - Emerald
	cp[ RCS_S1 ][ AX_DOWN ]		= new OneColor( 241, 196,  15 ); // Yellow - Sun flower
	cp[ RCS_S1 ][ AX_LEFT ]		= new OneColor( 231,  76,  60 ); // Red - Alizarin
	cp[ RCS_S1 ][ AX_RIGHT ]	= new OneColor( 230, 126,  34 ); // Orange - Carrot

	cp[ RCS_S2 ][ AX_FRONT ]	= new OneColor(  61,  96, 195 ); // Blue
	cp[ RCS_S2 ][ AX_UP ]		= new OneColor( 249, 249, 249 ); // White
	cp[ RCS_S2 ][ AX_BACK ]		= new OneColor( 157, 218,  93 ); // Green
	cp[ RCS_S2 ][ AX_DOWN ]		= new OneColor( 209, 182,  71 ); // Yellow
	cp[ RCS_S2 ][ AX_LEFT ]		= new OneColor( 220,  42,  59 ); // Red
	cp[ RCS_S2 ][ AX_RIGHT ]	= new OneColor( 213, 120,  76 ); // Orange


	cp[ RCS_S3 ][ AX_FRONT ]	= new OneColor(   0,   0, 255 ); // Blue
	cp[ RCS_S3 ][ AX_UP ]		= new OneColor( 255, 255, 255 ); // White
	cp[ RCS_S3 ][ AX_BACK ]		= new OneColor(   0, 220,   0 ); // Green
	cp[ RCS_S3 ][ AX_DOWN ]		= new OneColor( 255, 235,   0 ); // Yellow
	cp[ RCS_S3 ][ AX_LEFT ]		= new OneColor( 255,   0,   0 ); // Red
	cp[ RCS_S3 ][ AX_RIGHT ]	= new OneColor( 255, 128,   0 ); // Orange

	return cp;
}

std::map< RCColorScheme, std::map< RCColor, Colors::OneColor * > > Colors::InitMapRC()
{
	std::map< RCColorScheme, std::map< RCColor, OneColor * > > cp;

	cp[ RCS_S1 ][ RC_FG ]	= new OneColor(  52,  73,  94 ); // Wet asphalt
	cp[ RCS_S1 ][ RC_BG ]	= new OneColor( 149, 165, 166 ); // Concrete

	cp[ RCS_S2 ][ RC_FG ]	= new OneColor( 229, 229, 229 );
	cp[ RCS_S2 ][ RC_BG ]	= new OneColor(  33,  33,  33 );

	cp[ RCS_S3 ][ RC_FG ]	= new OneColor(   0,   0,   0 );
	cp[ RCS_S3 ][ RC_BG ]	= new OneColor( 120, 110, 120 );

	return cp;
}

void Colors::cleanup()
{
	for ( auto it = m_c.begin(); it != m_c.end(); ++it )
		for ( auto it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			delete it2->second;

	for ( auto it = m_ax.begin(); it != m_ax.end(); ++it )
		for ( auto it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			delete it2->second;
}
*/
