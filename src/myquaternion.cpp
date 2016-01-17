#include "all.h"

#include "myquaternion.h"
#include <math.h>

MyQuaternion::MyQuaternion()
{
	//ctor
}

MyQuaternion::MyQuaternion( const GLfloat wi, const GLfloat xi, const GLfloat yi, const GLfloat zi )
	: m_w{ wi }, m_x{ xi }, m_y{ yi }, m_z{ zi }
{
	//ctor
}

MyQuaternion::~MyQuaternion()
{
	//dtor
}

MyQuaternion MyQuaternion::operator + ( const MyQuaternion & q ) const
{
	return MyQuaternion( m_w + q.m_w, m_x + q.m_x, m_y + q.m_y, m_z + q.m_z );
}

MyQuaternion MyQuaternion::operator * ( const MyQuaternion & q ) const
{
	MyQuaternion r;

	r.m_w = m_w * q.m_w - m_x * q.m_x - m_y * q.m_y - m_z * q.m_z;
	r.m_x = m_w * q.m_x + m_x * q.m_w + m_y * q.m_z - m_z * q.m_y;
	r.m_y = m_w * q.m_y + m_y * q.m_w + m_z * q.m_x - m_x * q.m_z;
	r.m_z = m_w * q.m_z + m_z * q.m_w + m_x * q.m_y - m_y * q.m_x;

	return( r );
}

MyQuaternion MyQuaternion::operator = ( const MyQuaternion & q )
{
	m_w = q.m_w;
	m_x = q.m_x;
	m_y = q.m_y;
	m_z = q.m_z;

	return ( * this );
}

GLfloat MyQuaternion::norm() const
{
	return ( m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z );
}

MyQuaternion MyQuaternion::scale( GLfloat  s ) const
{
	return MyQuaternion( m_w * s, m_x * s, m_y * s, m_z * s );
}

MyQuaternion MyQuaternion::inverse() const
{
	return conjugate().scale( 1 / norm() );
}

MyQuaternion MyQuaternion::conjugate() const
{
	return MyQuaternion( m_w, -m_x, -m_y, -m_z );
}

void MyQuaternion::fromAxisAngle( const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat degrees )
{
	GLfloat angle = GLfloat( ( degrees / 180.0f ) * M_PI );
	GLfloat result = ( GLfloat )sin( angle / 2.0f );

	m_w = ( GLfloat ) cos( angle / 2.0f );
	m_x = GLfloat( x * result );
	m_y = GLfloat( y * result );
	m_z = GLfloat( z * result );
}

void MyQuaternion::getMatrix( GLfloat * pMatrix ) const
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

void MyQuaternion::getTrMatrix( GLfloat * pMatrix ) const
{
	if( !pMatrix ) return;

	pMatrix[ 0] = 1.0f - 2.0f * ( m_y * m_y + m_z * m_z );
	pMatrix[ 4] = 2.0f * ( m_x * m_y + m_z * m_w );
	pMatrix[ 8] = 2.0f * ( m_x * m_z - m_y * m_w );
	pMatrix[12] = 0.0f;

	pMatrix[ 1] = 2.0f * ( m_x * m_y - m_z * m_w );
	pMatrix[ 5] = 1.0f - 2.0f * ( m_x * m_x + m_z * m_z );
	pMatrix[ 9] = 2.0f * ( m_z * m_y + m_x * m_w );
	pMatrix[13] = 0.0f;

	pMatrix[ 2] = 2.0f * ( m_x * m_z + m_y * m_w );
	pMatrix[ 6] = 2.0f * ( m_y * m_z - m_x * m_w );
	pMatrix[10] = 1.0f - 2.0f * ( m_x * m_x + m_y * m_y );
	pMatrix[14] = 0.0f;

	pMatrix[ 3] = 0;
	pMatrix[ 7] = 0;
	pMatrix[11] = 0;
	pMatrix[15] = 1.0f;
}
