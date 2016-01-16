#include "all.h"

#include "rubikscube.h"

RubiksCube::RubiksCube()
{
	srand( time( 0 ) );
	for( int x = 0; x < PIECE_COUNT; ++x )
		for( int y = 0; y < PIECE_COUNT; ++y )
			for( int z = 0; z < PIECE_COUNT; ++z )
				m_pieces[ x ][ y ][ z ].colInd = rand() % COLOR_COUNT;
}

RubiksCube::~RubiksCube()
{
	//dtor
}

void RubiksCube::vertexCube( const float pX, const float pY, const float pZ, const float cubeSize )
{
	glBegin( GL_QUADS );

	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );  // Up
	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );

	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );	// Front
	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );

	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );	// Bottom
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );

	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );	// Back
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );

	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );	// Left
	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );

	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );	// Right
	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );

	glEnd();
}

void RubiksCube::drawPiece( const float pX, const float pY, const float pZ, const float cubeSize, const int colInd )
{
	if ( cubeSize < 1.0 )
	{
		glLineWidth( 1.5 );
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // Borders

		glColor3f( 0.1F, 0.1F, 0.1F );
		vertexCube( pX, pY, pZ, cubeSize );
	}

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // Fill

	glColor3f( COLOR_MATR[ colInd ][ 0 ], COLOR_MATR[ colInd ][ 1 ], COLOR_MATR[ colInd ][ 2 ] );
	vertexCube( pX, pY, pZ, cubeSize );
}

#ifdef MY_DEBUG
void writeMatrix( GLfloat * Matrix, const int length )
{
	for ( int i = 0; i < length; i++ )
	{
		if ( Matrix[ i ] >= 0 )
			std::cout << " ";

		std::cout << round( Matrix[ i ] * 1000000 ) / 1000000 << " ";

		if ( ( i % 4 ) == 3 )
			std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout.flush();
}
#endif

void RubiksCube::drawCube()
{
	static float cubeEdge = 1.0;
	static int cubeSizeSign = -1;

	cubeEdge = cubeEdge + cubeSizeSign * 0.01;
	if ( cubeEdge > 1.3 ) cubeSizeSign = -1;
	if ( cubeEdge < 0.7 ) cubeSizeSign = 1;
	cubeEdge = cubeEdge + cubeSizeSign * 0.01;

	for ( int x = 0; x < PIECE_COUNT; ++x )
		for ( int y = 0; y < PIECE_COUNT; ++y )
			for ( int z = 0; z < PIECE_COUNT; ++z )
				drawPiece( x - 1, y - 1, z - 1, ( cubeEdge > 1.0 ) ? 1.0 : cubeEdge,
						  m_pieces[ x ][ y ][ z ].colInd );
}
