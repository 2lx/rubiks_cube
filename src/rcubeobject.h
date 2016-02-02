#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include "gameobject.h"
#include <glm/gtc/type_ptr.hpp>

namespace RC { class CubeModel; }
class ShaderProgram;

using namespace RC;

class RCubeObject : public GameObject
{
public:
	RCubeObject( ShaderProgram * shaderPr );
	virtual ~RCubeObject();

	void drawObject( const glm::mat4 & pmv );
	void setMove( const RCMoveType newRT );
	void setMoveByCoords( const glm::vec3 & pBeg, const glm::vec3 & pEnd );
	bool isMoving() const { return ( m_moveMix >= 0 ); }
	void Update();

protected:
private:
	CubeModel * m_RCModel;

//	MyQuaternion m_moveQuat;
	glm::quat m_moveQuat;
	glm::quat m_newMoveQuat;
	float m_moveMix = { -1 };

//	GLfloat m_moveAngle = { 0 };
	RCMoveType m_moveType = { MT_NONE };
	int m_moveLayer = -1;

	GLuint m_VBOTexCoords, m_VBOTexIndex, m_VBOCubeVertices;
	GLint m_attrTexCoords, m_attrTexIndex, m_attrCubeVertices;

	GLuint m_VBOTexUnionID;
	GLint m_UniTexUnionID;

	GLint m_UniMVP;			// ModelViewProjection

	void drawCubie( const int x, const int y, const int z ) const;

	const GLfloat m_aTexCoords[ 2*4*6 ] = {
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
	};

	GLshort m_aTexIndex[ 4*6 ] = {
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

	const GLfloat m_aCubeVertices[ 3*4*6 ] = {
		// front
		-0.5, -0.5,  0.5, // 0
		 0.5, -0.5,  0.5, // 1
		 0.5,  0.5,  0.5, // 2
		-0.5,  0.5,  0.5, // 3
		// top
		-0.5,  0.5,  0.5, // 3
		 0.5,  0.5,  0.5, // 2
		 0.5,  0.5, -0.5, // 4
		-0.5,  0.5, -0.5, // 5
		// back
		 0.5, -0.5, -0.5, // 6
		-0.5, -0.5, -0.5, // 7
		-0.5,  0.5, -0.5, // 5
		 0.5,  0.5, -0.5, // 4
		// bottom
		 0.5, -0.5,  0.5, // 1
		-0.5, -0.5,  0.5, // 0
		-0.5, -0.5, -0.5, // 7
		 0.5, -0.5, -0.5, // 6
		// left
		-0.5, -0.5, -0.5, // 7
		-0.5, -0.5,  0.5, // 0
		-0.5,  0.5,  0.5, // 3
		-0.5,  0.5, -0.5, // 5
		// right
		 0.5, -0.5,  0.5, // 1
		 0.5, -0.5, -0.5, // 6
		 0.5,  0.5, -0.5, // 4
		 0.5,  0.5,  0.5, // 2
	};
};

#endif // RUBIKSCUBE_H
