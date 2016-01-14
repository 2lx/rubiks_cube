#include "all.h"

#include "myquaternion.h"
#include <math.h>

MyQuaternion::MyQuaternion()
{
	//ctor
}

MyQuaternion::~MyQuaternion()
{
	//dtor
}

MyQuaternion MyQuaternion::operator *( MyQuaternion q )
{
	MyQuaternion r;

	r.m_w = m_w * q.m_w - m_x * q.m_x - m_y * q.m_y - m_z * q.m_z;
	r.m_x = m_w * q.m_x + m_x * q.m_w + m_y * q.m_z - m_z * q.m_y;
	r.m_y = m_w * q.m_y + m_y * q.m_w + m_z * q.m_x - m_x * q.m_z;
	r.m_z = m_w * q.m_z + m_z * q.m_w + m_x * q.m_y - m_y * q.m_x;

	return( r );
}

void MyQuaternion::fromAxisAngle( GLfloat x, GLfloat y, GLfloat z, GLfloat degrees )
{
	GLfloat angle = GLfloat( ( degrees / 180.0f ) * M_PI );
	GLfloat result = ( GLfloat )sin( angle / 2.0f );

	m_w = ( GLfloat )cos( angle / 2.0f );
	m_x = GLfloat( x * result );
	m_y = GLfloat( y * result );
	m_z = GLfloat( z * result );
}

void MyQuaternion::getMatrix( GLfloat * pMatrix )
{
	if( !pMatrix ) return;

	pMatrix[ 0] = 1.0f - 2.0f * ( m_y * m_y + m_z * m_z );
	pMatrix[ 1] = 2.0f * ( m_x * m_y + m_z * m_w );
	pMatrix[ 2] = 2.0f * ( m_x * m_z - m_y * m_w );
	pMatrix[ 3] = 0.0f;

	pMatrix[ 4] = 2.0f * ( m_x * m_y - m_z * m_w );
	pMatrix[ 5] = 1.0f - 2.0f * ( m_x * m_x + m_z * m_z );
	pMatrix[ 6] = 2.0f * ( m_z * m_y + m_x * m_w );
	pMatrix[ 7] = 0.0f;

	pMatrix[ 8] = 2.0f * ( m_x * m_z + m_y * m_w );
	pMatrix[ 9] = 2.0f * ( m_y * m_z - m_x * m_w );
	pMatrix[10] = 1.0f - 2.0f * ( m_x * m_x + m_y * m_y );
	pMatrix[11] = 0.0f;

	pMatrix[12] = 0;
	pMatrix[13] = 0;
	pMatrix[14] = 0;
	pMatrix[15] = 1.0f;
}
