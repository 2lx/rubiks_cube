#include "all.h"
#include "rcubemodel.h"

#include <cmath>
#include "rcubeparams.h"

using namespace RC;

void Cubie::turnCubie( const TT tt )
{
    CF ft, f1, f2, f3, f4;

    const bool cw = TTPar::clockwise( tt );
    const RA ra = TTPar::axis( tt );

    if ( ra == RA::Z )
        if ( cw ) { f1 = CF::LEFT; f2 = CF::DOWN;  f3 = CF::RIGHT; f4 = CF::UP; }
        else      { f1 = CF::LEFT; f2 = CF::UP;    f3 = CF::RIGHT; f4 = CF::DOWN; }
    else if ( ra == RA::Y )
        if ( cw ) { f1 = CF::LEFT; f2 = CF::FRONT; f3 = CF::RIGHT; f4 = CF::BACK; }
        else      { f1 = CF::LEFT; f2 = CF::BACK;  f3 = CF::RIGHT; f4 = CF::FRONT; }
    else if ( ra == RA::X )
        if ( cw ) { f1 = CF::BACK; f2 = CF::UP;    f3 = CF::FRONT; f4 = CF::DOWN; }
        else      { f1 = CF::BACK; f2 = CF::DOWN;  f3 = CF::FRONT; f4 = CF::UP; }
    else return;

    ft = m_colourInd[ ( int ) f1 ];
    m_colourInd[ ( int ) f1 ] = m_colourInd[ ( int ) f2 ];
    m_colourInd[ ( int ) f2 ] = m_colourInd[ ( int ) f3 ];
    m_colourInd[ ( int ) f3 ] = m_colourInd[ ( int ) f4 ];
    m_colourInd[ ( int ) f4 ] = ft;
}

CubeModel::CubeModel()
{
    reset();
}

CubeModel::~CubeModel()
{

}

void CubeModel::reset()
{
    const int k = CUBIE_COUNT - 1;

    for( int i = 0; i < CUBIE_COUNT; ++i )
        for( int j = 0; j < CUBIE_COUNT; ++j )
        {
            m_cubies[ i ][ j ][ k ].setColourInd( CF::FRONT, CF::FRONT );
            m_cubies[ i ][ k ][ j ].setColourInd( CF::UP, CF::UP );
            m_cubies[ i ][ j ][ 0 ].setColourInd( CF::BACK, CF::BACK );
            m_cubies[ i ][ 0 ][ j ].setColourInd( CF::DOWN, CF::DOWN );
            m_cubies[ 0 ][ i ][ j ].setColourInd( CF::LEFT, CF::LEFT );
            m_cubies[ k ][ i ][ j ].setColourInd( CF::RIGHT, CF::RIGHT );
        }
}

void CubeModel::turnCubies( const TT tt, const int mLayer )
{
    //TODO: size > 3
    const int sc = ( CUBIE_COUNT - 1 );

    Cubie tmpPiece[ sc ];
    int mvX[ sc * 4 ];
    int mvY[ sc * 4 ];
    int mvZ[ sc * 4 ];

    for ( int i = 0; i < CUBIE_COUNT - 1; i++ )
    {
        mvZ[ i ] = mLayer;
        mvZ[ i + sc ] = mLayer;
        mvZ[ i + 2 * sc ] = mLayer;
        mvZ[ i + 3 * sc ] = mLayer;

        mvX[ i ] = i;
        mvX[ i + sc ] = sc;
        mvX[ i + 2 * sc ] = sc - i;
        mvX[ i + 3 * sc ] = 0;

        mvY[ i ] = 0;
        mvY[ i + sc ] = i;
        mvY[ i + 2 * sc ] = sc;
        mvY[ i + 3 * sc ] = sc - i;
    }

    const int * mv1;
    const int * mv2;
    const int * mv3;

    const bool cw = TTPar::clockwise( tt );
    const RA ra = TTPar::axis( tt );

    if ( ra == RA::Z )
        if ( cw ) { mv1 = mvX; mv2 = mvY; mv3 = mvZ; }
        else      { mv1 = mvY; mv2 = mvX; mv3 = mvZ; }
    else if ( ra == RA::Y )
        if ( cw ) { mv1 = mvY; mv2 = mvZ; mv3 = mvX; }
        else      { mv1 = mvX; mv2 = mvZ; mv3 = mvY; }
    else if ( ra == RA::X )
        if ( cw ) { mv1 = mvZ; mv2 = mvX; mv3 = mvY; }
        else      { mv1 = mvZ; mv2 = mvY; mv3 = mvX; }
    else return;

    for ( int i = 0; i < CUBIE_COUNT - 1; i++ )
    {
        tmpPiece[ i ] = m_cubies[ mv1[ i ] ][ mv2[ i ] ][ mv3[ i ] ];
        m_cubies[ mv1[ i ] ][ mv2[ i ] ][ mv3[ i ] ] = m_cubies[ mv1[ i + sc ] ][ mv2[ i + sc ] ][ mv3[ i + sc ] ];
        m_cubies[ mv1[ i + sc ] ][ mv2[ i + sc ] ][ mv3[ i + sc ] ] = m_cubies[ mv1[ i + 2 * sc ] ][ mv2[ i + 2 * sc ] ][ mv3[ i + 2 * sc ] ];
        m_cubies[ mv1[ i + 2 * sc ] ][ mv2[ i + 2 * sc ] ][ mv3[ i + 2 * sc ] ] = m_cubies[ mv1[ i + 3 * sc ] ][ mv2[ i + 3 * sc ] ][ mv3[ i + 3 * sc ] ];
        m_cubies[ mv1[ i + 3 * sc ] ][ mv2[ i + 3 * sc ] ][ mv3[ i + 3 * sc ] ] = tmpPiece[ i ];
    }

    for ( int i = 0; i < sc * 4; ++i )
        m_cubies[ mv1[ i ] ][ mv2[ i ] ][ mv3[ i ] ].turnCubie( tt );
}
