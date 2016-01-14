#ifndef MYQUATERNION_H
#define MYQUATERNION_H

#include <GL/gl.h>
#include <GL/glu.h>

class MyQuaternion
{
	public:
		MyQuaternion();
		virtual ~MyQuaternion();

		MyQuaternion operator *( MyQuaternion q );
		void fromAxisAngle( GLfloat x, GLfloat y, GLfloat z, GLfloat degrees );
		void getMatrix( GLfloat * pMatrix );
	protected:
	private:
		GLfloat m_w;
		GLfloat m_x;
		GLfloat m_y;
		GLfloat m_z;
};

#endif // MYQUATERNION_H
