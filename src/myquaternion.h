#ifndef MYQUATERNION_H
#define MYQUATERNION_H

#include <GL/gl.h>
#include <GL/glu.h>

class MyQuaternion
{
	public:
		MyQuaternion();
		MyQuaternion( const GLfloat wi, const GLfloat xi, const GLfloat yi, const GLfloat zi );
		virtual ~MyQuaternion();

		void reset() { m_w = 1; m_x = 0; m_y = 0; m_z = 0; }
		MyQuaternion operator + ( const MyQuaternion & q ) const;
		MyQuaternion operator * ( const MyQuaternion & q ) const;
		MyQuaternion operator = ( const MyQuaternion & q );

		GLfloat norm() const;
		MyQuaternion scale( GLfloat  s ) const;
		MyQuaternion inverse() const;
		MyQuaternion conjugate() const;
		void fromAxisAngle( const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat degrees );
		void getMatrix( GLfloat * pMatrix );
		void getTrMatrix( GLfloat * pMatrix );

	protected:
	private:
		GLfloat m_w { 1 };
		GLfloat m_x { 0 };
		GLfloat m_y { 0 };
		GLfloat m_z { 0 };
};

#endif // MYQUATERNION_H
