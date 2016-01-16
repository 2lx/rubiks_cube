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

void RubiksCube::vertexCube( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize )
{
	const GLfloat halfSize = cubeSize / 2;

	glBegin( GL_QUADS );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );  // Up
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );

	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );	// Front
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );

	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );	// Bottom
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );	// Back
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );

	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );	// Left
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );	// Right
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	glEnd();
}

void RubiksCube::drawPiece( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize, const int colInd )
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

void RubiksCube::drawObject()
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
