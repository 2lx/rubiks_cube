#include "all.h"
#include "rcubemodel.h"

#include <cmath>
#include "rcubeparams.h"

using namespace RC;

void Cubie::rotateCubie( const TT mt )
{
    CF tt, t1, t2, t3, t4;

    // TODO:
    switch ( mt )
    {
        case TT::F:
        case TT::BI:
        case TT::FM:
            t1 = CF::LEFT; t2 = CF::DOWN; t3 = CF::RIGHT; t4 = CF::UP;
            break;
        case TT::FI:
        case TT::B:
        case TT::FMI:
            t1 = CF::LEFT; t2 = CF::UP; t3 = CF::RIGHT; t4 = CF::DOWN;
            break;
        case TT::L:
        case TT::RI:
        case TT::RMI:
            t1 = CF::BACK; t2 = CF::DOWN; t3 = CF::FRONT; t4 = CF::UP;
            break;
        case TT::LI:
        case TT::R:
        case TT::RM:
            t1 = CF::BACK; t2 = CF::UP; t3 = CF::FRONT; t4 = CF::DOWN;
            break;
        case TT::U:
        case TT::DI:
        case TT::UM:
            t1 = CF::LEFT; t2 = CF::FRONT; t3 = CF::RIGHT; t4 = CF::BACK;
            break;
        case TT::UI:
        case TT::D:
        case TT::UMI:
            t1 = CF::LEFT; t2 = CF::BACK; t3 = CF::RIGHT; t4 = CF::FRONT;
            break;
        default: return;
    };

    tt = m_colourInd[ ( int ) t1 ];
    m_colourInd[ ( int ) t1 ] = m_colourInd[ ( int ) t2 ];
    m_colourInd[ ( int ) t2 ] = m_colourInd[ ( int ) t3 ];
    m_colourInd[ ( int ) t3 ] = m_colourInd[ ( int ) t4 ];
    m_colourInd[ ( int ) t4 ] = tt;
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

void CubeModel::turnCubies( const TT mt, const int mLayer )
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

    //TODO get pars
    switch ( mt )
    {
    case TT::F:
    case TT::BI:
    case TT::FM:
        mv1 = mvX; mv2 = mvY; mv3 = mvZ;
        break;
    case TT::B:
    case TT::FI:
    case TT::FMI:
        mv1 = mvY; mv2 = mvX; mv3 = mvZ;
        break;
    case TT::L:
    case TT::RI:
    case TT::RMI:
        mv1 = mvZ; mv2 = mvY; mv3 = mvX;
        break;
    case TT::LI:
    case TT::R:
    case TT::RM:
        mv1 = mvZ; mv2 = mvX; mv3 = mvY;
        break;
    case TT::U:
    case TT::DI:
    case TT::UM:
        mv1 = mvY; mv2 = mvZ; mv3 = mvX;
        break;
    case TT::UI:
    case TT::D:
    case TT::UMI:
        mv1 = mvX; mv2 = mvZ; mv3 = mvY;
        break;
    default:
        return;
    }

    for ( int i = 0; i < CUBIE_COUNT - 1; i++ )
    {
        tmpPiece[ i ] = m_cubies[ mv1[ i ] ][ mv2[ i ] ][ mv3[ i ] ];
        m_cubies[ mv1[ i ] ][ mv2[ i ] ][ mv3[ i ] ] = m_cubies[ mv1[ i + sc ] ][ mv2[ i + sc ] ][ mv3[ i + sc ] ];
        m_cubies[ mv1[ i + sc ] ][ mv2[ i + sc ] ][ mv3[ i + sc ] ] = m_cubies[ mv1[ i + 2 * sc ] ][ mv2[ i + 2 * sc ] ][ mv3[ i + 2 * sc ] ];
        m_cubies[ mv1[ i + 2 * sc ] ][ mv2[ i + 2 * sc ] ][ mv3[ i + 2 * sc ] ] = m_cubies[ mv1[ i + 3 * sc ] ][ mv2[ i + 3 * sc ] ][ mv3[ i + 3 * sc ] ];
        m_cubies[ mv1[ i + 3 * sc ] ][ mv2[ i + 3 * sc ] ][ mv3[ i + 3 * sc ] ] = tmpPiece[ i ];
    }

    for ( int i = 0; i < sc * 4; ++i )
        m_cubies[ mv1[ i ] ][ mv2[ i ] ][ mv3[ i ] ].rotateCubie( mt );
}
