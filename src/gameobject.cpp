#include "all.h"

#include "gameobject.h"
#include "rcubeparams.h"

using namespace RC;

GameObject::GameObject()
{
//    for ( int i = AX_FIRST; i < AX_COUNT; i++ )
//		m_axesPos[ i ] = RCAxis( i );
}

GameObject::~GameObject()
{

}

GLuint GameObject::loadGLTexture2D( const char * filename ) const
{
	GLuint bId;

	SDL_Surface * res_texture = IMG_Load( filename );
	if ( res_texture == NULL )
	{
		std::cout << "IMG_Load: " << SDL_GetError() << std::endl;
		return -1;
	}

	glGenTextures( 1, &bId );
	glBindTexture( GL_TEXTURE_2D, bId );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D,	// target
		0,							// level, 0 = base, no minimap,
		GL_RGBA,					// internalformat
		res_texture->w,  			// width
		res_texture->h,				// height
		0,							// border, always 0 in OpenGL ES
		GL_RGBA,					// format
		GL_UNSIGNED_BYTE,			// type
		res_texture->pixels			// data
	);

	SDL_FreeSurface( res_texture );

	return bId;
}

void GameObject::setRotate( const int newDirX, const int newDirY, const int newDirZ, const bool isPos )
{
	float angle = glm::radians( 90.0f );
	glm::quat tempQuat = glm::angleAxis( ( isPos ) ? angle : -angle, glm::vec3( newDirX, newDirY, newDirZ ) );

	m_newRotateQuat = tempQuat * m_newRotateQuat;
	m_oldRotateQuat = m_rotateQuat;
	m_rotateMix = 0;

//	std::cout << m_newRotateQuat.x << " " << m_newRotateQuat.y << " " << m_newRotateQuat.z << " " << m_newRotateQuat.w << " " << std::endl;
}

void GameObject::updateAxesPos()
{
 /*   for ( int i = AX_FIRST; i < AX_COUNT; i++ )
    {
		MyQuaternion quatT( AxisParams::vec( RCAxis( i ) ) );

		MyQuaternion quatR = m_rotateQuat.inverse() * quatT * m_rotateQuat;
		quatR = quatR.normalize();

		Vector3D vec = Vector3D( quatR.x(), quatR.y(), quatR.z() );
		m_axesPos[ i ] = AxisParams::getAxisForVector( vec );
#ifdef MY_DEBUG
		writeVector3D( AxisParams::vec( RCAxis( i ) ) );
		writeVector3D( vec );
		std::cout << std::endl;
#endif // MY_DEBUG
	}*/
}

bool GameObject::isAxisVisible( const RCAxis ax ) const
{
 /*   if ( m_axesPos[ ax ] == AX_FRONT || m_axesPos[ ax ] == AX_UP || m_axesPos[ ax ] == AX_LEFT )
		return true;
	else return false;*/
}

void GameObject::rotateObject( )
{
	if ( isRotating() )
	{
		if ( m_rotateMix < 1.0 )
		{
			m_rotateQuat = glm::mix( m_oldRotateQuat, m_newRotateQuat, m_rotateMix );
			m_rotateMix += 0.10;
		}
		else
		{
			m_rotateQuat = m_newRotateQuat;
			m_oldRotateQuat = m_newRotateQuat;
			m_rotateMix = -1;
		}
	}
}
