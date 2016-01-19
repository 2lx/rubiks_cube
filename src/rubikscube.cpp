#include "all.h"

#include "rubikscube.h"
#include <cmath>

RubiksCube::RubiksCube()
{
	m_paramsMap[ MT_FRONT ] 	= new RCMoveParam(  0,  0,  1, true );
	m_paramsMap[ MT_FRONTINV ] 	= new RCMoveParam(  0,  0,  1, false );
	m_paramsMap[ MT_BACK ] 		= new RCMoveParam(  0,  0, -1, true );
	m_paramsMap[ MT_BACKINV ] 	= new RCMoveParam(  0,  0, -1, false );
	m_paramsMap[ MT_RIGHT ] 	= new RCMoveParam(  1,  0,  0, true );
	m_paramsMap[ MT_RIGHTINV ] 	= new RCMoveParam(  1,  0,  0, false );
	m_paramsMap[ MT_LEFT ] 		= new RCMoveParam( -1,  0,  0, true );
	m_paramsMap[ MT_LEFTINV ] 	= new RCMoveParam( -1,  0,  0, false );
	m_paramsMap[ MT_UP ] 		= new RCMoveParam(  0,  1,  0, true );
	m_paramsMap[ MT_UPINV ] 	= new RCMoveParam(  0,  1,  0, false );
	m_paramsMap[ MT_DOWN ] 		= new RCMoveParam(  0, -1,  0, true );
	m_paramsMap[ MT_DOWNINV ] 	= new RCMoveParam(  0, -1,  0, false );

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
	short int nx = m_paramsMap[ newRT ]->x();
	short int ny = m_paramsMap[ newRT ]->y();
	short int nz = m_paramsMap[ newRT ]->z();

	MyQuaternion quatT( 0, nx, ny, nz );
	MyQuaternion quatR = m_rotateQuat * quatT * m_rotateQuat.inverse();
	quatR = quatR.normalize();

	nx = static_cast< int >( quatR.x() );
	ny = static_cast< int >( quatR.y() );
	nz = static_cast< int >( quatR.z() );

	std::cout << quatR.x() << " " << quatR.y() << " " << quatR.z() << std::endl;

	for ( int i = 0; i < MT_COUNT; ++i )
	{
		RCMoveParam * mp = m_paramsMap[ RCMoveType( i ) ];

		if ( mp->x() == nx && mp->y() == ny && mp->z() == nz && mp->isClockwise() == m_paramsMap[ newRT ]->isClockwise() )
		{
			m_moveType = RCMoveType( i );
			break;
		}
	}
}

void RubiksCube::movePieces( const RCMoveType rt )
{
	CubePiece tmpPiece1;
	CubePiece tmpPiece2;
	const int k = PIECE_COUNT - 1;

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
	}

	tmpPiece1 = m_pieces[ mv1[ 0 ] ][ mv2[ 0 ] ][ mv3[ 0 ] ];
	tmpPiece2 = m_pieces[ mv1[ 1 ] ][ mv2[ 1 ] ][ mv3[ 1 ] ];
	m_pieces[ mv1[ 0 ] ][ mv2[ 0 ] ][ mv3[ 0 ] ] = m_pieces[ mv1[ 2 ] ][ mv2[ 2 ] ][ mv3[ 2 ] ];
	m_pieces[ mv1[ 1 ] ][ mv2[ 1 ] ][ mv3[ 1 ] ] = m_pieces[ mv1[ 3 ] ][ mv2[ 3 ] ][ mv3[ 3 ] ];
	m_pieces[ mv1[ 2 ] ][ mv2[ 2 ] ][ mv3[ 2 ] ] = m_pieces[ mv1[ 4 ] ][ mv2[ 4 ] ][ mv3[ 4 ] ];
	m_pieces[ mv1[ 3 ] ][ mv2[ 3 ] ][ mv3[ 3 ] ] = m_pieces[ mv1[ 5 ] ][ mv2[ 5 ] ][ mv3[ 5 ] ];
	m_pieces[ mv1[ 4 ] ][ mv2[ 4 ] ][ mv3[ 4 ] ] = m_pieces[ mv1[ 6 ] ][ mv2[ 6 ] ][ mv3[ 6 ] ];
	m_pieces[ mv1[ 5 ] ][ mv2[ 5 ] ][ mv3[ 5 ] ] = m_pieces[ mv1[ 7 ] ][ mv2[ 7 ] ][ mv3[ 7 ] ];
	m_pieces[ mv1[ 6 ] ][ mv2[ 6 ] ][ mv3[ 6 ] ] = tmpPiece1;
	m_pieces[ mv1[ 7 ] ][ mv2[ 7 ] ][ mv3[ 7 ] ] = tmpPiece2;

	for ( int i = 0; i < 8; ++i )
		m_pieces[ mv1[ i ] ][ mv2[ i ] ][ mv3[ i ] ].rotatePiece( rt );
}

void RubiksCube::drawObject()
{
	const GLfloat centerDiff = ( -1 * PIECE_COUNT ) / 2.0 + 0.5;
	vertexCube( 0, 0, 0, 2.2 );

	MyQuaternion quatTemp;
	GLfloat aX[ 3 ] = { 1.0, 0.0, 0.0 };
	GLfloat aY[ 3 ] = { 0.0, 1.0, 0.0 };
	GLfloat aZ[ 3 ] = { 0.0, 0.0, 1.0 };

	if ( m_moveType != MT_NONE )
	{
		RCMoveParam * mp = m_paramsMap[ m_moveType ];
		short int axisN = mp->axisN();

		if ( m_moveAngle >= 90 - ANGLE_DIFF )
		{
			GLfloat newAngle;
			if ( mp->isClockwiseAbs() )
				newAngle = 90 - m_moveAngle;
			else newAngle = m_moveAngle - 90;

			quatTemp.fromAxisAngle( aX[ axisN ], aY[ axisN ], aZ[ axisN ], newAngle );

			m_moveAngle = 0;
			movePieces( m_moveType );

			m_moveType = MT_NONE;
			m_moveQuat.reset();
		}
		else
		{
			quatTemp.fromAxisAngle( aX[ axisN ], aY[ axisN ], aZ[ axisN ],
					( mp->isClockwiseAbs() ) ? ANGLE_DIFF : -ANGLE_DIFF );

			m_moveQuat = m_moveQuat * quatTemp;
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
					short int axisN = mp->axisN();

					if (	( z == 2 && axisN == 2 &&  mp->isFront() ) ||
							( z == 0 && axisN == 2 && !mp->isFront() ) ||
							( x == 2 && axisN == 0 &&  mp->isFront() ) ||
							( x == 0 && axisN == 0 && !mp->isFront() ) ||
							( y == 2 && axisN == 1 &&  mp->isFront() ) ||
							( y == 0 && axisN == 1 && !mp->isFront() )
						)
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
