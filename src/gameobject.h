#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

//#include <GL/gl.h>
#include "shapes.h"
#include "rcubemodel.h"
#include "myquaternion.h"

class GameObject
{
public:
	virtual ~GameObject();

	virtual void drawObject() = 0;
	void rotateObject();
	void setRotates( const int newDirX, const int newDirY, const int newDirZ, const bool isPos );
	bool isRotating() const { return ( m_rotateVec.length() > 0 ); };

protected:
	MyQuaternion m_rotateQuat;

	GameObject();	// Hide public constructor
	bool isAxisVisible( const RC::RCAxis ax ) const;

private:
	Point3D m_pos;
	float m_rotateAngle = { 0 };
	bool m_isPositive = { true };
	Vector3D m_rotateVec;
	RC::RCAxis m_axesPos[ RC::AX_COUNT ];

	void updateAxesPos();
};

#endif // GAMEOBJECT_H
