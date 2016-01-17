#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/gl.h>
#include "shapes.h"
#include "myquaternion.h"

class GameObject
{
public:
	enum RotateDirection
	{
		RD_NONE, RD_POSITIVE, RD_NEGATIVE
	};

	GameObject();
	virtual ~GameObject();

	virtual void drawObject() = 0;
	void rotateObject();
	void setRotates( const RotateDirection newDirX, const RotateDirection newDirY, const RotateDirection newDirZ );
	bool isRotating() const;

protected:
	MyQuaternion m_rotateQuat;

private:
	Point3D m_pos;

	float m_SmoothAngle[ 3 ] = { 0, 0, 0 };
	RotateDirection m_rotateDir[ 3 ] = { RD_NONE, RD_NONE, RD_NONE };
};

#endif // GAMEOBJECT_H
