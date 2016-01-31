#include "all.h"

#include "gameobject.h"
#include "rcubeparams.h"

#ifdef MY_DEBUG
#include "output.h"
#endif

using namespace RC;

GameObject::GameObject()
{
//    for ( int i = AX_FIRST; i < AX_COUNT; i++ )
//		m_axesPos[ i ] = RCAxis( i );
}

GameObject::~GameObject()
{
	//dtor
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

	glGenTextures(1, &bId);
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

void GameObject::setRotates( const int newDirX, const int newDirY, const int newDirZ, const bool isPos )
{
	if ( !isRotating() )
	{
//		m_rotateVec.setXYZ( newDirX, newDirY, newDirZ );
		m_rotateVec = glm::vec3( newDirX, newDirY, newDirZ );
		m_isPositive = isPos;
	}
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
//	MyQuaternion quatTemp;
	glm::quat quatTemp2;

	if ( glm::length( m_rotateVec ) > 0 )
	{
		if ( m_rotateAngle >= 90 - ANGLE_DIFF )
		{
			GLfloat newAngle = ( m_isPositive ) ? m_rotateAngle - 90 : 90 - m_rotateAngle;

			quatTemp2 = glm::angleAxis( newAngle, m_rotateVec );
			m_rotateQuat = quatTemp2 * m_rotateQuat ;

            updateAxesPos();

			m_rotateAngle = 0;
			m_rotateVec = glm::vec3();
		}
		else
		{
			quatTemp2 = glm::angleAxis( ( m_isPositive ) ? -ANGLE_DIFF : ANGLE_DIFF, m_rotateVec );
			m_rotateQuat = quatTemp2 * m_rotateQuat;

			m_rotateAngle += ANGLE_DIFF;
		}

#ifdef MY_DEBUG
//		GLfloat Matrix[16];
//		m_rotateQuat.getTrMatrix( Matrix );
//		writeMatrix( Matrix, 4, 4 );
#endif
	}

//	GLfloat MatrixRes[16];
//	m_rotateQuat.getTrMatrix( MatrixRes );
//	glMultMatrixf( MatrixRes );
}
