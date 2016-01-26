#ifndef MYQUATERNION_H
#define MYQUATERNION_H

#include <GL/gl.h>
//#include <GL/glu.h>
//#include "shapes.h"
class Vector3D;

class MyQuaternion
{
	public:
		MyQuaternion();
		MyQuaternion( const MyQuaternion & q );
		MyQuaternion( const GLfloat wi, const GLfloat xi, const GLfloat yi, const GLfloat zi );
		MyQuaternion( const Vector3D vec );
		virtual ~MyQuaternion();

		void fromAxisAngle( const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat degrees );

		void reset() { m_w = 1; m_x = 0; m_y = 0; m_z = 0; }
		MyQuaternion operator + ( const MyQuaternion & q ) const;
		MyQuaternion operator * ( const MyQuaternion & q ) const;
		MyQuaternion operator = ( const MyQuaternion & q );
		bool operator == ( const MyQuaternion & lq ) const;

		GLfloat norm() const;
		MyQuaternion scale( GLfloat  s ) const;
		MyQuaternion inverse() const;
		MyQuaternion conjugate() const;
		MyQuaternion normalize() const;

		void getMatrix( GLfloat * pMatrix ) const;
		void getTrMatrix( GLfloat * pMatrix ) const;

		inline GLfloat w() const { return m_w; };
		inline GLfloat x() const { return m_x; };
		inline GLfloat y() const { return m_y; };
		inline GLfloat z() const { return m_z; };

	protected:
	private:
		GLfloat m_w { 1 };
		GLfloat m_x { 0 };
		GLfloat m_y { 0 };
		GLfloat m_z { 0 };
};

#endif // MYQUATERNION_H
