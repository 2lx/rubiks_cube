#include "all.h"
#include "rcubemodel.h"

#include <cmath>
#include "rcubeparams.h"

using namespace RC;

void Cubie::rotate( CF f1, CF f2, CF f3, CF f4)
{
    CF ft;

/*    std::swap( m_colourInd[ ( int ) f1 ], m_colourInd[ ( int ) f2 ] );
    std::swap( m_colourInd[ ( int ) f2 ], m_colourInd[ ( int ) f3 ] );
    std::swap( m_colourInd[ ( int ) f3 ], m_colourInd[ ( int ) f4 ] ); */

    ft = m_colourInd[ ( int ) f1 ];
    m_colourInd[ ( int ) f1 ] = m_colourInd[ ( int ) f2 ];
    m_colourInd[ ( int ) f2 ] = m_colourInd[ ( int ) f3 ];
    m_colourInd[ ( int ) f3 ] = m_colourInd[ ( int ) f4 ];
    m_colourInd[ ( int ) f4 ] = ft;
}

void Cubie::turnCubie( const TT tt )
{
    const bool cw = TTPar::clockwise( tt );
    const RA ra = TTPar::axis( tt );

    if ( ra == RA::Z )
        if ( cw ) rotate(CF::Left, CF::Down, CF::Right, CF::Up);
        else      rotate(CF::Left, CF::Up,   CF::Right, CF::Down);
    else if ( ra == RA::Y )
        if ( cw ) rotate(CF::Left, CF::Front,CF::Right, CF::Back);
        else      rotate(CF::Left, CF::Back, CF::Right, CF::Front);
    else if ( ra == RA::X )
        if ( cw ) rotate(CF::Back, CF::Up,   CF::Front, CF::Down);
        else      rotate(CF::Back, CF::Down, CF::Front, CF::Up);
    else return;
}

CubeModel::CubeModel()
{
    const int c = CUBIE_COUNT - 1;

    // TODO: for side >2 skip center cubies
    for( int i = 0; i < CUBIE_COUNT; ++i )
        for( int j = 0; j < CUBIE_COUNT; ++j )
            for( int k = 0; k < CUBIE_COUNT; ++k ) {
 //               if ( i != 0 && i != c && j != 0 && j != c && k != 0 && k != c )
                    m_cubie[ i ][ j ][ k ] = new Cubie();
 //               else m_cubie[ i ][ j ][ k ] = nullptr;
            }

    // initialize rings in order
    //  2 3 4
    //  1 X 5
    //  0 7 6
    // 3 rings (consistion of 8 cubies) for each of the 3 axis, but only 26 cubies in total
    for ( int i = 0; i < CUBIE_COUNT; ++i )
    {
        for ( int j = 0; j < c; ++j )
        {
            m_ring[ i ][ RA::X ].push_back( *m_cubie[ i ][ 0 ][ j ] );
            m_ring[ i ][ RA::Y ].push_back( *m_cubie[ j ][ i ][ 0 ] );
            m_ring[ i ][ RA::Z ].push_back( *m_cubie[ 0 ][ j ][ i ] );
        }

        for ( int k = 0; k < c; ++k )
        {
            m_ring[ i ][ RA::X ].push_back( *m_cubie[ i ][ k ][ c ] );
            m_ring[ i ][ RA::Y ].push_back( *m_cubie[ c ][ i ][ k ] );
            m_ring[ i ][ RA::Z ].push_back( *m_cubie[ k ][ c ][ i ] );
        }

        for ( int j = c; j > 0; --j )
        {
            m_ring[ i ][ RA::X ].push_back( *m_cubie[ i ][ c ][ j ] );
            m_ring[ i ][ RA::Y ].push_back( *m_cubie[ j ][ i ][ c ] );
            m_ring[ i ][ RA::Z ].push_back( *m_cubie[ c ][ j ][ i ] );
        }

        for ( int k = c; k > 0; --k )
        {
            m_ring[ i ][ RA::X ].push_back( *m_cubie[ i ][ k ][ 0 ] );
            m_ring[ i ][ RA::Y ].push_back( *m_cubie[ 0 ][ i ][ k ] );
            m_ring[ i ][ RA::Z ].push_back( *m_cubie[ k ][ 0 ][ i ] );
        }
    }

    reset();
}

CubeModel::~CubeModel()
{
    for( int i = 0; i < CUBIE_COUNT; ++i )
        for( int j = 0; j < CUBIE_COUNT; ++j )
            for( int k = 0; k < CUBIE_COUNT; ++k )
                delete m_cubie[ i ][ j ][ k ];
}

void CubeModel::reset()
{
    const int k = CUBIE_COUNT - 1;

    for( int i = 0; i < CUBIE_COUNT; ++i )
        for( int j = 0; j < CUBIE_COUNT; ++j )
        {
            m_cubie[ i ][ j ][ k ]->setColourInd( CF::Front, CF::Front );
            m_cubie[ i ][ k ][ j ]->setColourInd( CF::Up, CF::Up );
            m_cubie[ i ][ j ][ 0 ]->setColourInd( CF::Back, CF::Back );
            m_cubie[ i ][ 0 ][ j ]->setColourInd( CF::Down, CF::Down );
            m_cubie[ 0 ][ i ][ j ]->setColourInd( CF::Left, CF::Left );
            m_cubie[ k ][ i ][ j ]->setColourInd( CF::Right, CF::Right );
        }
}

void CubeModel::turnSide( const TT tt, const int mLayer )
{
    //TODO: implement for size > 3
    const int c = CUBIE_COUNT - 1;

    const bool cw = TTPar::clockwise( tt );
    const RA ra = TTPar::axis( tt );
    int lay = TTPar::layer( tt );

    if ( lay < 0 ) lay = 1;
    if ( ra == RA::None ) return;

    // turn the side of the cube, moving around the ring
    auto vBeg = m_ring[ lay ][ ra ].begin();
    auto vMid = vBeg + ( !cw ? c : 3 * c );
    auto vEnd = m_ring[ lay ][ ra ].end();
    auto vNext = vMid;

    // std::rotate analogue
    while ( vBeg != vNext )
    {
        std::swap( (*vBeg).get(), (*vNext).get() );
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
        ( * vBeg++ ).get().turnCubie( tt );
}
