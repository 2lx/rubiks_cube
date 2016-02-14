#ifndef RCUBEPARAMS_H
#define RCUBEPARAMS_H

#include "rcdefines.h"
#include <glm/gtc/type_ptr.hpp>

namespace RC
{
    namespace RAPar
    {
        glm::vec3 vec( const RA ra );
        RA closestRA( glm::vec3 vec );
    }

    namespace RTPar
    {
    inline bool isRT( const int val )
    {
        if( 0 < val && val < ( int ) RC::RT::Count )
            return true;
        else return false;
    }

    glm::quat quat( const RT rt );

    RT equalRT( const RA ra, const bool cw );
    }

    namespace TTPar
    {
    inline bool isTT( const int val ) {
        if( 0 < val && val < ( int ) RC::TT::Count )
            return true;
        else return false;
    }

        glm::vec3 vec( const TT mt );
        RA axis( const TT mt );
        bool clockwise( const TT mt );
        int layer( const TT mt );

        TT equalTT( const RA ra, const int lay, const bool cw );
    }

    namespace GKPar
    {
    TT toTT( const GK gk );
    RT toRT( const GK gk );
    GK fromTT( const TT mt );
    GK fromRT( const RT rt );

    inline bool enableWithTurn( const RC::GK gk )
    {
        if ( ( RC::GKPar::toTT( gk ) == RC::TT::None )
                && gk != RC::GK::GameMix && gk != RC::GK::GameReset && gk != RC::GK::GameUndo )
            return true;
        else return false;
    }

    RC::GK prevGK( const RC::GK gk );

    std::string str( const GK gk );
    }
}
#endif // RCUBEPARAMS_H
