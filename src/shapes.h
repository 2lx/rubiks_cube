#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include <GL/gl.h>
#include <cmath>

class Point3D
{
public:
	Point3D() { };
	Point3D( const GLfloat x, const GLfloat y, const GLfloat z )
		: m_x{ x }, m_y{ y }, m_z{ z }
	{ };

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

	GLfloat x() const { return m_x; };
	GLfloat y() const { return m_y; };
	GLfloat z() const { return m_z; };
	void setXYZ( const GLfloat x, const GLfloat y, const GLfloat z ) { m_x = x; m_y = y; m_z = z; };
	bool isFirstOctant() const { return ( m_x >= 0 && m_y >= 0 && m_z >= 0 ); }; // Top-Front-Right octant
	GLfloat length() const { return std::sqrt( m_x * m_x + m_y * m_y + m_z * m_z ); };

private:
	GLfloat m_x = { 0 };
	GLfloat m_y = { 0 };
	GLfloat m_z = { 0 };
};

#endif // SHAPES_H_INCLUDED
