#include "rcubeparams.h"

using namespace RC;

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
