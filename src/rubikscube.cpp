#include "all.h"

#include "rubikscube.h"

RubiksCube::RubiksCube()
{
	MyQuaternion mq;

	mq.fromAxisAngle( 0, 0, 1, 90 );
	m_paramsMap[ MT_FRONT ] = new RCMoveParam( 2, true, true, mq );
	m_paramsMap[ MT_BACK ] = new RCMoveParam( 2, true, false, mq );
	mq.fromAxisAngle( 0, 0, 1, -90 );
	m_paramsMap[ MT_FRONTINV ] = new RCMoveParam( 2, false, true, mq );
	m_paramsMap[ MT_BACKINV ] = new RCMoveParam( 2, false, false, mq );

	srand( time( 0 ) );
	const int k = PIECE_COUNT - 1;

	for( int i = 0; i < PIECE_COUNT; ++i )
		for( int j = 0; j < PIECE_COUNT; ++j )
		{
			m_pieces[ i ][ j ][ k ].colFront = 0;
			m_pieces[ i ][ k ][ j ].colUp = 1;
			m_pieces[ i ][ j ][ 0 ].colBack = 2;
			m_pieces[ i ][ 0 ][ j ].colDown = 3;
			m_pieces[ 0 ][ i ][ j ].colLeft = 4;
			m_pieces[ k ][ i ][ j ].colRight = 5;
		}
}

RubiksCube::~RubiksCube()
{
	for ( std::map< RCMoveType, RCMoveParam * >::iterator it = m_paramsMap.begin(); it != m_paramsMap.end(); ++it )
		delete it->second;
}

void RubiksCube::setMove( const RCMoveType newRT )
{
	m_moveType = newRT;
}

void RubiksCube::movePieces( const RCMoveType rt )
{
	CubePiece tmpPiece1;
	CubePiece tmpPiece2;
	const int k = PIECE_COUNT - 1;

	if ( rt == MT_FRONT )
	{
		tmpPiece1 = m_pieces[ 0 ][ 0 ][ k ];
		tmpPiece2 = m_pieces[ 1 ][ 0 ][ k ];
		m_pieces[ 0 ][ 0 ][ k ] = m_pieces[ 2 ][ 0 ][ k ];
		m_pieces[ 1 ][ 0 ][ k ] = m_pieces[ 2 ][ 1 ][ k ];
		m_pieces[ 2 ][ 0 ][ k ] = m_pieces[ 2 ][ 2 ][ k ];
		m_pieces[ 2 ][ 1 ][ k ] = m_pieces[ 1 ][ 2 ][ k ];
		m_pieces[ 2 ][ 2 ][ k ] = m_pieces[ 0 ][ 2 ][ k ];
		m_pieces[ 1 ][ 2 ][ k ] = m_pieces[ 0 ][ 1 ][ k ];
		m_pieces[ 0 ][ 2 ][ k ] = tmpPiece1;
		m_pieces[ 0 ][ 1 ][ k ] = tmpPiece2;

		for ( int i = 0; i < PIECE_COUNT; ++i )
			for ( int j = 0; j < PIECE_COUNT; ++j )
				m_pieces[ i ][ j ][ k ].rotatePiece( rt );
	}
	else if ( rt == MT_FRONTINV )
	{
		tmpPiece1 = m_pieces[ 0 ][ 0 ][ k ];
		tmpPiece2 = m_pieces[ 0 ][ 1 ][ k ];
		m_pieces[ 0 ][ 0 ][ k ] = m_pieces[ 0 ][ 2 ][ k ];
		m_pieces[ 0 ][ 1 ][ k ] = m_pieces[ 1 ][ 2 ][ k ];
		m_pieces[ 0 ][ 2 ][ k ] = m_pieces[ 2 ][ 2 ][ k ];
		m_pieces[ 1 ][ 2 ][ k ] = m_pieces[ 2 ][ 1 ][ k ];
		m_pieces[ 2 ][ 2 ][ k ] = m_pieces[ 2 ][ 0 ][ k ];
		m_pieces[ 2 ][ 1 ][ k ] = m_pieces[ 1 ][ 0 ][ k ];
		m_pieces[ 2 ][ 0 ][ k ] = tmpPiece1;
		m_pieces[ 1 ][ 0 ][ k ] = tmpPiece2;

		for ( int i = 0; i < PIECE_COUNT; ++i )
			for ( int j = 0; j < PIECE_COUNT; ++j )
				m_pieces[ i ][ j ][ k ].rotatePiece( rt );
	}

}

void RubiksCube::drawObject()
{
	const GLfloat centerDiff = ( -1 * PIECE_COUNT ) / 2.0 + 0.5;
	vertexCube( 0, 0, 0, 2.2 );

	MyQuaternion quatTemp;
	GLfloat aX[ 3 ] = { 1.0, 0.0, 0.0 };
	GLfloat aY[ 3 ] = { 0.0, 1.0, 0.0 };
	GLfloat aZ[ 3 ] = { 0.0, 0.0, 1.0 };

//	m_moveQuat = m_rotateQuat.inverse();

	if ( m_moveType != MT_NONE )
	{
		RCMoveParam * mp = m_paramsMap[ m_moveType ];

		if ( m_moveAngle >= 90 - ANGLE_DIFF )
		{
			GLfloat newAngle;
			if ( mp->isClockwise )
				newAngle = 90 - m_moveAngle;
			else newAngle = m_moveAngle - 90;

			quatTemp.fromAxisAngle( aX[ mp->axisN ], aY[ mp->axisN ], aZ[ mp->axisN ], newAngle );
	//		m_moveQuat = m_moveQuat * quatTemp;

			m_moveAngle = 0;
			movePieces( m_moveType );

			m_moveType = MT_NONE;
			m_moveQuat.reset();
		}
		else
		{
			quatTemp.fromAxisAngle( aX[ mp->axisN ], aY[ mp->axisN ], aZ[ mp->axisN ],
					( mp->isClockwise ) ? ANGLE_DIFF : -ANGLE_DIFF );

			m_moveQuat = m_moveQuat * quatTemp;
//			m_moveQuat.reset();
//			m_moveQuat = m_moveQuat * mp->quat; // test mp->quat
			m_moveAngle += ANGLE_DIFF;
		}
	}

	for ( int x = 0; x < PIECE_COUNT; ++x )
		for ( int y = 0; y < PIECE_COUNT; ++y )
			for ( int z = 0; z < PIECE_COUNT; ++z )
			{
				if ( m_moveType != MT_NONE )
				{
					RCMoveParam * mp = m_paramsMap[ m_moveType ];

					if ( z == 2 && mp->axisN == 2 && mp->isFront )
					{
						glPushMatrix();

						GLfloat MatrixRes[ 16 ];
						m_moveQuat.getTrMatrix( MatrixRes );
						glMultMatrixf( MatrixRes );
						vertexPiece( x + centerDiff, y + centerDiff, z + centerDiff, CUBE_EDGE, x, y, z );

						glPopMatrix();
					}
					else vertexPiece( x + centerDiff, y + centerDiff, z + centerDiff, CUBE_EDGE, x, y, z );
				}
				else vertexPiece( x + centerDiff, y + centerDiff, z + centerDiff, CUBE_EDGE, x, y, z );
			}
}

void RubiksCube::vertexCube( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize )
{
	const GLfloat halfSize = cubeSize / 2.0 - 0.1;

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


		if ( m_pieces[ x ][ y ][ z ].colUp == -1 )
			glColor3f( COLOR_MATR[ 6 ][ 0 ], COLOR_MATR[ 6 ][ 1 ], COLOR_MATR[ 6 ][ 2 ] );
		else glColor3f( COLOR_MATR[ m_pieces[ x ][ y ][ z ].colUp ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colUp ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colUp ][ 2 ] );

		glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize ); // Up
		glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
		glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );


	if ( m_pieces[ x ][ y ][ z ].colFront == -1 )
		glColor3f( COLOR_MATR[ 6 ][ 0 ], COLOR_MATR[ 6 ][ 1 ], COLOR_MATR[ 6 ][ 2 ] );
	else glColor3f( COLOR_MATR[ m_pieces[ x ][ y ][ z ].colFront ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colFront ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colFront ][ 2 ] );

	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );	// Front
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );

	if ( m_pieces[ x ][ y ][ z ].colDown == -1 )
		glColor3f( COLOR_MATR[ 6 ][ 0 ], COLOR_MATR[ 6 ][ 1 ], COLOR_MATR[ 6 ][ 2 ] );
	else glColor3f( COLOR_MATR[ m_pieces[ x ][ y ][ z ].colDown ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colDown ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colDown ][ 2 ] );

	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );	// Down
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	if ( m_pieces[ x ][ y ][ z ].colBack == -1 )
		glColor3f( COLOR_MATR[ 6 ][ 0 ], COLOR_MATR[ 6 ][ 1 ], COLOR_MATR[ 6 ][ 2 ] );
	else glColor3f( COLOR_MATR[ m_pieces[ x ][ y ][ z ].colBack ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colBack ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colBack ][ 2 ] );

	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );	// Back
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );

	if ( m_pieces[ x ][ y ][ z ].colLeft == -1 )
		glColor3f( COLOR_MATR[ 6 ][ 0 ], COLOR_MATR[ 6 ][ 1 ], COLOR_MATR[ 6 ][ 2 ] );
	else glColor3f( COLOR_MATR[ m_pieces[ x ][ y ][ z ].colLeft ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colLeft ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colLeft ][ 2 ] );

	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );	// Left
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );

	if ( m_pieces[ x ][ y ][ z ].colRight == -1 )
		glColor3f( COLOR_MATR[ 6 ][ 0 ], COLOR_MATR[ 6 ][ 1 ], COLOR_MATR[ 6 ][ 2 ] );
	else glColor3f( COLOR_MATR[ m_pieces[ x ][ y ][ z ].colRight ][ 0 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colRight ][ 1 ],
					COLOR_MATR[ m_pieces[ x ][ y ][ z ].colRight ][ 2 ] );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );	// Right
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	glEnd();
}
