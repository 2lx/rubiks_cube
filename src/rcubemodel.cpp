#include "all.h"
#include "rcubemodel.h"

#include <cmath>

std::map< RCMoveType, RCMoveTable * > RCMoveTable::m_p = InitMap();

void RCCubie::rotateCubie( const RCMoveType rt )
{
	short int colTemp;

	if ( rt == MT_FRONT || rt == MT_BACKINV )
	{
		colTemp = m_colourInd[ AX_LEFT ];
		m_colourInd[ AX_LEFT ] = m_colourInd[ AX_DOWN ];
		m_colourInd[ AX_DOWN ] = m_colourInd[ AX_RIGHT ];
		m_colourInd[ AX_RIGHT ] = m_colourInd[ AX_UP ];
		m_colourInd[ AX_UP ] = colTemp;
	}
	else if ( rt == MT_FRONTINV || rt == MT_BACK )
	{
		colTemp = m_colourInd[ AX_LEFT ];
		m_colourInd[ AX_LEFT ] = m_colourInd[ AX_UP ];
		m_colourInd[ AX_UP ] = m_colourInd[ AX_RIGHT ];
		m_colourInd[ AX_RIGHT ] = m_colourInd[ AX_DOWN ];
		m_colourInd[ AX_DOWN ] = colTemp;
	}
	else if ( rt == MT_LEFT || rt == MT_RIGHTINV )
	{
		colTemp = m_colourInd[ AX_BACK ];
		m_colourInd[ AX_BACK ] = m_colourInd[ AX_DOWN ];
		m_colourInd[ AX_DOWN ] = m_colourInd[ AX_FRONT ];
		m_colourInd[ AX_FRONT ] = m_colourInd[ AX_UP ];
		m_colourInd[ AX_UP ] = colTemp;
	}
	else if ( rt == MT_RIGHT || rt == MT_LEFTINV )
	{
		colTemp = m_colourInd[ AX_BACK ];
		m_colourInd[ AX_BACK ] = m_colourInd[ AX_UP ];
		m_colourInd[ AX_UP ] = m_colourInd[ AX_FRONT ];
		m_colourInd[ AX_FRONT ] = m_colourInd[ AX_DOWN ];
		m_colourInd[ AX_DOWN ] = colTemp;
	}
	else if ( rt == MT_UP || rt == MT_DOWNINV )
	{
		colTemp = m_colourInd[ AX_LEFT ];
		m_colourInd[ AX_LEFT ] = m_colourInd[ AX_FRONT ];
		m_colourInd[ AX_FRONT ] = m_colourInd[ AX_RIGHT ];
		m_colourInd[ AX_RIGHT ] = m_colourInd[ AX_BACK ];
		m_colourInd[ AX_BACK ] = colTemp;
	}
	else if ( rt == MT_UPINV || rt == MT_DOWN )
	{
		colTemp = m_colourInd[ AX_LEFT ];
		m_colourInd[ AX_LEFT ] = m_colourInd[ AX_BACK ];
		m_colourInd[ AX_BACK ] = m_colourInd[ AX_RIGHT ];
		m_colourInd[ AX_RIGHT ] = m_colourInd[ AX_FRONT ];
		m_colourInd[ AX_FRONT ] = colTemp;
	}
}

float RCCubie::colR( const RCAxis ax ) const
{
	if ( m_colourInd[ ax ] == -1 )
		return COLOR_MATR[ 6 ][ 0 ];
	else return COLOR_MATR[ m_colourInd[ ax ] ][ 0 ];
}

float RCCubie::colG( const RCAxis ax ) const
{
	if ( m_colourInd[ ax ] == -1 )
		return COLOR_MATR[ 6 ][ 1 ];
	else return COLOR_MATR[ m_colourInd[ ax ] ][ 1 ];
}

float RCCubie::colB( const RCAxis ax ) const
{
	if ( m_colourInd[ ax ] == -1 )
		return COLOR_MATR[ 6 ][ 2 ];
	else return COLOR_MATR[ m_colourInd[ ax ] ][ 2 ];
}

RCubeModel::RCubeModel()
{
	srand( time( 0 ) );
	const int k = CUBIE_COUNT - 1;

	for( int i = 0; i < CUBIE_COUNT; ++i )
		for( int j = 0; j < CUBIE_COUNT; ++j )
		{
			m_cubies[ i ][ j ][ k ].setColourInd( AX_FRONT, 0 );
			m_cubies[ i ][ k ][ j ].setColourInd( AX_UP, 1 );
			m_cubies[ i ][ j ][ 0 ].setColourInd( AX_BACK, 2 );
			m_cubies[ i ][ 0 ][ j ].setColourInd( AX_DOWN, 3 );
			m_cubies[ 0 ][ i ][ j ].setColourInd( AX_LEFT, 4 );
			m_cubies[ k ][ i ][ j ].setColourInd( AX_RIGHT, 5 );
		}
}

RCubeModel::~RCubeModel()
{

}

void RCubeModel::moveCubies( const RCMoveType rt )
{
	RCCubie tmpPiece1;
	RCCubie tmpPiece2;
	const int k = CUBIE_COUNT - 1;

	const int mvX[ 8 ] = { 0, 1, 2, 2, 2, 1, 0, 0 };
	const int mvY[ 8 ] = { 0, 0, 0, 1, 2, 2, 2, 1 };
	const int mvZ[ 8 ] = { k, k, k, k, k, k, k, k };
	const int mv0[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	const int * mv1;
	const int * mv2;
	const int * mv3;

    switch ( rt )
    {
	case MT_FRONT:
		mv1 = mvX; mv2 = mvY; mv3 = mvZ;
		break;
	case MT_FRONTINV:
		mv1 = mvY; mv2 = mvX; mv3 = mvZ;
		break;
	case MT_BACK:
		mv1 = mvY; mv2 = mvX; mv3 = mv0;
		break;
	case MT_BACKINV:
		mv1 = mvX; mv2 = mvY; mv3 = mv0;
		break;
	case MT_LEFT:
		mv1 = mv0; mv2 = mvY; mv3 = mvX;
		break;
	case MT_LEFTINV:
		mv1 = mv0; mv2 = mvX; mv3 = mvY;
		break;
	case MT_RIGHT:
		mv1 = mvZ; mv2 = mvX; mv3 = mvY;
		break;
	case MT_RIGHTINV:
		mv1 = mvZ; mv2 = mvY; mv3 = mvX;
		break;
	case MT_UP:
		mv1 = mvY; mv2 = mvZ; mv3 = mvX;
		break;
	case MT_UPINV:
		mv1 = mvX; mv2 = mvZ; mv3 = mvY;
		break;
	case MT_DOWN:
		mv1 = mvX; mv2 = mv0; mv3 = mvY;
		break;
	case MT_DOWNINV:
		mv1 = mvY; mv2 = mv0; mv3 = mvX;
		break;
	default: ;
	}

	tmpPiece1 = m_cubies[ mv1[ 0 ] ][ mv2[ 0 ] ][ mv3[ 0 ] ];
	tmpPiece2 = m_cubies[ mv1[ 1 ] ][ mv2[ 1 ] ][ mv3[ 1 ] ];
	m_cubies[ mv1[ 0 ] ][ mv2[ 0 ] ][ mv3[ 0 ] ] = m_cubies[ mv1[ 2 ] ][ mv2[ 2 ] ][ mv3[ 2 ] ];
	m_cubies[ mv1[ 1 ] ][ mv2[ 1 ] ][ mv3[ 1 ] ] = m_cubies[ mv1[ 3 ] ][ mv2[ 3 ] ][ mv3[ 3 ] ];
	m_cubies[ mv1[ 2 ] ][ mv2[ 2 ] ][ mv3[ 2 ] ] = m_cubies[ mv1[ 4 ] ][ mv2[ 4 ] ][ mv3[ 4 ] ];
	m_cubies[ mv1[ 3 ] ][ mv2[ 3 ] ][ mv3[ 3 ] ] = m_cubies[ mv1[ 5 ] ][ mv2[ 5 ] ][ mv3[ 5 ] ];
	m_cubies[ mv1[ 4 ] ][ mv2[ 4 ] ][ mv3[ 4 ] ] = m_cubies[ mv1[ 6 ] ][ mv2[ 6 ] ][ mv3[ 6 ] ];
	m_cubies[ mv1[ 5 ] ][ mv2[ 5 ] ][ mv3[ 5 ] ] = m_cubies[ mv1[ 7 ] ][ mv2[ 7 ] ][ mv3[ 7 ] ];
	m_cubies[ mv1[ 6 ] ][ mv2[ 6 ] ][ mv3[ 6 ] ] = tmpPiece1;
	m_cubies[ mv1[ 7 ] ][ mv2[ 7 ] ][ mv3[ 7 ] ] = tmpPiece2;

	for ( int i = 0; i < 8; ++i )
		m_cubies[ mv1[ i ] ][ mv2[ i ] ][ mv3[ i ] ].rotateCubie( rt );
}
