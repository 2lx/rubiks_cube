#include "all.h"

#include "gameobject.h"
#include "rcubeparams.h"

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

void GameObject::setRotate( const RC::RotateType rt )
{
	const glm::quat tqt = RC::RTPar::quat( rt );

	m_newRotateQuat = tqt * m_newRotateQuat;
	m_oldRotateQuat = m_rotateQuat;
	m_rotateMix = 0;
}

RC::RotateType GameObject::setRotateByCoords( const glm::vec3 & pBeg, const glm::vec3 & pEnd )
{
	const float cOffset = RC::CUBIE_COUNT / 2.0f;

	// if the points in different planes or don't lie on the surface of the cube
	if ( std::abs( pBeg.x ) > cOffset + 0.1 || std::abs( pBeg.y ) > cOffset + 0.1 || std::abs( pBeg.z ) > cOffset + 0.1 )
		return RC::RT_NONE;

	// get close rotation axis
	const glm::vec3 rvBeg = pBeg * m_rotateQuat;
	const glm::vec3 rvEnd = pEnd * m_rotateQuat;
	glm::vec3 pRes = glm::cross( rvBeg, rvEnd ) ;

	// get closest rotation axis
	const RC::RotAxis ra = RC::RAPar::closestRA( pRes );
	if ( ra == RC::RA_NONE )
		return RC::RT_NONE;

    const glm::vec3 vAx = RC::RAPar::vec( ra );
    const bool cw = glm::dot( vAx, pRes ) > 0;

	// get rotation type
	const RC::RotateType rt = RC::RTPar::equalRT( ra, cw );
	if ( rt == RC::RT_NONE )
		return RC::RT_NONE;

#ifdef NDEBUG
	std::cout << pBeg.x << " " << pBeg.y << " " << pBeg.z << std::endl;
	std::cout << pEnd.x << " " << pEnd.y << " " << pEnd.z << std::endl;
	std::cout << rvBeg.x << " " << rvBeg.y << " " << rvBeg.z << std::endl;
	std::cout << rvEnd.x << " " << rvEnd.y << " " << rvEnd.z << std::endl;
	std::cout << pRes.x << " " << pRes.y << " " << pRes.z << std::endl;
	std::cout << vAx.x  << " " << vAx.y  << " " << vAx.z  << std::endl;
	std::cout << std::endl;
#endif // NDEBUG

	const glm::quat tqt = RC::RTPar::quat( rt );
	m_newRotateQuat = m_newRotateQuat * tqt;
	m_oldRotateQuat = m_rotateQuat;
	m_rotateMix = 0;

	return rt;
}
/*
void GameObject::updateAxesPos()
{
    for ( int i = AX_FIRST; i < AX_COUNT; i++ )
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
	}
}

bool GameObject::isAxisVisible( const RCAxis ax ) const
{
    if ( m_axesPos[ ax ] == AX_FRONT || m_axesPos[ ax ] == AX_UP || m_axesPos[ ax ] == AX_LEFT )
		return true;
	else return false;
}
*/
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
