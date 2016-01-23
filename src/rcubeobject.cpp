#include "all.h"

#include "rcubeobject.h"

RCubeObject::RCubeObject()
{
	m_RCModel = new RCubeModel();
}

RCubeObject::~RCubeObject()
{
	delete m_RCModel;
}

void RCubeObject::setMove( const RCMoveType newRT )
{
	MyQuaternion quatT( RCMoveTable::vec( newRT ) );

	MyQuaternion quatR = m_rotateQuat * quatT * m_rotateQuat.inverse();
	quatR = quatR.normalize();

	Vector3D vec = Vector3D( quatR.x(), quatR.y(), quatR.z() );

	for ( int i = MT_FIRST; i < MT_COUNT; ++i )
	{
		if ( RCMoveTable::vec( RCMoveType( i ) ) == vec && RCMoveTable::clockwise( RCMoveType( i ) ) == RCMoveTable::clockwise( newRT ) )
		{
			m_moveType = RCMoveType( i );
			break;
		}
	}
}

void RCubeObject::drawObject()
{
	const GLfloat centerDiff = ( -1 * CUBIE_COUNT ) / 2.0 + 0.5;
	setCubeVertices( 0, 0, 0, 2.2 );

	MyQuaternion quatTemp;

	if ( m_moveType != MT_NONE )
	{
		if ( m_moveAngle >= 90 - ANGLE_DIFF )
		{
			m_moveAngle = 0;
			m_RCModel->moveCubies( m_moveType );

			m_moveType = MT_NONE;
			m_moveQuat.reset();
		}
		else
		{
			quatTemp.fromAxisAngle( abs( RCMoveTable::vec( m_moveType ).x() ),
				abs( RCMoveTable::vec( m_moveType ).y() ), abs( RCMoveTable::vec( m_moveType ).z() ),
				( RCMoveTable::vec( m_moveType ).isFirstOctant() == RCMoveTable::clockwise( m_moveType ) ) ? ANGLE_DIFF : -ANGLE_DIFF );

			m_moveQuat = m_moveQuat * quatTemp;
			m_moveAngle += ANGLE_DIFF;
		}
	}

	for ( int x = 0; x < CUBIE_COUNT; ++x )
		for ( int y = 0; y < CUBIE_COUNT; ++y )
			for ( int z = 0; z < CUBIE_COUNT; ++z )
			{
				if ( m_moveType != MT_NONE )
				{
					if (	( z == 2 && RCMoveTable::vec( m_moveType ).z() > 0 ) ||
							( z == 0 && RCMoveTable::vec( m_moveType ).z() < 0 ) ||
							( x == 2 && RCMoveTable::vec( m_moveType ).x() > 0 ) ||
							( x == 0 && RCMoveTable::vec( m_moveType ).x() < 0 ) ||
							( y == 2 && RCMoveTable::vec( m_moveType ).y() > 0 ) ||
							( y == 0 && RCMoveTable::vec( m_moveType ).y() < 0 )
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

void RCubeObject::setCubeVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize ) const
{
	const GLfloat halfSize = cubeSize / 2.0 - 0.1;

	glColor3f( COLOR_MATR[ 6 ][ 0 ], COLOR_MATR[ 6 ][ 1 ], COLOR_MATR[ 6 ][ 2 ] );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBegin( GL_QUADS );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );  // Up
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );

	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );	// Front
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );

	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );	// Down
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

void RCubeObject::setCubieVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize,
	const int x, const int y, const int z ) const
{
	const GLfloat halfSize = cubeSize / 2 - 0.05;

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBegin( GL_QUADS );

	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_UP ),
			m_RCModel->cubie( x, y, z ).colG( AX_UP ), m_RCModel->cubie( x, y, z ).colB( AX_UP ) );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );  // Up
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
