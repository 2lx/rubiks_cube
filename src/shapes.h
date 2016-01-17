#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

class Point3D
{
public:
	Point3D() { };
	Point3D( const GLfloat x, const GLfloat y, const GLfloat z )
		: m_x( x ), m_y( y ), m_z( z ) { };

	GLfloat x() const { return m_x; };
	GLfloat y() const { return m_y; };
	GLfloat z() const { return m_z; };

private:
	GLfloat m_x = { 0 };
	GLfloat m_y = { 0 };
	GLfloat m_z = { 0 };
};

class CubePiece
{
public:
	unsigned int colTop = -1;
	unsigned int colBottom = -1;
	unsigned int colLeft = -1;
	unsigned int colRight = -1;
	unsigned int colFront = -1;
	unsigned int colBack = -1;
};

#endif // SHAPES_H_INCLUDED
