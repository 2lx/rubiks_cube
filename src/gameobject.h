#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/gl.h>
#include "shapes.h"
#include "myquaternion.h"

class GameObject
{
	public:
		GameObject();
		virtual ~GameObject();

		virtual void drawObject() = 0;
		void rotateObject( const MoveDirection newDirX, const MoveDirection newDirY, const MoveDirection newDirZ );
		bool isMoved() const { if ( m_moveDirX == MD_NONE && m_moveDirY == MD_NONE && m_moveDirZ == MD_NONE ) return false; else return true; };

	protected:

	private:
		Point3D m_pos;

		MyQuaternion m_quatCurrent;
		float m_XSmoothAngle = 0;
		float m_YSmoothAngle = 0;
		float m_ZSmoothAngle = 0;

		MoveDirection m_moveDirX = { MD_NONE };
		MoveDirection m_moveDirY = { MD_NONE };
		MoveDirection m_moveDirZ = { MD_NONE };
};

#endif // GAMEOBJECT_H
