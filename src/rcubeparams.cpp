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

	std::cout << "RAPar: error vec()" << std::endl;
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

	std::cout << "RAPar: error closestRA()" << std::endl;
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

	std::cout << "RTPar: error quat()" << std::endl;
	return glm::quat();
}

RT RC::RTPar::equalRT( const RA ra, const bool cw )
{
	const RTPair pars{ ra, cw };

	for ( auto it : p_RTPar )
		if ( it.second == pars )
			return it.first;

	std::cout << "RTPar: error equalRT()" << std::endl;
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

	std::cout << "MTPar: error vec()" << std::endl;
	return glm::vec3( 0.0f );
}

RA RC::MTPar::axis( const MT mt )
{
	auto src = p_MTPar.find( mt );

	if ( src != p_MTPar.end() )
		return std::get< 0 >( src->second );

	std::cout << "MTPar: error axis()" << std::endl;
	return RA::NONE;
}

int RC::MTPar::layer( const MT mt )
{
	auto src = p_MTPar.find( mt );

	if ( src != p_MTPar.end() )
		return std::get< 1 >( src->second );

	std::cout << "MTPar: error layer()" << std::endl;
	return -1;
}

bool RC::MTPar::clockwise( const MT mt )
{
	auto src = p_MTPar.find( mt );

	if ( src != p_MTPar.end() )
		return std::get< 2 >( src->second );

	std::cout << "MTPar: error clockwise()" << std::endl;
	return false;
}

MT RC::MTPar::equalMT( const RA ra, const int lay, const bool cw )
{
	const int lay1 = ( 0 < lay && lay < CUBIE_COUNT - 1 ) ? -1 : lay;
	const MTTuple pars{ ra, lay1, cw };

	for ( auto it : p_MTPar )
		if ( it.second == pars )
			return it.first;

	std::cout << "MTPar: error equalMT()" << std::endl;
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
