#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/gtc/type_ptr.hpp>

class GameObject
{
public:
	virtual ~GameObject();

	virtual void drawObject() = 0;
	void rotateObject();
	void setRotates( const int newDirX, const int newDirY, const int newDirZ, const bool isPos );
	bool isRotating() const	{ return ( m_rotateMix >= 0 ); };

protected:
	glm::quat m_rotateQuat;

	GameObject();	// Hide public constructor
	bool isAxisVisible( const RC::RCAxis ax ) const;

	template < class T >
	GLuint loadGLArrayBuffer( T * pArr, const int arrSize, const GLenum changeHint = GL_STATIC_DRAW ) const
	{
		GLuint bId;
		glGenBuffers( 1, &bId );
		glBindBuffer( GL_ARRAY_BUFFER, bId );
		glBufferData( GL_ARRAY_BUFFER, arrSize, pArr, changeHint );

		return bId;
	}

	GLuint loadGLTexture2D( const char * filename ) const;

private:
    float m_rotateMix = { -1 };
	glm::quat m_newQuat;
//	RC::RCAxis m_axesPos[ RC::AX_COUNT ];

	void updateAxesPos();
};

#endif // GAMEOBJECT_H
