#include "all.h"

#include "rubikscube.h"

RubiksCube::RubiksCube()
{
	srand( time( 0 ) );

	for( int i = 0; i < PIECE_COUNT; ++i )
		for( int j = 0; j < PIECE_COUNT; ++j )
		{
			m_pieces[ i ][ j ][ 2 ].colFront = 0;
			m_pieces[ i ][ 2 ][ j ].colTop = 1;
			m_pieces[ i ][ j ][ 0 ].colBack = 2;
			m_pieces[ i ][ 0 ][ j ].colBottom = 3;
			m_pieces[ 0 ][ i ][ j ].colLeft = 4;
			m_pieces[ 2 ][ i ][ j ].colRight = 5;
		}
}

RubiksCube::~RubiksCube()
{
	//dtor
}

void RubiksCube::vertexCube( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize )
{
	const GLfloat halfSize = cubeSize / 2;

	glBegin( GL_QUADS );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );  // Top
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
	const GLfloat halfSize = cubeSize / 2;

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

void RubiksCube::borderPiece( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize,
	const int x, const int y, const int z )
{
	const GLfloat halfSize = ( cubeSize / 2 ) + 0.0001;
	glLineWidth( 6 );

	glColor3f( 	COLOR_MATR[ 6 ][ 0 ],
				COLOR_MATR[ 6 ][ 1 ],
				COLOR_MATR[ 6 ][ 2 ] );

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glBegin( GL_QUADS );

	if ( m_pieces[ x ][ y ][ z ].colTop != -1 )
	{
		glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize ); // Top
		glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
		glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
	}

	if ( m_pieces[ x ][ y ][ z ].colFront != -1 )
	{
		glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );	// Front
		glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
		glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
	}

	if ( m_pieces[ x ][ y ][ z ].colBottom != -1 )
	{
		glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );	// Bottom
		glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
		glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );
	}

	if ( m_pieces[ x ][ y ][ z ].colBack != -1 )
	{
		glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );	// Back
		glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
		glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );
	}

	if ( m_pieces[ x ][ y ][ z ].colLeft != -1 )
	{
		glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );	// Left
		glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	}

	if ( m_pieces[ x ][ y ][ z ].colRight != -1 )
	{
		glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );	// Right
		glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
		glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
		glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );
	}

	glEnd();
}

void RubiksCube::borderCube( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize )
{
	glLineWidth( 1.5 );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // Borders

	glColor3f( COLOR_DARKGRAY[ 0 ], COLOR_DARKGRAY[ 1 ], COLOR_DARKGRAY[ 2 ] );
	vertexCube( pX, pY, pZ, cubeSize );
}

void RubiksCube::fillPiece( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize, const int colInd )
{
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // Fill

	glColor3f( COLOR_MATR[ colInd ][ 0 ], COLOR_MATR[ colInd ][ 1 ], COLOR_MATR[ colInd ][ 2 ] );
	vertexCube( pX, pY, pZ, cubeSize - 0.02 );
}

void RubiksCube::drawObject()
{
	static float cubeEdge = 1.0;
/*	static int cubeSizeSign = -1;

	cubeEdge = cubeEdge + cubeSizeSign * 0.01;
	if ( cubeEdge > 1.3 ) cubeSizeSign = -1;
	if ( cubeEdge < 0.7 ) cubeSizeSign = 1;
	cubeEdge = cubeEdge + cubeSizeSign * 0.01;
*/

//	borderPiece( 0, 0, 0, cubeEdge * 3 );
//	fillPiece( 0, 0, 0, cubeEdge * 3, 6 );

	for ( int x = 0; x < PIECE_COUNT; ++x )
		for ( int y = 0; y < PIECE_COUNT; ++y )
			for ( int z = 0; z < PIECE_COUNT; ++z )
			{
				vertexPiece( x- 1, y - 1, z - 1, cubeEdge, x, y, z );
				borderPiece( x- 1, y - 1, z - 1, cubeEdge, x, y, z );
			}

/*	for ( int x = 0; x < PIECE_COUNT; ++x )
		for ( int y = 0; y < PIECE_COUNT; ++y )
			for ( int z = 0; z < PIECE_COUNT; ++z )
				drawPiece( x - 1, y - 1, z - 1, ( cubeEdge > 1.0 ) ? 1.0 : cubeEdge,
						  m_pieces[ x ][ y ][ z ].colInd );
*/
}
