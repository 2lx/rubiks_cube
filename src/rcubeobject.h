#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include "gameobject.h"
#include <glm/gtc/type_ptr.hpp>

namespace RC { class CubeModel; }
class ShaderProgram;

class RCubeObject : public GameObject
{
public:
	RCubeObject( ShaderProgram * shaderPr );
	virtual ~RCubeObject();

	void drawObject( const glm::mat4 & pmv );
	void setMove( const RC::MT rt );
	RC::MT setMoveByCoords( const glm::vec3 & pBeg, const glm::vec3 & pEnd );
	bool isMoving() const { return ( m_moveMix >= 0 ); }
	void update();
	void incCurScheme() { m_texCurScheme = ( m_texCurScheme + 1 ) % m_texCount; };
	void reset();

protected:
private:
	RC::CubeModel * m_RCModel;

	glm::quat m_moveQuat;
	glm::quat m_newMoveQuat;
	float m_moveMix = { -1 };

	RC::MT m_moveType = { RC::MT::NONE };
	int m_moveLayer = -1;

	GLuint m_VBOTexCoords, m_VBOTexIndex, m_VBOCubeVertices;
	GLint m_attrTexCoords, m_attrTexIndex, m_attrCubeVertices;

	GLuint m_VBOTexUnionID;
	GLint m_UniTexUnionID;

	GLint m_UniMVP, m_UniTexCount, m_UniTexCurScheme;
	int m_texCount;
	int m_texCurScheme;

	void drawCubie( const int x, const int y, const int z ) const;

	const GLfloat m_aTexCoords[ 2*4*6 ] = {
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
		0.0, 0.0,	1.0, 0.0,	1.0, 1.0,	0.0, 1.0,
	};

	RC::CF m_aTexIndex[ 4*6 ] = {
		RC::CF::FRONT, 	RC::CF::FRONT, 	RC::CF::FRONT, 	RC::CF::FRONT,
		RC::CF::UP,		RC::CF::UP, 	RC::CF::UP, 	RC::CF::UP,
		RC::CF::BACK, 	RC::CF::BACK, 	RC::CF::BACK, 	RC::CF::BACK,
		RC::CF::DOWN, 	RC::CF::DOWN, 	RC::CF::DOWN, 	RC::CF::DOWN,
		RC::CF::LEFT, 	RC::CF::LEFT, 	RC::CF::LEFT, 	RC::CF::LEFT,
		RC::CF::RIGHT, 	RC::CF::RIGHT, 	RC::CF::RIGHT, 	RC::CF::RIGHT
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
