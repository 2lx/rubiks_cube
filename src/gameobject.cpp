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

void GameObject::rotateObject( const MoveDirection newDirX, const MoveDirection newDirY, const MoveDirection newDirZ )
{
	const GLfloat angleDiff = 8.0;

	if ( m_moveDirX == MD_NONE ) m_moveDirX = newDirX;
	if ( m_moveDirY == MD_NONE ) m_moveDirY = newDirY;
	if ( m_moveDirZ == MD_NONE ) m_moveDirZ = newDirZ;

	MyQuaternion quatTempX;
	MyQuaternion quatTempY;
	MyQuaternion quatTempZ;

	if ( ( m_moveDirX != MD_NONE ) && ( ( ( m_moveDirY == MD_NONE ) && ( m_moveDirZ == MD_NONE ) ) || ( m_XSmoothAngle > 0 ) ) )
	{
		if ( m_XSmoothAngle >= 90 - angleDiff )
		{
			quatTempX.fromAxisAngle( 1.0, 0.0, 0.0, ( m_moveDirX == MD_POSITIVE ) ? -( 90 - m_XSmoothAngle ) : ( 90 - m_XSmoothAngle ) );
			m_quatCurrent = m_quatCurrent * quatTempX;

			m_XSmoothAngle = 0;
			m_moveDirX = MD_NONE;
		}
		else
		{
			quatTempX.fromAxisAngle( 1.0, 0.0, 0.0, ( m_moveDirX == MD_POSITIVE ) ? -angleDiff : angleDiff );
			m_quatCurrent = m_quatCurrent * quatTempX;

			m_XSmoothAngle += angleDiff;
		}

#ifdef MY_DEBUG
		GLfloat Matrix[16];
		m_quatCurrent.getTrMatrix( Matrix );
		writeMatrix( Matrix, 16 );
#endif
	}
	else if ( ( m_moveDirY != MD_NONE ) && ( ( m_moveDirZ == MD_NONE ) || ( m_YSmoothAngle > 0 ) ) )
	{
		if ( m_YSmoothAngle >= 90 - angleDiff )
		{
			quatTempY.fromAxisAngle( 0.0, 1.0, 0.0, ( m_moveDirY == MD_POSITIVE ) ? -( 90 - m_YSmoothAngle ) : ( 90 - m_YSmoothAngle ) );
			m_quatCurrent = m_quatCurrent * quatTempY;

			m_YSmoothAngle = 0;
			m_moveDirY = MD_NONE;
		}
		else
		{
			quatTempY.fromAxisAngle( 0.0, 1.0, 0.0, ( m_moveDirY == MD_POSITIVE ) ? -angleDiff : angleDiff );
			m_quatCurrent = m_quatCurrent * quatTempY;

			m_YSmoothAngle += angleDiff;
		}
#ifdef MY_DEBUG
		GLfloat Matrix[16];
		m_quatCurrent.getTrMatrix( Matrix );
		writeMatrix( Matrix, 16 );
#endif
	}
	else if ( m_moveDirZ != MD_NONE )
	{
		if ( m_ZSmoothAngle >= 90 - angleDiff )
		{
			quatTempZ.fromAxisAngle( 0.0, 0.0, 1.0, ( m_moveDirZ == MD_POSITIVE ) ? -( 90 - m_ZSmoothAngle ) : ( 90 - m_ZSmoothAngle ) );
			m_quatCurrent = m_quatCurrent * quatTempZ;

			m_ZSmoothAngle = 0;
			m_moveDirZ = MD_NONE;
		}
		else
		{
			quatTempZ.fromAxisAngle( 0.0, 0.0, 1.0, ( m_moveDirZ == MD_POSITIVE ) ? -angleDiff : angleDiff );
			m_quatCurrent = m_quatCurrent * quatTempZ;

			m_ZSmoothAngle += angleDiff;
		}
#ifdef MY_DEBUG
		GLfloat Matrix[16];
		m_quatCurrent.getTrMatrix( Matrix );
		writeMatrix( Matrix, 16 );
#endif
	}

	GLfloat MatrixRes[16];
	m_quatCurrent.getTrMatrix( MatrixRes );
	glMultMatrixf( MatrixRes );
}
