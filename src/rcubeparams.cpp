#include "all.h"

#include "rcubeparams.h"
#include "enumhash.h"
#include <unordered_map>
#include <tuple>

using namespace RC;

// RotAxis params
const std::unordered_map< RA, const glm::vec3, EnumHash > p_RAPar {
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
}

// RotateType params
typedef std::pair< RA, const bool > RTPair;

const std::unordered_map< const RT, const RTPair, EnumHash > p_RTPar {
    { RT::Up,    std::make_pair( RA::X, true ) },
    { RT::Down,  std::make_pair( RA::X, false ) },
    { RT::Left,  std::make_pair( RA::Y, true ) },
    { RT::Right, std::make_pair( RA::Y, false ) },
    { RT::ACW,   std::make_pair( RA::Z, true ) },
    { RT::CW,    std::make_pair( RA::Z, false ) }
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
}

RT RC::RTPar::equalRT( const RA ra, const bool cw )
{
    const RTPair pars{ ra, cw };

    for ( auto it : p_RTPar )
        if ( it.second == pars )
            return it.first;

    throw std::logic_error( "RTPar::equalRT()" );
}

// TurnType params
const int ll = CUBIE_COUNT - 1;
typedef std::tuple< const RA, const int, const bool > TTTuple;

const std::unordered_map< TT, const TTTuple, EnumHash > p_TTPar {
    { TT::F,    TTTuple( RA::Z, ll, true  ) },
    { TT::FI,   TTTuple( RA::Z, ll, false ) },
    { TT::B,    TTTuple( RA::Z,  0, false ) },
    { TT::BI,   TTTuple( RA::Z,  0, true  ) },
    { TT::R,    TTTuple( RA::X, ll, true  ) },
    { TT::RI,   TTTuple( RA::X, ll, false ) },
    { TT::L,    TTTuple( RA::X,  0, false ) },
    { TT::LI,   TTTuple( RA::X,  0, true  ) },
    { TT::U,    TTTuple( RA::Y, ll, true  ) },
    { TT::UI,   TTTuple( RA::Y, ll, false ) },
    { TT::D,    TTTuple( RA::Y,  0, false ) },
    { TT::DI,   TTTuple( RA::Y,  0, true  ) },
    { TT::FM,   TTTuple( RA::Z, -1, true  ) },
    { TT::FMI,  TTTuple( RA::Z, -1, false ) },
    { TT::UM,   TTTuple( RA::Y, -1, true  ) },
    { TT::UMI,  TTTuple( RA::Y, -1, false ) },
    { TT::RM,   TTTuple( RA::X, -1, true  ) },
    { TT::RMI,  TTTuple( RA::X, -1, false ) }
};

glm::vec3 RC::TTPar::vec( const TT mt )
{
    auto src = p_TTPar.find( mt );

    if ( src != p_TTPar.end() )
        return RAPar::vec( std::get< 0 >( src->second ) );

    throw std::logic_error( "MTPar::vec()" );
}

RA RC::TTPar::axis( const TT mt )
{
    auto src = p_TTPar.find( mt );

    if ( src != p_TTPar.end() )
        return std::get< 0 >( src->second );

    throw std::logic_error( "MTPar::axis()" );
}

int RC::TTPar::layer( const TT mt )
{
    auto src = p_TTPar.find( mt );

    if ( src != p_TTPar.end() )
        return std::get< 1 >( src->second );

    throw std::logic_error( "MTPar::layer()" );
}

bool RC::TTPar::clockwise( const TT mt )
{
    auto src = p_TTPar.find( mt );

    if ( src != p_TTPar.end() )
        return std::get< 2 >( src->second );

    throw std::logic_error( "MTPar::clockwise()" );
}

TT RC::TTPar::equalTT( const RA ra, const int lay, const bool cw )
{
    const int lay1 = ( 0 < lay && lay < CUBIE_COUNT - 1 ) ? -1 : lay;
    const TTTuple pars { ra, lay1, cw };

    for ( auto it : p_TTPar )
        if ( it.second == pars )
            return it.first;

    throw std::logic_error( "MTPar::equalMT()" );
}

//
TT RC::GKPar::toTT( const GK gk )
{
    switch ( gk )
    {
    case GK::TurnF:     return TT::F;
    case GK::TurnFI:    return TT::FI;
    case GK::TurnB:     return TT::B;
    case GK::TurnBI:    return TT::BI;
    case GK::TurnL:     return TT::L;
    case GK::TurnLI:    return TT::LI;
    case GK::TurnR:     return TT::R;
    case GK::TurnRI:    return TT::RI;
    case GK::TurnU:     return TT::U;
    case GK::TurnUI:    return TT::UI;
    case GK::TurnD:     return TT::D;
    case GK::TurnDI:    return TT::DI;
    case GK::TurnFM:    return TT::FM;
    case GK::TurnFMI:   return TT::FMI;
    case GK::TurnUM:    return TT::UM;
    case GK::TurnUMI:   return TT::UMI;
    case GK::TurnRM:    return TT::RM;
    case GK::TurnRMI:   return TT::RMI;
    default: return TT::None;
    }
}

GK RC::GKPar::fromTT( const TT mt )
{
    switch ( mt )
    {
    case TT::F:     return GK::TurnF;
    case TT::FI:    return GK::TurnFI;
    case TT::B:     return GK::TurnB;
    case TT::BI:    return GK::TurnBI;
    case TT::L:     return GK::TurnL;
    case TT::LI:    return GK::TurnLI;
    case TT::R:     return GK::TurnR;
    case TT::RI:    return GK::TurnRI;
    case TT::U:     return GK::TurnU;
    case TT::UI:    return GK::TurnUI;
    case TT::D:     return GK::TurnD;
    case TT::DI:    return GK::TurnDI;
    case TT::FM:    return GK::TurnFM;
    case TT::FMI:   return GK::TurnFMI;
    case TT::UM:    return GK::TurnUM;
    case TT::UMI:   return GK::TurnUMI;
    case TT::RM:    return GK::TurnRM;
    case TT::RMI:   return GK::TurnRMI;

    default: return GK::None;
    }
}

RT RC::GKPar::toRT( const GK gk )
{
    switch ( gk )
    {
    case GK::RotateUp:      return RT::Up;
    case GK::RotateDown:    return RT::Down;
    case GK::RotateLeft:    return RT::Left;
    case GK::RotateRight:   return RT::Right;
    case GK::RotateCW:      return RT::CW;
    case GK::RotateACW:     return RT::ACW;

    default: return RT::None;
    }
}

GK RC::GKPar::fromRT( const RT rt )
{
    switch ( rt )
    {
    case RT::Up:    return GK::RotateUp;
    case RT::Down:  return GK::RotateDown;
    case RT::Left:  return GK::RotateLeft;
    case RT::Right: return GK::RotateRight;
    case RT::CW:    return GK::RotateCW;
    case RT::ACW:   return GK::RotateACW;

    default: return GK::None;
    }
}

RC::GK RC::GKPar::prevGK( const RC::GK gk )
{
    switch( gk )
    {
    case RC::GK::RotateDown:    return RC::GK::RotateUp;
    case RC::GK::RotateUp:      return RC::GK::RotateDown;
    case RC::GK::RotateLeft:    return RC::GK::RotateRight;
    case RC::GK::RotateRight:   return RC::GK::RotateLeft;
    case RC::GK::RotateCW:      return RC::GK::RotateACW;
    case RC::GK::RotateACW:     return RC::GK::RotateCW;

    case RC::GK::TurnB:         return RC::GK::TurnBI;
    case RC::GK::TurnBI:        return RC::GK::TurnB;
    case RC::GK::TurnD:         return RC::GK::TurnDI;
    case RC::GK::TurnDI:        return RC::GK::TurnD;
    case RC::GK::TurnF:         return RC::GK::TurnFI;
    case RC::GK::TurnFI:        return RC::GK::TurnF;
    case RC::GK::TurnL:         return RC::GK::TurnLI;
    case RC::GK::TurnLI:        return RC::GK::TurnL;
    case RC::GK::TurnR:         return RC::GK::TurnRI;
    case RC::GK::TurnRI:        return RC::GK::TurnR;
    case RC::GK::TurnU:         return RC::GK::TurnUI;
    case RC::GK::TurnUI:        return RC::GK::TurnU;
    case RC::GK::TurnFM:        return RC::GK::TurnFMI;
    case RC::GK::TurnFMI:       return RC::GK::TurnFM;
    case RC::GK::TurnUM:        return RC::GK::TurnUMI;
    case RC::GK::TurnUMI:       return RC::GK::TurnUM;
    case RC::GK::TurnRM:        return RC::GK::TurnRMI;
    case RC::GK::TurnRMI:       return RC::GK::TurnRM;

    default: return RC::GK::None;
    }
}

const std::unordered_map< GK, const std::string, EnumHash > p_GKPar {
    { GK::TurnF,    "F" },
    { GK::TurnFI,   "F'" },
    { GK::TurnB,    "B" },
    { GK::TurnBI,   "B'" },
    { GK::TurnL,    "L" },
    { GK::TurnLI,   "L'" },
    { GK::TurnR,    "R" },
    { GK::TurnRI,   "R'" },
    { GK::TurnU,    "U" },
    { GK::TurnUI,   "U'" },
    { GK::TurnD,    "D" },
    { GK::TurnDI,   "D'" },
    { GK::TurnFM,   "f" },
    { GK::TurnFMI,  "f'" },
    { GK::TurnUM,   "u" },
    { GK::TurnUMI,  "u'" },
    { GK::TurnRM,   "r" },
    { GK::TurnRMI,  "r'" },

    { GK::RotateUp,     "rtU" },
    { GK::RotateDown,   "rtD" },
    { GK::RotateLeft,   "rtL" },
    { GK::RotateRight,  "rtR" },
    { GK::RotateCW,     "rtCW" },
    { GK::RotateACW,    "rtACW" }
};

std::string RC::GKPar::str( const GK gk )
{
    auto src = p_GKPar.find( gk );

    if ( src != p_GKPar.end() )
        return src->second;

    throw std::logic_error( "GKPar::str()" );
}
