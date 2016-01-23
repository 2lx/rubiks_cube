#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <GL/gl.h>
#include "gameobject.h"
#include "rcubemodel.h"

using namespace RC;

class RCubeObject : public GameObject
{
public:
	RCubeObject();
	virtual ~RCubeObject();

	void drawObject();
	void setMove( const RCMoveType newRT );
	bool isMoving() const { return m_moveType != MT_NONE; }

protected:
private:
	CubeModel * m_RCModel;

	MyQuaternion m_moveQuat;
	GLfloat m_moveAngle = { 0 };
	RCMoveType m_moveType = { MT_NONE };

	void setCubeVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize ) const;
	void setCubieVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize, const int x, const int y, const int z ) const;
};

#endif // RUBIKSCUBE_H
