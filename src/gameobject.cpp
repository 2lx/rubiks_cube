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

bool GameObject::isMoved() const
{
	if ( m_moveDir[ 0 ] == MD_NONE && m_moveDir[ 1 ] == MD_NONE && m_moveDir[ 2 ] == MD_NONE )
		return false;
	else return true;
};

void GameObject::setMoves( const MoveDirection newDirX, const MoveDirection newDirY, const MoveDirection newDirZ )
{
	if ( !isMoved() )
	{
		m_moveDir[ 0 ] = newDirX;
		m_moveDir[ 1 ] = newDirY;
		m_moveDir[ 2 ] = newDirZ;
	}
}

void GameObject::moveObject( )
{
	const GLfloat angleDiff = 8.0;

	MyQuaternion quatTemp[ 3 ];
	GLfloat aX[ 3 ] = { 1.0, 0.0, 0.0 };
	GLfloat aY[ 3 ] = { 0.0, 1.0, 0.0 };
	GLfloat aZ[ 3 ] = { 0.0, 0.0, 1.0 };

	for ( int i = 0; i < 3; i++ )
	{
		if ( m_moveDir[ i ] != MD_NONE )
		{
			if ( m_SmoothAngle[ i ] >= 90 - angleDiff )
			{
				GLfloat newAngle;
				if ( m_moveDir[ i ] == MD_POSITIVE )
					newAngle = m_SmoothAngle[ i ] - 90;
				else newAngle = 90 - m_SmoothAngle[ i ];

				quatTemp[ i ].fromAxisAngle( aX[ i ], aY[ i ], aZ[ i ], newAngle );
				m_quatCurrent = m_quatCurrent * quatTemp[ i ];

				m_SmoothAngle[ i ] = 0;
				m_moveDir[ i ] = MD_NONE;
			}
			else
			{
				quatTemp[ i ].fromAxisAngle( aX[ i ], aY[ i ], aZ[ i ], ( m_moveDir[ i ] == MD_POSITIVE ) ? -angleDiff : angleDiff );
				m_quatCurrent = m_quatCurrent * quatTemp[ i ];

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
	m_quatCurrent.getTrMatrix( MatrixRes );
	glMultMatrixf( MatrixRes );
}
