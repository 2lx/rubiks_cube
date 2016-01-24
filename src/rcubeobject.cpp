#include "all.h"

#include "rcubeobject.h"
#include "rcubeparams.h"

using namespace RC;

RCubeObject::RCubeObject()
{
	m_RCModel = new CubeModel();
}

RCubeObject::~RCubeObject()
{
	delete m_RCModel;
}

void RCubeObject::setMove( const RCMoveType newRT )
{
	MyQuaternion quatT( MoveParams::vec( newRT ) );

	MyQuaternion quatR = m_rotateQuat * quatT * m_rotateQuat.inverse();
	quatR = quatR.normalize();

	Vector3D vec = Vector3D( quatR.x(), quatR.y(), quatR.z() );
	m_moveType = MoveParams::getMTypeForPars( vec, MoveParams::clockwise( newRT ) );
}

Point3D RCubeObject::getCubeByCoord( const Point3D p ) const
{
    int x, y, z;

    x = ( int ) ( p.x() + 1.5 );
    if ( x > CUBIE_COUNT || x < 0 )
		return Point3D( -1, -1, -1 );
    else
    {
		y = ( int ) ( p.y() + 1.5 );
		if ( y > CUBIE_COUNT || y < 0 )
			return Point3D( -1, -1, -1 );
		else
		{
			z = ( int ) ( p.z() + 1.5 );
			if ( z > CUBIE_COUNT || z < 0 )
				return Point3D( -1, -1, -1 );
		}
    }

    return Point3D( x, y, z );
}

void RCubeObject::setMoveByCoords( const Point3D pBeg, const Point3D pEnd )
{
//	std::cout << pBeg.x() << " " << pBeg.y() << " " << pBeg.z() << std::endl;
//	std::cout << pEnd.x() << " " << pEnd.y() << " " << pEnd.z() << std::endl;
	Vector3D vecTangent;
    if ( std::abs( pBeg.x() - 1.45 ) < 0.14 && std::abs( pEnd.x() - 1.45 ) < 0.14 )
		vecTangent.setXYZ(  1,  0,  0 );
	else if ( std::abs( pBeg.x() + 1.45 ) < 0.14 && std::abs( pEnd.x() + 1.45 ) < 0.14 )
		vecTangent.setXYZ( -1,  0,  0 );
    else if ( std::abs( pBeg.y() - 1.45 ) < 0.14 && std::abs( pEnd.y() - 1.45 ) < 0.14 )
		vecTangent.setXYZ(  0,  1,  0 );
	else if ( std::abs( pBeg.y() + 1.45 ) < 0.14 && std::abs( pEnd.y() + 1.45 ) < 0.14 )
		vecTangent.setXYZ(  0, -1,  0 );
    else if ( std::abs( pBeg.z() - 1.45 ) < 0.14 && std::abs( pEnd.z() - 1.45 ) < 0.14 )
		vecTangent.setXYZ(  0,  0,  1 );
	else if ( std::abs( pBeg.z() + 1.45 ) < 0.14 && std::abs( pEnd.z() + 1.45 ) < 0.14 )
		vecTangent.setXYZ(  0,  0, -1 );
	else return;

//	std::cout << vecTangent.x() << " " << vecTangent.y() << " " << vecTangent.z() << std::endl;
	Point3D pcBeg =  getCubeByCoord( pBeg );
	Point3D pcEnd =  getCubeByCoord( pEnd );

	if ( pcBeg.x() != -1 && pcEnd.x() != -1 )
	{
		Point3D pcRes = getCubeByCoord( pEnd ) - getCubeByCoord( pBeg );
		if ( pcRes.x() != 0 && pcRes.y() != 0 || pcRes.x() != 0 && pcRes.z() != 0 || pcRes.z() != 0 && pcRes.y() != 0 )
			return;

		pcRes = pcRes / pcRes.length();
		pcRes.normalize();
//		std::cout << pcRes.x() << " " << pcRes.y() << " " << pcRes.z() << std::endl;
		Vector3D vecMove( pcRes.x(), pcRes.y(), pcRes.z() );
		Vector3D vecResult;

		MyQuaternion rQt;
		if ( vecTangent.x() == 0 && vecMove.x() == 0 )
			if ( pcBeg.x() > 1 )
			{
				vecResult.setXYZ( 1, 0, 0 );
				rQt.fromAxisAngle( 0, 1, 0, -90 );
			}
			else
			{
				vecResult.setXYZ( -1, 0, 0 );
				rQt.fromAxisAngle( 0, 1, 0, 90 );
			}
		else if ( vecTangent.y() == 0 && vecMove.y() == 0 )
			if ( pcBeg.y() > 1 )
			{
				vecResult.setXYZ( 0, 1, 0 );
				rQt.fromAxisAngle( 1, 0, 0, 90 );
			}
			else
			{
				vecResult.setXYZ( 0, -1, 0 );
				rQt.fromAxisAngle( 1, 0, 0, -90 );
			}
		else if ( vecTangent.z() == 0 && vecMove.z() == 0 )
			if ( pcBeg.z() > 1 )
			{
				vecResult.setXYZ( 0, 0, 1 );
				rQt.fromAxisAngle( 1, 0, 0, 0 );
			}
			else
			{
				vecResult.setXYZ( 0, 0, -1 );
				rQt.fromAxisAngle( 1, 0, 0, 180 );
			}

		MyQuaternion qBeg = MyQuaternion( 0, pBeg.x(), pBeg.y(), pBeg.z() );
		MyQuaternion qEnd = MyQuaternion( 0, pEnd.x(), pEnd.y(), pEnd.z() );
		MyQuaternion quatRB = rQt * qBeg * rQt.inverse();
		MyQuaternion quatRE = rQt * qEnd * rQt.inverse();

//		std::cout << quatRB.x() << " " << quatRB.y() << " " << quatRB.z() << std::endl;
//		std::cout << quatRE.x() << " " << quatRE.y() << " " << quatRE.z() << std::endl;

		bool isCW = ( quatRE.x() * quatRB.y() - quatRB.x() * quatRE.y() ) >= 0;

		m_moveType = MoveParams::getMTypeForPars( vecResult, isCW );
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
			quatTemp.fromAxisAngle( abs( MoveParams::vec( m_moveType ).x() ),
				abs( MoveParams::vec( m_moveType ).y() ), abs( MoveParams::vec( m_moveType ).z() ),
				( MoveParams::vec( m_moveType ).isFirstOctant() == MoveParams::clockwise( m_moveType ) ) ? ANGLE_DIFF : -ANGLE_DIFF );

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
					if (	( z == 2 && MoveParams::vec( m_moveType ).z() > 0 ) ||
							( z == 0 && MoveParams::vec( m_moveType ).z() < 0 ) ||
							( x == 2 && MoveParams::vec( m_moveType ).x() > 0 ) ||
							( x == 0 && MoveParams::vec( m_moveType ).x() < 0 ) ||
							( y == 2 && MoveParams::vec( m_moveType ).y() > 0 ) ||
							( y == 0 && MoveParams::vec( m_moveType ).y() < 0 )
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

	glColor3f( Colors::colR( RC_FG ), Colors::colG( RC_FG ), Colors::colB( RC_FG ) );
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

	// Up
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_UP ),
			m_RCModel->cubie( x, y, z ).colG( AX_UP ), m_RCModel->cubie( x, y, z ).colB( AX_UP ) );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );

	if ( y == 2 && !isAxisVisible( AX_UP ) )
	{
		glVertex3f( pX + halfSize, pY + 2.0 + halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY + 2.0 + halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY + 2.0 + halfSize, pZ + halfSize );
		glVertex3f( pX + halfSize, pY + 2.0 + halfSize, pZ + halfSize );
	}

	// Front
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_FRONT ),
			m_RCModel->cubie( x, y, z ).colG( AX_FRONT ), m_RCModel->cubie( x, y, z ).colB( AX_FRONT ) );

	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );

	if ( z == 2 && !isAxisVisible( AX_FRONT ) )
	{
		glVertex3f( pX + halfSize, pY + halfSize, pZ + 2.0 + halfSize );
		glVertex3f( pX - halfSize, pY + halfSize, pZ + 2.0 + halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ + 2.0 + halfSize );
		glVertex3f( pX + halfSize, pY - halfSize, pZ + 2.0 + halfSize );
	}

	// Down
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_DOWN ),
			m_RCModel->cubie( x, y, z ).colG( AX_DOWN ), m_RCModel->cubie( x, y, z ).colB( AX_DOWN ) );

	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	if ( y == 0 && !isAxisVisible( AX_DOWN ) )
	{
		glVertex3f( pX + halfSize, pY - 2.0 - halfSize, pZ + halfSize );
		glVertex3f( pX - halfSize, pY - 2.0 - halfSize, pZ + halfSize );
		glVertex3f( pX - halfSize, pY - 2.0 - halfSize, pZ - halfSize );
		glVertex3f( pX + halfSize, pY - 2.0 - halfSize, pZ - halfSize );
	}

	// Back
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_BACK ),
			m_RCModel->cubie( x, y, z ).colG( AX_BACK ), m_RCModel->cubie( x, y, z ).colB( AX_BACK ) );

	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );

	if ( z == 0 && !isAxisVisible( AX_BACK ) )
	{
		glVertex3f( pX + halfSize, pY - halfSize, pZ - 2.0 - halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ - 2.0 - halfSize );
		glVertex3f( pX - halfSize, pY + halfSize, pZ - 2.0 - halfSize );
		glVertex3f( pX + halfSize, pY + halfSize, pZ - 2.0 - halfSize );
	}

	// Left
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_LEFT ),
			m_RCModel->cubie( x, y, z ).colG( AX_LEFT ), m_RCModel->cubie( x, y, z ).colB( AX_LEFT ) );

	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );

	if ( x == 0 && !isAxisVisible( AX_LEFT ) )
	{
		glVertex3f( pX - 2.0 - halfSize, pY + halfSize, pZ - halfSize );
		glVertex3f( pX - 2.0 - halfSize, pY + halfSize, pZ + halfSize );
		glVertex3f( pX - 2.0 - halfSize, pY - halfSize, pZ + halfSize );
		glVertex3f( pX - 2.0 - halfSize, pY - halfSize, pZ - halfSize );
	}

	// Right
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_RIGHT ),
			m_RCModel->cubie( x, y, z ).colG( AX_RIGHT ), m_RCModel->cubie( x, y, z ).colB( AX_RIGHT ) );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	if ( x == 2 && !isAxisVisible( AX_RIGHT ) )
	{
		glVertex3f( pX + 2.0 + halfSize, pY + halfSize, pZ - halfSize );
		glVertex3f( pX + 2.0 + halfSize, pY + halfSize, pZ + halfSize );
		glVertex3f( pX + 2.0 + halfSize, pY - halfSize, pZ + halfSize );
		glVertex3f( pX + 2.0 + halfSize, pY - halfSize, pZ - halfSize );
	}

	glEnd();
}
