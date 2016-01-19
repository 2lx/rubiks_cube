#include "all.h"

#include "gameobject.h"

#ifdef MY_DEBUG
#include "output.h"
#endif

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	//dtor
}

bool GameObject::isRotating() const
{
	if ( m_rotateDir[ 0 ] == RD_NONE && m_rotateDir[ 1 ] == RD_NONE && m_rotateDir[ 2 ] == RD_NONE )
		return false;
	else return true;
};

void GameObject::setRotates( const RotateDirection newDirX, const RotateDirection newDirY, const RotateDirection newDirZ )
{
	if ( !isRotating() )
	{
		m_rotateDir[ 0 ] = newDirX;
		m_rotateDir[ 1 ] = newDirY;
		m_rotateDir[ 2 ] = newDirZ;
	}
}

void GameObject::rotateObject( )
{
	MyQuaternion quatTemp[ 3 ];
	GLfloat aX[ 3 ] = { 1.0, 0.0, 0.0 };
	GLfloat aY[ 3 ] = { 0.0, 1.0, 0.0 };
	GLfloat aZ[ 3 ] = { 0.0, 0.0, 1.0 };

	for ( int i = 0; i < 3; i++ )
	{
		if ( m_rotateDir[ i ] != RD_NONE )
		{
			if ( m_rotateAngle[ i ] >= 90 - ANGLE_DIFF )
			{
				GLfloat newAngle;
				if ( m_rotateDir[ i ] == RD_POSITIVE )
					newAngle = m_rotateAngle[ i ] - 90;
				else newAngle = 90 - m_rotateAngle[ i ];

				quatTemp[ i ].fromAxisAngle( aX[ i ], aY[ i ], aZ[ i ], newAngle );
				m_rotateQuat = m_rotateQuat * quatTemp[ i ];
				m_rotateQuat = m_rotateQuat.normalize();

				m_rotateAngle[ i ] = 0;
				m_rotateDir[ i ] = RD_NONE;
			}
			else
			{
				quatTemp[ i ].fromAxisAngle( aX[ i ], aY[ i ], aZ[ i ], ( m_rotateDir[ i ] == RD_POSITIVE ) ? -ANGLE_DIFF : ANGLE_DIFF );
				m_rotateQuat = m_rotateQuat * quatTemp[ i ];

				m_rotateAngle[ i ] += ANGLE_DIFF;
			}

#ifdef MY_DEBUG
//		GLfloat Matrix[16];
//		m_rotateQuat.getTrMatrix( Matrix );
//		writeMatrix( Matrix, 4, 4 );
#endif
		}
	}

	GLfloat MatrixRes[16];
	m_rotateQuat.getTrMatrix( MatrixRes );
	glMultMatrixf( MatrixRes );
}
