#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/gl.h>
#include "shapes.h"
#include "myquaternion.h"
//#include "myquaternion.h"

class GameObject
{
public:
	virtual ~GameObject();

	virtual void drawObject() = 0;
	void rotateObject();
	void setRotates( const int newDirX, const int newDirY, const int newDirZ, const bool isPos );
	bool isRotating() const { return ( m_rotateVec.length() > 0 ); };

protected:
	GameObject();	// Hide public constructor

	MyQuaternion m_rotateQuat;

private:
	Point3D m_pos;
	float m_rotateAngle = { 0 };
	bool m_isPositive = { true };
	Vector3D m_rotateVec;
};

#endif // GAMEOBJECT_H
