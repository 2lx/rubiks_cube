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
    const int c = CUBIE_COUNT - 1;

    // create cubies
    for( int i = 0; i < CUBIE_COUNT; ++i )
        for( int j = 0; j < CUBIE_COUNT; ++j )
            for( int k = 0; k < CUBIE_COUNT; ++k )
                m_cubies[ i ][ j ][ k ] = new Cubie();

    // initialize rings
    for ( int i = 0; i < CUBIE_COUNT; ++i )
    {
        for ( int j = 0; j < c; ++j )
        {
            m_ring[ i ][ RC::RA::X ].push_back( m_cubies[ i ][ 0 ][ j ] );
            m_ring[ i ][ RC::RA::Y ].push_back( m_cubies[ j ][ i ][ 0 ] );
            m_ring[ i ][ RC::RA::Z ].push_back( m_cubies[ 0 ][ j ][ i ] );
        }

        for ( int k = 0; k < c; ++k )
        {
            m_ring[ i ][ RC::RA::X ].push_back( m_cubies[ i ][ k ][ c ] );
            m_ring[ i ][ RC::RA::Y ].push_back( m_cubies[ c ][ i ][ k ] );
            m_ring[ i ][ RC::RA::Z ].push_back( m_cubies[ k ][ c ][ i ] );
        }

        for ( int j = c; j > 0; --j )
        {
            m_ring[ i ][ RC::RA::X ].push_back( m_cubies[ i ][ c ][ j ] );
            m_ring[ i ][ RC::RA::Y ].push_back( m_cubies[ j ][ i ][ c ] );
            m_ring[ i ][ RC::RA::Z ].push_back( m_cubies[ c ][ j ][ i ] );
        }

        for ( int k = c; k > 0; --k )
        {
            m_ring[ i ][ RC::RA::X ].push_back( m_cubies[ i ][ k ][ 0 ] );
            m_ring[ i ][ RC::RA::Y ].push_back( m_cubies[ 0 ][ i ][ k ] );
            m_ring[ i ][ RC::RA::Z ].push_back( m_cubies[ k ][ 0 ][ i ] );
        }
    }

    reset();
}

CubeModel::~CubeModel()
{
    for( int i = 0; i < CUBIE_COUNT; ++i )
        for( int j = 0; j < CUBIE_COUNT; ++j )
            for( int k = 0; k < CUBIE_COUNT; ++k )
                delete m_cubies[ i ][ j ][ k ];
}

void CubeModel::reset()
{
    const int k = CUBIE_COUNT - 1;

    for( int i = 0; i < CUBIE_COUNT; ++i )
        for( int j = 0; j < CUBIE_COUNT; ++j )
        {
            m_cubies[ i ][ j ][ k ]->setColourInd( CF::FRONT, CF::FRONT );
            m_cubies[ i ][ k ][ j ]->setColourInd( CF::UP, CF::UP );
            m_cubies[ i ][ j ][ 0 ]->setColourInd( CF::BACK, CF::BACK );
            m_cubies[ i ][ 0 ][ j ]->setColourInd( CF::DOWN, CF::DOWN );
            m_cubies[ 0 ][ i ][ j ]->setColourInd( CF::LEFT, CF::LEFT );
            m_cubies[ k ][ i ][ j ]->setColourInd( CF::RIGHT, CF::RIGHT );
        }
}

void CubeModel::turnSide( const TT tt, const int mLayer )
{
    //TODO: implement for size > 3
    const bool cw = TTPar::clockwise( tt );
    const RA ra = TTPar::axis( tt );
    int lay = TTPar::layer( tt );
    if ( lay < 0 ) lay = 1;

    // turn the side of the cube, moving around the ring
    auto vBeg = m_ring[ lay ][ ra ].begin();
    auto vMid = vBeg + ( !cw ? CUBIE_COUNT - 1 : 3 * CUBIE_COUNT - 3 );
    auto vEnd = m_ring[ lay ][ ra ].end();
    auto vNext = vMid;

    // std::rotate analogue
    while ( vBeg != vNext )
    {
        std::swap( **vBeg, **vNext );
        vBeg++;
        vNext++;

        if ( vNext == vEnd )
            vNext = vMid;
        else if ( vBeg == vMid )
            vMid = vNext;
    }

    // turn faces of cubies
    vBeg = m_ring[ lay ][ ra ].begin();
    vEnd = m_ring[ lay ][ ra ].end();

    while ( vBeg != vEnd )
        ( * vBeg++ )->turnCubie( tt );
}
