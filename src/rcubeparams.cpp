#include "all.h"

#include "rcubeparams.h"

using namespace RC;

std::map< RCAxis, Vector3D * > AxisParams::m_p = InitMap();

std::map< RCAxis, Vector3D * > AxisParams::InitMap()
{
	std::map< RCAxis, Vector3D * > mp;

	mp[ AX_FRONT ]	= new Vector3D(  0,  0,  1 );
	mp[ AX_BACK ] 	= new Vector3D(  0,  0, -1 );
	mp[ AX_UP ] 	= new Vector3D(  0,  1,  0 );
	mp[ AX_DOWN ] 	= new Vector3D(  0, -1,  0 );
	mp[ AX_RIGHT ] 	= new Vector3D(  1,  0,  0 );
	mp[ AX_LEFT ] 	= new Vector3D( -1,  0,  0 );

	return mp;
}

RCAxis AxisParams::getAxisForVector( const Vector3D & vec )
{
	for ( int i = AX_FIRST; i < AX_COUNT; ++i )
		if ( ( * m_p[ RCAxis( i ) ] ) == vec )
		{
			return RCAxis( i );
			break;
		}

	std::cout << "error AP";
	return AX_NONE;
}

void AxisParams::cleanup()
{
	for ( auto it = m_p.begin(); it != m_p.end(); ++it )
		delete it->second;
}

std::map< RCMoveType, MoveParams::OneParam * > MoveParams::m_p = InitMap();

std::map< RCMoveType, MoveParams::OneParam * > MoveParams::InitMap()
{
	std::map< RCMoveType, OneParam * > mp;
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

RCMoveType MoveParams::getMTypeForPars( const Vector3D & vec, const bool cw )
{
	for ( int i = MT_FIRST; i < MT_COUNT; ++i )
	{
		if ( MoveParams::vec( RCMoveType( i ) ) == vec && MoveParams::clockwise( RCMoveType( i ) ) == cw )
			return RCMoveType( i );
	}

	std::cout << "error MP";
	return MT_NONE;
}

void MoveParams::cleanup()
{
	for ( auto it = m_p.begin(); it != m_p.end(); ++it )
		delete it->second;
}

RCColorScheme Colors::m_schemeId = RCS_S2;
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

	return cp;
}

std::map< RCColorScheme, std::map< RCColor, Colors::OneColor * > > Colors::InitMapRC()
{
	std::map< RCColorScheme, std::map< RCColor, OneColor * > > cp;

	cp[ RCS_S1 ][ RC_FG ]	= new OneColor(  52,  73,  94 ); // Wet asphalt
	cp[ RCS_S1 ][ RC_BG ]	= new OneColor( 149, 165, 166 ); // Concrete

	cp[ RCS_S2 ][ RC_FG ]	= new OneColor( 229, 229, 229 );
	cp[ RCS_S2 ][ RC_BG ]	= new OneColor(  33,  33,  33 );

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
