#include "all.h"

#include "gameobject.h"
#include "rcubeparams.h"

#ifdef MY_DEBUG
#include "output.h"
#endif

using namespace RC;

GameObject::GameObject()
{
    for ( int i = AX_FIRST; i < AX_COUNT; i++ )
		m_axesPos[ i ] = RCAxis( i );
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

void GameObject::updateAxesPos()
{
    for ( int i = AX_FIRST; i < AX_COUNT; i++ )
    {
		MyQuaternion quatT( AxisParams::vec( RCAxis( i ) ) );

		MyQuaternion quatR = m_rotateQuat.inverse() * quatT * m_rotateQuat;
		quatR = quatR.normalize();

		Vector3D vec = Vector3D( quatR.x(), quatR.y(), quatR.z() );
		m_axesPos[ i ] = AxisParams::getAxisForVector( vec );
#ifdef MY_DEBUG
		writeVector3D( AxisParams::vec( RCAxis( i ) ) );
		writeVector3D( vec );
		std::cout << std::endl;
#endif // MY_DEBUG
	}
}

bool GameObject::isAxisVisible( const RCAxis ax ) const
{
    if ( m_axesPos[ ax ] == AX_FRONT || m_axesPos[ ax ] == AX_UP || m_axesPos[ ax ] == AX_LEFT )
		return true;
	else return false;
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

            updateAxesPos();

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
