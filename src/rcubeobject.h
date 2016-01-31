#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

//#include <GL/gl.h>
#include "gameobject.h"
#include "rcubemodel.h"

class ShaderProgram;

using namespace RC;

class RCubeObject : public GameObject
{
public:
	RCubeObject( ShaderProgram * shaderPr );
	virtual ~RCubeObject();

	void drawObject();
	void setMove( const RCMoveType newRT );
	void setMoveByCoords( const Point3D pBeg, const Point3D pEnd );
//	bool isMoving() const { return m_moveType != MT_NONE; }
	void Update();

protected:
private:
	CubeModel * m_RCModel;

//	MyQuaternion m_moveQuat;
//	GLfloat m_moveAngle = { 0 };
//	RCMoveType m_moveType = { MT_NONE };
//	int m_moveLayer = -1;

	GLuint m_VBOTexCoords, m_VBOTexIndex, m_VBOCubeVertices;
	GLint m_attrTexCoords, m_attrTexIndex, m_attrCubeVertices;

	GLuint m_VBOTexID[ AX_COUNT ];
	GLint m_UniTexID[ AX_COUNT ];
	GLint m_UniMVP;		// ModelViewProjection


	GLfloat m_aTexCoords[ 2*4*6 ] = {
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
	};

	GLushort m_aTexIndex[ 4*6 ] = {
		AX_FRONT, AX_FRONT, AX_FRONT, AX_FRONT,
		AX_UP, AX_UP, AX_UP, AX_UP,
		AX_BACK, AX_BACK, AX_BACK, AX_BACK,
		AX_DOWN, AX_DOWN, AX_DOWN, AX_DOWN,
		AX_LEFT, AX_LEFT, AX_LEFT, AX_LEFT,
		AX_RIGHT, AX_RIGHT, AX_RIGHT, AX_RIGHT
	};

//		    5-----4
//		   /|    /|
//		  3-+---2 |
//		  | 7---+-6
//		  |/    |/
//		  0-----1

	GLfloat m_aCubeVertices[ 3*4*6 ] = {
		// front
		-1.0, -1.0,  1.0, // 0
		 1.0, -1.0,  1.0, // 1
		 1.0,  1.0,  1.0, // 2
		-1.0,  1.0,  1.0, // 3
		// top
		-1.0,  1.0,  1.0, // 3
		 1.0,  1.0,  1.0, // 2
		 1.0,  1.0, -1.0, // 4
		-1.0,  1.0, -1.0, // 5
		// back
		 1.0, -1.0, -1.0, // 6
		-1.0, -1.0, -1.0, // 7
		-1.0,  1.0, -1.0, // 5
		 1.0,  1.0, -1.0, // 4
		// bottom
		 1.0, -1.0,  1.0, // 1
		-1.0, -1.0,  1.0, // 0
		-1.0, -1.0, -1.0, // 7
		 1.0, -1.0, -1.0, // 6
		// left
		-1.0, -1.0, -1.0, // 7
		-1.0, -1.0,  1.0, // 0
		-1.0,  1.0,  1.0, // 3
		-1.0,  1.0, -1.0, // 5
		// right
		 1.0, -1.0,  1.0, // 1
		 1.0, -1.0, -1.0, // 6
		 1.0,  1.0, -1.0, // 4
		 1.0,  1.0,  1.0, // 2
	};

	RCAxis getMoveAxis( const Point3D pBeg, const Point3D pEnd ) const;

	void drawCubie( const int x, const int y, const int z ) const;
	void setCubeVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize ) const;
	void setCubieVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize, const int x, const int y, const int z ) const;
};

#endif // RUBIKSCUBE_H
