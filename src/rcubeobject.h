#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

//#include <GL/gl.h>
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
	void setMoveByCoords( const Point3D pBeg, const Point3D pEnd );
	bool isMoving() const { return m_moveType != MT_NONE; }

protected:
private:
	CubeModel * m_RCModel;

	MyQuaternion m_moveQuat;
	GLfloat m_moveAngle = { 0 };
	RCMoveType m_moveType = { MT_NONE };
	int m_moveLayer = -1;

	RCAxis getMoveAxis( const Point3D pBeg, const Point3D pEnd ) const;

	void drawCubie( const int x, const int y, const int z ) const;
	void setCubeVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize ) const;
	void setCubieVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize, const int x, const int y, const int z ) const;
};

#endif // RUBIKSCUBE_H
