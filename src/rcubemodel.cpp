#include "all.h"
#include "rcubemodel.h"

#include <cmath>
#include "rcubeparams.h"

using namespace RC;

void Cubie::rotateCubie( const RCMoveType rt )
{
	RCAxis tt, t1, t2, t3, t4;

	switch ( rt )
	{
		case MT_FRONT:
		case MT_BACKINV:
			t1 = AX_LEFT; t2 = AX_DOWN; t3 = AX_RIGHT; t4 = AX_UP;
			break;
		case MT_FRONTINV:
		case MT_BACK:
			t1 = AX_LEFT; t2 = AX_UP; t3 = AX_RIGHT; t4 = AX_DOWN;
			break;
		case MT_LEFT:
		case MT_RIGHTINV:
			t1 = AX_BACK; t2 = AX_DOWN; t3 = AX_FRONT; t4 = AX_UP;
			break;
		case MT_LEFTINV:
		case MT_RIGHT:
			t1 = AX_BACK; t2 = AX_UP; t3 = AX_FRONT; t4 = AX_DOWN;
			break;
		case MT_UP:
		case MT_DOWNINV:
			t1 = AX_LEFT; t2 = AX_FRONT; t3 = AX_RIGHT; t4 = AX_BACK;
			break;
		case MT_UPINV:
		case MT_DOWN:
			t1 = AX_LEFT; t2 = AX_BACK; t3 = AX_RIGHT; t4 = AX_FRONT;
			break;
		default: return;
	};

	tt = m_colourInd[ t1 ];
	m_colourInd[ t1 ] = m_colourInd[ t2 ];
	m_colourInd[ t2 ] = m_colourInd[ t3 ];
	m_colourInd[ t3 ] = m_colourInd[ t4 ];
	m_colourInd[ t4 ] = tt;
}

float Cubie::colR( const RCAxis ax ) const
{
	if ( m_colourInd[ ax ] == AX_NONE )
		return Colors::colR( RC_FG );
	else return Colors::colR( m_colourInd[ ax ] );
}

float Cubie::colG( const RCAxis ax ) const
{
	if ( m_colourInd[ ax ] == AX_NONE )
		return Colors::colG( RC_FG );
	else return Colors::colG( m_colourInd[ ax ] );
}

float Cubie::colB( const RCAxis ax ) const
{
	if ( m_colourInd[ ax ] == AX_NONE )
		return Colors::colB( RC_FG );
	else return Colors::colB( m_colourInd[ ax ] );
}

CubeModel::CubeModel()
{
	srand( time( 0 ) );
	const int k = CUBIE_COUNT - 1;

	for( int i = 0; i < CUBIE_COUNT; ++i )
		for( int j = 0; j < CUBIE_COUNT; ++j )
		{
			m_cubies[ i ][ j ][ k ].setColourInd( AX_FRONT, AX_FRONT );
			m_cubies[ i ][ k ][ j ].setColourInd( AX_UP, AX_UP );
			m_cubies[ i ][ j ][ 0 ].setColourInd( AX_BACK, AX_BACK );
			m_cubies[ i ][ 0 ][ j ].setColourInd( AX_DOWN, AX_DOWN );
			m_cubies[ 0 ][ i ][ j ].setColourInd( AX_LEFT, AX_LEFT );
			m_cubies[ k ][ i ][ j ].setColourInd( AX_RIGHT, AX_RIGHT );
		}
}

CubeModel::~CubeModel()
{

}

void CubeModel::moveCubies( const RCMoveType rt )
{
	Cubie tmpPiece1;
	Cubie tmpPiece2;
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
	default:
		return;
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
