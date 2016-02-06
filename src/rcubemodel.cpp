#include "all.h"
#include "rcubemodel.h"

#include <cmath>
#include "rcubeparams.h"

using namespace RC;

void Cubie::rotateCubie( const MoveType rt )
{
	CubeFace tt, t1, t2, t3, t4;

	switch ( rt )
	{
		case MT_FRONT:
		case MT_BACKINV:
		case MT_FRONTMID:
			t1 = CF_LEFT; t2 = CF_DOWN; t3 = CF_RIGHT; t4 = CF_UP;
			break;
		case MT_FRONTINV:
		case MT_BACK:
		case MT_FRONTMIDINV:
			t1 = CF_LEFT; t2 = CF_UP; t3 = CF_RIGHT; t4 = CF_DOWN;
			break;
		case MT_LEFT:
		case MT_RIGHTINV:
		case MT_RIGHTMIDINV:
			t1 = CF_BACK; t2 = CF_DOWN; t3 = CF_FRONT; t4 = CF_UP;
			break;
		case MT_LEFTINV:
		case MT_RIGHT:
		case MT_RIGHTMID:
			t1 = CF_BACK; t2 = CF_UP; t3 = CF_FRONT; t4 = CF_DOWN;
			break;
		case MT_UP:
		case MT_DOWNINV:
		case MT_UPMID:
			t1 = CF_LEFT; t2 = CF_FRONT; t3 = CF_RIGHT; t4 = CF_BACK;
			break;
		case MT_UPINV:
		case MT_DOWN:
		case MT_UPMIDINV:
			t1 = CF_LEFT; t2 = CF_BACK; t3 = CF_RIGHT; t4 = CF_FRONT;
			break;
		default: return;
	};

	tt = m_colourInd[ t1 ];
	m_colourInd[ t1 ] = m_colourInd[ t2 ];
	m_colourInd[ t2 ] = m_colourInd[ t3 ];
	m_colourInd[ t3 ] = m_colourInd[ t4 ];
	m_colourInd[ t4 ] = tt;
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
			m_cubies[ i ][ j ][ k ].setColourInd( CF_FRONT, CF_FRONT );
			m_cubies[ i ][ k ][ j ].setColourInd( CF_UP, CF_UP );
			m_cubies[ i ][ j ][ 0 ].setColourInd( CF_BACK, CF_BACK );
			m_cubies[ i ][ 0 ][ j ].setColourInd( CF_DOWN, CF_DOWN );
			m_cubies[ 0 ][ i ][ j ].setColourInd( CF_LEFT, CF_LEFT );
			m_cubies[ k ][ i ][ j ].setColourInd( CF_RIGHT, CF_RIGHT );
		}
}

void CubeModel::moveCubies( const MoveType rt, const int mLayer )
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

    switch ( rt )
    {
	case MT_FRONT:
	case MT_BACKINV:
	case MT_FRONTMID:
		mv1 = mvX; mv2 = mvY; mv3 = mvZ;
		break;
	case MT_BACK:
	case MT_FRONTINV:
	case MT_FRONTMIDINV:
		mv1 = mvY; mv2 = mvX; mv3 = mvZ;
		break;
	case MT_LEFT:
	case MT_RIGHTINV:
	case MT_RIGHTMIDINV:
		mv1 = mvZ; mv2 = mvY; mv3 = mvX;
		break;
	case MT_LEFTINV:
	case MT_RIGHT:
	case MT_RIGHTMID:
		mv1 = mvZ; mv2 = mvX; mv3 = mvY;
		break;
	case MT_UP:
	case MT_DOWNINV:
	case MT_UPMID:
		mv1 = mvY; mv2 = mvZ; mv3 = mvX;
		break;
	case MT_UPINV:
	case MT_DOWN:
	case MT_UPMIDINV:
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
		m_cubies[ mv1[ i ] ][ mv2[ i ] ][ mv3[ i ] ].rotateCubie( rt );
}
