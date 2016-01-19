#include "all.h"

#include "rubikscube.h"

RubiksCube::RubiksCube()
{
	m_RCModel = new RCubeModel();

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
}

RubiksCube::~RubiksCube()
{
	delete m_RCModel;

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

	std::cout << quatR.x() << " " << quatR.y() << " " << quatR.z() << std::endl;

	nx = static_cast< int >( quatR.x() );
	ny = static_cast< int >( quatR.y() );
	nz = static_cast< int >( quatR.z() );

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

void RubiksCube::drawObject()
{
	const GLfloat centerDiff = ( -1 * PIECE_COUNT ) / 2.0 + 0.5;
	setCubeVertices( 0, 0, 0, 2.2 );

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
			m_RCModel->moveCubies( m_moveType );

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
						setCubieVertices( x + centerDiff, y + centerDiff, z + centerDiff, CUBE_EDGE, x, y, z );

						glPopMatrix();
					}
					else setCubieVertices( x + centerDiff, y + centerDiff, z + centerDiff, CUBE_EDGE, x, y, z );
				}
				else setCubieVertices( x + centerDiff, y + centerDiff, z + centerDiff, CUBE_EDGE, x, y, z );
			}
}

void RubiksCube::setCubeVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize )
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

void RubiksCube::setCubieVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize,
	const int x, const int y, const int z )
{
	const GLfloat halfSize = cubeSize / 2 - 0.05;

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBegin( GL_QUADS );

	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_UP ),
			m_RCModel->cubie( x, y, z ).colG( AX_UP ), m_RCModel->cubie( x, y, z ).colB( AX_UP ) );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize ); // Up
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );


	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_FRONT ),
			m_RCModel->cubie( x, y, z ).colG( AX_FRONT ), m_RCModel->cubie( x, y, z ).colB( AX_FRONT ) );

	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );	// Front
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );

	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_DOWN ),
			m_RCModel->cubie( x, y, z ).colG( AX_DOWN ), m_RCModel->cubie( x, y, z ).colB( AX_DOWN ) );

	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );	// Down
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_BACK ),
			m_RCModel->cubie( x, y, z ).colG( AX_BACK ), m_RCModel->cubie( x, y, z ).colB( AX_BACK ) );

	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );	// Back
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );

	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_LEFT ),
			m_RCModel->cubie( x, y, z ).colG( AX_LEFT ), m_RCModel->cubie( x, y, z ).colB( AX_LEFT ) );

	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );	// Left
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );

	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_RIGHT ),
			m_RCModel->cubie( x, y, z ).colG( AX_RIGHT ), m_RCModel->cubie( x, y, z ).colB( AX_RIGHT ) );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );	// Right
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	glEnd();
}
