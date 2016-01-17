#include "all.h"

#include "gameobject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	//dtor
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
	const GLfloat angleDiff = 8.0;

	MyQuaternion quatTemp[ 3 ];
	GLfloat aX[ 3 ] = { 1.0, 0.0, 0.0 };
	GLfloat aY[ 3 ] = { 0.0, 1.0, 0.0 };
	GLfloat aZ[ 3 ] = { 0.0, 0.0, 1.0 };

	for ( int i = 0; i < 3; i++ )
	{
		if ( m_rotateDir[ i ] != RD_NONE )
		{
			if ( m_SmoothAngle[ i ] >= 90 - angleDiff )
			{
				GLfloat newAngle;
				if ( m_rotateDir[ i ] == RD_POSITIVE )
					newAngle = m_SmoothAngle[ i ] - 90;
				else newAngle = 90 - m_SmoothAngle[ i ];

				quatTemp[ i ].fromAxisAngle( aX[ i ], aY[ i ], aZ[ i ], newAngle );
				m_rotateQuat = m_rotateQuat * quatTemp[ i ];

				m_SmoothAngle[ i ] = 0;
				m_rotateDir[ i ] = RD_NONE;
			}
			else
			{
				quatTemp[ i ].fromAxisAngle( aX[ i ], aY[ i ], aZ[ i ], ( m_rotateDir[ i ] == RD_POSITIVE ) ? -angleDiff : angleDiff );
				m_rotateQuat = m_rotateQuat * quatTemp[ i ];

				m_SmoothAngle[ i ] += angleDiff;
			}

#ifdef MY_DEBUG
//		GLfloat Matrix[16];
//		m_quatCurrent.getTrMatrix( Matrix );
//		writeMatrix( Matrix, 16 );
#endif
		}
	}

	GLfloat MatrixRes[16];
	m_rotateQuat.getTrMatrix( MatrixRes );
	glMultMatrixf( MatrixRes );
}
