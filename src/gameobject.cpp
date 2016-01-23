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

void GameObject::setRotates( const int newDirX, const int newDirY, const int newDirZ, const bool isPos )
{
	if ( !isRotating() )
	{
		m_rotateVec.setXYZ( newDirX, newDirY, newDirZ );
		m_isPositive = isPos;
	}
}

void GameObject::rotateObject( )
{
	MyQuaternion quatTemp;

	if ( m_rotateVec.length() > 0 )
	{
		if ( m_rotateAngle >= 90 - ANGLE_DIFF )
		{
			GLfloat newAngle = ( m_isPositive ) ? m_rotateAngle - 90 : 90 - m_rotateAngle;

			quatTemp.fromAxisAngle( m_rotateVec.x(), m_rotateVec.y(), m_rotateVec.z(), newAngle );
			m_rotateQuat = m_rotateQuat * quatTemp;
			m_rotateQuat = m_rotateQuat.normalize();

			m_rotateAngle = 0;
			m_rotateVec.setXYZ( 0, 0, 0 );
		}
		else
		{
			quatTemp.fromAxisAngle( m_rotateVec.x(), m_rotateVec.y(), m_rotateVec.z(), ( m_isPositive ) ? -ANGLE_DIFF : ANGLE_DIFF );
			m_rotateQuat = m_rotateQuat * quatTemp;

			m_rotateAngle += ANGLE_DIFF;
		}

#ifdef MY_DEBUG
//		GLfloat Matrix[16];
//		m_rotateQuat.getTrMatrix( Matrix );
//		writeMatrix( Matrix, 4, 4 );
#endif
	}

	GLfloat MatrixRes[16];
	m_rotateQuat.getTrMatrix( MatrixRes );
	glMultMatrixf( MatrixRes );
}
