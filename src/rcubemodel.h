#ifndef RCUBEMODEL_H
#define RCUBEMODEL_H

#include <vector>
#include <map>
#include "rcdefines.h"

namespace RC
{
    class Cubie
    {
    public:
        Cubie() { }

        void turnCubie( const TT tt );
        inline CF colInd( const CF cf ) const { return m_colourInd[ ( int ) cf ]; }
        inline void setColourInd( const CF cf, const CF cind )
        {
            m_colourInd[ ( int ) cf ] = cind;
        }

    private:
        CF m_colourInd[ ( int ) CF::Count ] = { CF::None, CF::None, CF::None, CF::None, CF::None, CF::None };
    };

    class CubeModel
    {
    public:
        CubeModel();
        virtual ~CubeModel();

        void turnSide( const TT mt, const int mLayer );
        void reset();

        const Cubie cubie( const int x, const int y, const int z ) const
        {
            const int c = CUBIE_COUNT - 1;
            if ( x != 0 && x != c && y != 0 && y != c && z != 0 && z != c )
                throw std::logic_error( "CubeModel::cubie(): invalid cubie coordinates" );

            return * m_cubie[ x ][ y ][ z ];
        }

    private:
        Cubie * m_cubie[ CUBIE_COUNT ][ CUBIE_COUNT ][ CUBIE_COUNT ];
        std::map< RC::RA, std::vector< Cubie * > > m_ring[ CUBIE_COUNT ];
    };
}

#endif // RCUBEMODEL_H
