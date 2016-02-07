#include "all.h"

#include "rcubeparams.h"
#include <map>
#include <tuple>

using namespace RC;

// RotAxis params
const std::map< const RA, const glm::vec3 > p_RAPar {
	{ RA::X, glm::vec3( 1.0f, 0.0f, 0.0f ) },
	{ RA::Y, glm::vec3( 0.0f, 1.0f, 0.0f ) },
	{ RA::Z, glm::vec3( 0.0f, 0.0f, 1.0f ) }
};

glm::vec3 RC::RAPar::vec( const RA ra )
{
	auto src = p_RAPar.find( ra );

	if ( src != p_RAPar.end() )
		return src->second;

	throw std::logic_error( "RAPar::vec()" );
	return glm::vec3( 0.0f );
}

RA RC::RAPar::closestRA( glm::vec3 vec )
{
    const float aX = std::abs( vec.x );
	const float aY = std::abs( vec.y );
	const float aZ = std::abs( vec.z );

    if ( aX > aY && aX > aZ )
		return RA::X;
	else if ( aY > aX && aY > aZ )
		return RA::Y;
	else if ( aZ > aX && aZ > aY )
		return RA::Z;

	throw std::logic_error( "RAPar::closestRA()" );
	return RA::NONE;
}

// RotateType params
typedef std::pair< const RA, const bool > RTPair;

const std::map< const RT, const RTPair > p_RTPar {
	{ RT::UP, 		std::make_pair( RA::X, true ) },
	{ RT::DOWN, 	std::make_pair( RA::X, false ) },
	{ RT::LEFT, 	std::make_pair( RA::Y, true ) },
	{ RT::RIGHT,	std::make_pair( RA::Y, false ) },
	{ RT::ACW, 		std::make_pair( RA::Z, true ) },
	{ RT::CW, 		std::make_pair( RA::Z, false ) }
};

glm::quat RC::RTPar::quat( const RT rt )
{
	auto src = p_RTPar.find( rt );

	if ( src != p_RTPar.end() )
	{
		const glm::vec3 vec = RAPar::vec( src->second.first );
		const bool cw = src->second.second;
		const float angle = glm::radians( 90.0f );

		return glm::angleAxis( cw ? angle : -angle, vec );
	}

	throw std::logic_error( "RTPar::quat()" );
	return glm::quat();
}

RT RC::RTPar::equalRT( const RA ra, const bool cw )
{
	const RTPair pars{ ra, cw };

	for ( auto it : p_RTPar )
		if ( it.second == pars )
			return it.first;

	throw std::logic_error( "RTPar::equalRT()" );
	return RT::NONE;
}

// MoveType params
const int ll = CUBIE_COUNT - 1;
typedef std::tuple< const RA, const int, const bool > MTTuple;

const std::map< const MT, const MTTuple > p_MTPar {
	{ MT::F, 	std::make_tuple( RA::Z, ll, true ) },
	{ MT::FI,	std::make_tuple( RA::Z, ll, false ) },
	{ MT::B, 	std::make_tuple( RA::Z, 0, false ) },
	{ MT::BI,	std::make_tuple( RA::Z, 0, true ) },
	{ MT::R, 	std::make_tuple( RA::X, ll, true ) },
	{ MT::RI,	std::make_tuple( RA::X, ll, false ) },
	{ MT::L, 	std::make_tuple( RA::X, 0, false ) },
	{ MT::LI,	std::make_tuple( RA::X, 0, true ) },
	{ MT::U, 	std::make_tuple( RA::Y, ll, true ) },
	{ MT::UI,	std::make_tuple( RA::Y, ll, false ) },
	{ MT::D, 	std::make_tuple( RA::Y, 0, false ) },
	{ MT::DI, 	std::make_tuple( RA::Y, 0, true ) },
	{ MT::FM, 	std::make_tuple( RA::Z, -1, true ) },
	{ MT::FMI, 	std::make_tuple( RA::Z, -1, false ) },
	{ MT::UM, 	std::make_tuple( RA::Y, -1, true ) },
	{ MT::UMI,	std::make_tuple( RA::Y, -1, false ) },
	{ MT::RM, 	std::make_tuple( RA::X, -1, true ) },
	{ MT::RMI, 	std::make_tuple( RA::X, -1, false ) }
};

glm::vec3 RC::MTPar::vec( const MT mt )
{
	auto src = p_MTPar.find( mt );

	if ( src != p_MTPar.end() )
		return RAPar::vec( std::get< 0 >( src->second ) );

	throw std::logic_error( "MTPar::vec()" );
	return glm::vec3( 0.0f );
}

RA RC::MTPar::axis( const MT mt )
{
	auto src = p_MTPar.find( mt );

	if ( src != p_MTPar.end() )
		return std::get< 0 >( src->second );

	throw std::logic_error( "MTPar::axis()" );
	return RA::NONE;
}

int RC::MTPar::layer( const MT mt )
{
	auto src = p_MTPar.find( mt );

	if ( src != p_MTPar.end() )
		return std::get< 1 >( src->second );

	throw std::logic_error( "MTPar::layer()" );
	return -1;
}

bool RC::MTPar::clockwise( const MT mt )
{
	auto src = p_MTPar.find( mt );

	if ( src != p_MTPar.end() )
		return std::get< 2 >( src->second );

	throw std::logic_error( "MTPar::clockwise()" );
	return false;
}

MT RC::MTPar::equalMT( const RA ra, const int lay, const bool cw )
{
	const int lay1 = ( 0 < lay && lay < CUBIE_COUNT - 1 ) ? -1 : lay;
	const MTTuple pars{ ra, lay1, cw };

	for ( auto it : p_MTPar )
		if ( it.second == pars )
			return it.first;

	throw std::logic_error( "MTPar::equalMT()" );
	return MT::NONE;
}

//
MT RC::GKPar::toMT( const GK gk )
{
	switch ( gk )
	{
	case GK::MOVEF:		return MT::F;
	case GK::MOVEFI:	return MT::FI;
	case GK::MOVEB:		return MT::B;
	case GK::MOVEBI:	return MT::BI;
	case GK::MOVEL:		return MT::L;
	case GK::MOVELI:	return MT::LI;
	case GK::MOVER:		return MT::R;
	case GK::MOVERI:	return MT::RI;
	case GK::MOVEU:		return MT::U;
	case GK::MOVEUI:	return MT::UI;
	case GK::MOVED:		return MT::D;
	case GK::MOVEDI:	return MT::DI;
	case GK::MOVEFM:	return MT::FM;
	case GK::MOVEFMI:	return MT::FMI;
	case GK::MOVEUM:	return MT::UM;
	case GK::MOVEUMI:	return MT::UMI;
	case GK::MOVERM:	return MT::RM;
	case GK::MOVERMI:	return MT::RMI;
	default: return MT::NONE;
	}
}

GK RC::GKPar::fromMT( const MT mt )
{
	switch ( mt )
	{
	case MT::F:		return GK::MOVEF;
	case MT::FI:	return GK::MOVEFI;
	case MT::B:		return GK::MOVEB;
	case MT::BI:	return GK::MOVEBI;
	case MT::L:		return GK::MOVEL;
	case MT::LI:	return GK::MOVELI;
	case MT::R:		return GK::MOVER;
	case MT::RI:	return GK::MOVERI;
	case MT::U:		return GK::MOVEU;
	case MT::UI:	return GK::MOVEUI;
	case MT::D:		return GK::MOVED;
	case MT::DI:	return GK::MOVEDI;
	case MT::FM:	return GK::MOVEFM;
	case MT::FMI:	return GK::MOVEFMI;
	case MT::UM:	return GK::MOVEUM;
	case MT::UMI:	return GK::MOVEUMI;
	case MT::RM:	return GK::MOVERM;
	case MT::RMI:	return GK::MOVERMI;

	default: return GK::NONE;
	}
}

RT RC::GKPar::toRT( const GK gk )
{
	switch ( gk )
	{
	case GK::ROTATEUP: 	return RT::UP;
	case GK::ROTATEDOWN: return RT::DOWN;
	case GK::ROTATELEFT: return RT::LEFT;
	case GK::ROTATERIGHT:return RT::RIGHT;
	case GK::ROTATECW: 	return RT::CW;
	case GK::ROTATEACW: 	return RT::ACW;

	default: return RT::NONE;
	}
}

GK RC::GKPar::fromRT( const RT rt )
{
	switch ( rt )
	{
	case RT::UP:		return GK::ROTATEUP;
	case RT::DOWN:		return GK::ROTATEDOWN;
	case RT::LEFT:		return GK::ROTATELEFT;
	case RT::RIGHT:		return GK::ROTATERIGHT;
	case RT::CW:		return GK::ROTATECW;
	case RT::ACW:		return GK::ROTATEACW;

	default: return GK::NONE;
	}
}

RC::GK RC::GKPar::prevGK( const RC::GK gk )
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
	case RC::GK::MOVEFM:	return RC::GK::MOVEFMI;
	case RC::GK::MOVEFMI:	return RC::GK::MOVEFM;
	case RC::GK::MOVEUM:	return RC::GK::MOVEUMI;
	case RC::GK::MOVEUMI:	return RC::GK::MOVEUM;
	case RC::GK::MOVERM:	return RC::GK::MOVERMI;
	case RC::GK::MOVERMI:	return RC::GK::MOVERM;

	default: return RC::GK::NONE;
	}
}

const std::map< const GK, const std::string > p_GKPar {
	{ GK::MOVEF, 	"F" },
	{ GK::MOVEFI, 	"F'" },
	{ GK::MOVEB, 	"B" },
	{ GK::MOVEBI, 	"B'" },
	{ GK::MOVEL, 	"L" },
	{ GK::MOVELI, 	"L'" },
	{ GK::MOVER, 	"R" },
	{ GK::MOVERI, 	"R'" },
	{ GK::MOVEU, 	"U" },
	{ GK::MOVEUI, 	"U'" },
	{ GK::MOVED, 	"D" },
	{ GK::MOVEDI, 	"D'" },
	{ GK::MOVEFM, 	"f" },
	{ GK::MOVEFMI, 	"f'" },
	{ GK::MOVEUM, 	"u" },
	{ GK::MOVEUMI, 	"u'" },
	{ GK::MOVERM, 	"r" },
	{ GK::MOVERMI, 	"r'" },

	{ GK::ROTATEUP,  	"rtU" },
	{ GK::ROTATEDOWN,  	"rtD" },
	{ GK::ROTATELEFT,  	"rtL" },
	{ GK::ROTATERIGHT,	"rtR" },
	{ GK::ROTATECW,		"rtCW" },
	{ GK::ROTATEACW, 	"rtACW" }
};

std::string RC::GKPar::str( const GK gk )
{
	auto src = p_GKPar.find( gk );

	if ( src != p_GKPar.end() )
		return src->second;

	throw std::logic_error( "GKPar::str()" );
	return "";
}
