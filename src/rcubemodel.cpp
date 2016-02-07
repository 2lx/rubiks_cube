#include "all.h"
#include "rcubemodel.h"

#include <cmath>
#include "rcubeparams.h"

using namespace RC;

void Cubie::rotateCubie( const MT mt )
{
	CF tt, t1, t2, t3, t4;

	// TODO:
	switch ( mt )
	{
		case MT::F:
		case MT::BI:
		case MT::FM:
			t1 = CF::LEFT; t2 = CF::DOWN; t3 = CF::RIGHT; t4 = CF::UP;
			break;
		case MT::FI:
		case MT::B:
		case MT::FMI:
			t1 = CF::LEFT; t2 = CF::UP; t3 = CF::RIGHT; t4 = CF::DOWN;
			break;
		case MT::L:
		case MT::RI:
		case MT::RMI:
			t1 = CF::BACK; t2 = CF::DOWN; t3 = CF::FRONT; t4 = CF::UP;
			break;
		case MT::LI:
		case MT::R:
		case MT::RM:
			t1 = CF::BACK; t2 = CF::UP; t3 = CF::FRONT; t4 = CF::DOWN;
			break;
		case MT::U:
		case MT::DI:
		case MT::UM:
			t1 = CF::LEFT; t2 = CF::FRONT; t3 = CF::RIGHT; t4 = CF::BACK;
			break;
		case MT::UI:
		case MT::D:
		case MT::UMI:
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

void CubeModel::moveCubies( const MT mt, const int mLayer )
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
	case MT::F:
	case MT::BI:
	case MT::FM:
		mv1 = mvX; mv2 = mvY; mv3 = mvZ;
		break;
	case MT::B:
	case MT::FI:
	case MT::FMI:
		mv1 = mvY; mv2 = mvX; mv3 = mvZ;
		break;
	case MT::L:
	case MT::RI:
	case MT::RMI:
		mv1 = mvZ; mv2 = mvY; mv3 = mvX;
		break;
	case MT::LI:
	case MT::R:
	case MT::RM:
		mv1 = mvZ; mv2 = mvX; mv3 = mvY;
		break;
	case MT::U:
	case MT::DI:
	case MT::UM:
		mv1 = mvY; mv2 = mvZ; mv3 = mvX;
		break;
	case MT::UI:
	case MT::D:
	case MT::UMI:
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
