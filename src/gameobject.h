#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/gl.h>
#include "shapes.h"
#include "myquaternion.h"

class GameObject
{
public:
	enum MoveDirection
	{
		MD_NONE, MD_POSITIVE, MD_NEGATIVE
	};

	GameObject();
	virtual ~GameObject();

	virtual void drawObject() = 0;
	void rotateObject();
	void setRotates( const MoveDirection newDirX, const MoveDirection newDirY, const MoveDirection newDirZ );
	bool isRotating() const;

protected:

private:
	Point3D m_pos;

	float m_SmoothAngle[ 3 ] = { 0, 0, 0 };
	MoveDirection m_moveDir[ 3 ] = { MD_NONE, MD_NONE, MD_NONE };
	MyQuaternion m_quatCurrent;
};

#endif // GAMEOBJECT_H
