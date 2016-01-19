#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include "myquaternion.h"

enum RCMoveType
{
	MT_FRONT, MT_FRONTINV,  // INV = inverse, counter-clockwise move
	MT_BACK, MT_BACKINV,
	MT_LEFT, MT_LEFTINV,
	MT_RIGHT, MT_RIGHTINV,
	MT_UP, MT_UPINV,
	MT_DOWN, MT_DOWNINV,

	MT_COUNT,
	MT_NONE
};

class RCMoveParam
{
public:
	RCMoveParam( const short int vX, const short int vY, const short int vZ, const bool nisClockwise )
		: m_x { vX }, m_y { vY }, m_z { vZ }, m_isClockwise { nisClockwise }
	{ };
	short int axisN() const {
		if ( m_x != 0 ) return 0;
		else if ( m_y != 0 ) return 1;
		else return 2;
		};
	short int x() const { return m_x; };
	short int y() const { return m_y; };
	short int z() const { return m_z; };
	bool isClockwise() const { return m_isClockwise; };
	bool isClockwiseAbs() const { if ( m_x + m_y + m_z > 0 ) return m_isClockwise; else return !m_isClockwise; };
	bool isFront() const { return ( m_x + m_y + m_z > 0 ); };

private:
	const short int m_x = 0;
	const short int m_y = 0;
	const short int m_z = 0;
    const bool m_isClockwise = true;
};

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

class CubePiece
{
public:
	CubePiece() { };
	void rotatePiece( const RCMoveType rt )
	{
		unsigned int colTemp;

		if ( rt == MT_FRONT || rt == MT_BACKINV )
		{
			colTemp = colLeft;
			colLeft = colDown;
			colDown = colRight;
			colRight = colUp;
			colUp = colTemp;
		}
		else if ( rt == MT_FRONTINV || rt == MT_BACK )
		{
			colTemp = colLeft;
			colLeft = colUp;
			colUp = colRight;
			colRight = colDown;
			colDown = colTemp;
		}
		else if ( rt == MT_LEFT || rt == MT_RIGHTINV )
		{
			colTemp = colBack;
			colBack = colDown;
			colDown = colFront;
			colFront = colUp;
			colUp = colTemp;
		}
		else if ( rt == MT_RIGHT || rt == MT_LEFTINV )
		{
			colTemp = colBack;
			colBack = colUp;
			colUp = colFront;
			colFront = colDown;
			colDown = colTemp;
		}
		else if ( rt == MT_UP || rt == MT_DOWNINV )
		{
			colTemp = colLeft;
			colLeft = colFront;
			colFront = colRight;
			colRight = colBack;
			colBack = colTemp;
		}
		else if ( rt == MT_UPINV || rt == MT_DOWN )
		{
			colTemp = colLeft;
			colLeft = colBack;
			colBack = colRight;
			colRight = colFront;
			colFront = colTemp;
		}
	}

	unsigned int colUp = -1;
	unsigned int colDown = -1;
	unsigned int colLeft = -1;
	unsigned int colRight = -1;
	unsigned int colFront = -1;
	unsigned int colBack = -1;
};

#endif // SHAPES_H_INCLUDED
