#include "all.h"

#include "rubikscube.h"

RubiksCube::RubiksCube()
{
	srand( time( 0 ) );

	for( int i = 0; i < PIECE_COUNT; ++i )
		for( int j = 0; j < PIECE_COUNT; ++j )
		{
			m_pieces[ i ][ j ][ PIECE_COUNT - 1 ].colFront = 0;
			m_pieces[ i ][ PIECE_COUNT - 1 ][ j ].colTop = 1;
			m_pieces[ i ][ j ][ 0 ].colBack = 2;
			m_pieces[ i ][ 0 ][ j ].colBottom = 3;
			m_pieces[ 0 ][ i ][ j ].colLeft = 4;
			m_pieces[ PIECE_COUNT - 1 ][ i ][ j ].colRight = 5;
		}
}

RubiksCube::~RubiksCube()
{
	//dtor
}

void RubiksCube::vertexCube( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize )
{
	const GLfloat halfSize = cubeSize * PIECE_COUNT / 2.0 - 0.1;

	glColor3f( COLOR_MATR[ 6 ][ 0 ], COLOR_MATR[ 6 ][ 1 ], COLOR_MATR[ 6 ][ 2 ] );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBegin( GL_QUADS );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize ); // Top
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

void RubiksCube::vertexPiece( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize,
	const int x, const int y, const int z )
{
	const GLfloat halfSize = cubeSize / 2 - 0.05;

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBegin( GL_QUADS );

	if ( m_pieces[ x ][ y ][ z ].colTop != -1 )
	{
		glColor3f( 	COLOR_MATR[ m_pieces[ x ][ y ][ z ].colTop ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colTop ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colTop ][ 2 ] );

		glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize ); // Top
		glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
		glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
	}

	if ( m_pieces[ x ][ y ][ z ].colFront != -1 )
	{
		glColor3f( 	COLOR_MATR[ m_pieces[ x ][ y ][ z ].colFront ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colFront ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colFront ][ 2 ] );

		glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );	// Front
		glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
		glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
	}

	if ( m_pieces[ x ][ y ][ z ].colBottom != -1 )
	{
		glColor3f( 	COLOR_MATR[ m_pieces[ x ][ y ][ z ].colBottom ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colBottom ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colBottom ][ 2 ] );

		glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );	// Bottom
		glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
		glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );
	}

	if ( m_pieces[ x ][ y ][ z ].colBack != -1 )
	{
		glColor3f( 	COLOR_MATR[ m_pieces[ x ][ y ][ z ].colBack ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colBack ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colBack ][ 2 ] );

		glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );	// Back
		glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
		glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );
	}

	if ( m_pieces[ x ][ y ][ z ].colLeft != -1 )
	{
		glColor3f( 	COLOR_MATR[ m_pieces[ x ][ y ][ z ].colLeft ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colLeft ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colLeft ][ 2 ] );

		glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );	// Left
		glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	}

	if ( m_pieces[ x ][ y ][ z ].colRight != -1 )
	{
		glColor3f( 	COLOR_MATR[ m_pieces[ x ][ y ][ z ].colRight ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colRight ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colRight ][ 2 ] );

		glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );	// Right
		glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
		glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
		glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );
	}

	glEnd();
}

void RubiksCube::drawObject()
{
	static GLfloat cubeEdge = 1.0;
	const GLfloat centerDiff = ( -1 * PIECE_COUNT ) / 2.0 + 0.5;

	vertexCube( 0, 0, 0, 1 );

	for ( int x = 0; x < PIECE_COUNT; ++x )
		for ( int y = 0; y < PIECE_COUNT; ++y )
			for ( int z = 0; z < PIECE_COUNT; ++z )
			{
				vertexPiece( x + centerDiff, y + centerDiff, z + centerDiff, cubeEdge, x, y, z );
			}
}
