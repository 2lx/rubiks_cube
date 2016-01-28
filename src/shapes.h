#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

//#include <GL/gl.h>
#include <cmath>
#include "rcdefines.h"
#include "myquaternion.h"

using namespace RC;

class Point3D
{
public:
	Point3D() { };
	Point3D( const GLfloat x, const GLfloat y, const GLfloat z )
		: m_x{ x }, m_y{ y }, m_z{ z }
	{ };
	Point3D operator - ( const Point3D & p ) const { return Point3D( m_x - p.m_x, m_y - p.m_y, m_z - p.m_z ); };
	Point3D operator + ( const Point3D & p ) const { return Point3D( m_x + p.m_x, m_y + p.m_y, m_z + p.m_z ); };
	Point3D operator / ( const GLfloat d ) const { return Point3D( m_x / d, m_y / d, m_z / d ); };
	Point3D rotateByQuat( const MyQuaternion & q ) const
	{
		MyQuaternion quatRes = q * MyQuaternion( 0, m_x, m_y, m_z ) * q.inverse();
		return Point3D( quatRes.x(), quatRes.y(), quatRes.z() );
	}

	RCAxis getTangentAxis( const GLfloat sphRad, const GLfloat eps ) const
	{
		if ( std::abs( m_x - sphRad ) < eps )
			return AX_RIGHT;
		else if ( std::abs( m_x + sphRad ) < eps )
			return AX_LEFT;
		else if ( std::abs( m_y - sphRad ) < eps )
			return AX_UP;
		else if ( std::abs( m_y + sphRad ) < eps )
			return AX_DOWN;
		else if ( std::abs( m_z - sphRad ) < eps )
			return AX_FRONT;
		else if ( std::abs( m_z + sphRad ) < eps )
			return AX_BACK;
		else return AX_NONE;
	}

	void setXYZ( const GLfloat x, const GLfloat y, const GLfloat z ) { m_x = x; m_y = y; m_z = z; };
	bool is0() const { if ( m_x == 0 && m_y == 0 && m_z == 0 ) return true; else return false; };
	void normalize() {
		if ( abs( m_x - 1.0 ) < 0.00001 ) m_x =  1;
		if ( abs( m_x + 1.0 ) < 0.00001 ) m_x = -1;
		if ( abs( m_y - 1.0 ) < 0.00001 ) m_y =  1;
		if ( abs( m_y + 1.0 ) < 0.00001 ) m_y = -1;
		if ( abs( m_z - 1.0 ) < 0.00001 ) m_z =  1;
		if ( abs( m_z + 1.0 ) < 0.00001 ) m_z = -1;
	};
	GLfloat length() const { return sqrt( m_x * m_x + m_y * m_y + m_z * m_z ); }
	GLfloat x() const { return m_x; };
	GLfloat y() const { return m_y; };
	GLfloat z() const { return m_z; };

private:
	GLfloat m_x = { 0 };
	GLfloat m_y = { 0 };
	GLfloat m_z = { 0 };
};

class Vector3D
{
public:
	Vector3D() { };
	Vector3D( const GLfloat x, const GLfloat y, const GLfloat z ) : m_x{ x }, m_y{ y }, m_z{ z } { };
	bool operator == ( const Vector3D & vc ) const
	{
		if ( std::abs( m_x - vc.m_x ) < 0.00001
			&& std::abs( m_y - vc.m_y ) < 0.00001
			&& std::abs( m_z - vc.m_z ) < 0.00001
			)
			return true;
		else return false;
	};

	Vector3D operator * ( const GLfloat m ) const { return Vector3D( m_x * m, m_y * m, m_z * m ); };

	GLfloat x() const { return m_x; };
	GLfloat y() const { return m_y; };
	GLfloat z() const { return m_z; };
	void setXYZ( const GLfloat x, const GLfloat y, const GLfloat z ) { m_x = x; m_y = y; m_z = z; };
	bool isFirstOctant() const { return ( m_x >= 0 && m_y >= 0 && m_z >= 0 ); }; // Top-Front-Right octant
	GLfloat length() const { return std::sqrt( m_x * m_x + m_y * m_y + m_z * m_z ); };

	RCAxis getNearestAxis() const {
        if ( std::abs( m_x ) > std::abs( m_y ) && std::abs( m_x ) > std::abs( m_z ) )
            if ( m_x > 0 )
				return AX_RIGHT;
			else return AX_LEFT;
        else if ( std::abs( m_y ) > std::abs( m_x ) && std::abs( m_y ) > std::abs( m_z ) )
            if ( m_y > 0 )
				return AX_UP;
			else return AX_DOWN;
        else if ( std::abs( m_z ) > std::abs( m_x ) && std::abs( m_z ) > std::abs( m_y ) )
            if ( m_z > 0 )
				return AX_FRONT;
			else return AX_BACK;
		else return AX_NONE;
	}

private:
	GLfloat m_x = { 0 };
	GLfloat m_y = { 0 };
	GLfloat m_z = { 0 };
};

#endif // SHAPES_H_INCLUDED
