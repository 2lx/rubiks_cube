#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <GL/gl.h>
#include "gameobject.h"
#include "shapes.h"
#include "rcubemodel.h"
#include <map>

class RubiksCube : public GameObject
{
public:
	RubiksCube();
	virtual ~RubiksCube();

	void drawObject();
	void setMove( const RCMoveType newRT );
	bool isMoving() const { return m_moveType != MT_NONE; }

protected:
private:
	RCubeModel * m_RCModel;
	std::map< RCMoveType, RCMoveParam * > m_paramsMap;

	MyQuaternion m_moveQuat;
	GLfloat m_moveAngle = { 0 };
	RCMoveType m_moveType = { MT_NONE };

	void setCubeVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize );
	void setCubieVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize, const int x, const int y, const int z );
};

#endif // RUBIKSCUBE_H
