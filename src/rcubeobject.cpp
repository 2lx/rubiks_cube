#include "all.h"

#include "rcubeobject.h"
#include "rcubeparams.h"
#include "shaderprogram.h"
#include "rcubemodel.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace RC;

RCubeObject::RCubeObject( ShaderProgram * shaderPr )
{
	m_RCModel = new CubeModel();

	m_VBOCubeVertices = loadGLArrayBuffer( m_aCubeVertices, sizeof( m_aCubeVertices ) );
	m_VBOTexCoords = loadGLArrayBuffer( m_aTexCoords, sizeof( m_aTexCoords ) );
	m_VBOTexIndex = loadGLArrayBuffer( m_aTexIndex, sizeof( m_aTexIndex ), GL_STREAM_DRAW );

	m_attrCubeVertices = shaderPr->addAttribute( "cVertex" );
	m_attrTexCoords = shaderPr->addAttribute( "texCoord" );
	m_attrTexIndex = shaderPr->addAttribute( "texIndex" );

	m_VBOTexUnionID = loadGLTexture2D( "glsl/texture.png" );
	m_UniTexUnionID = shaderPr->addUniform( "texUnion" );

	m_UniMVP = shaderPr->addUniform( "mvp" );
}

RCubeObject::~RCubeObject()
{
	glDeleteBuffers( 1, &m_VBOCubeVertices );
	glDeleteBuffers( 1, &m_VBOTexCoords );
	glDeleteBuffers( 1, &m_VBOTexIndex );
	glDeleteTextures( 1, &m_VBOTexUnionID );

	delete m_RCModel;
}

void RCubeObject::Update( )
{
}

void RCubeObject::setMove( const RCMoveType newRT )
{
	glm::vec3 vec = MoveParams::vec( newRT );
	bool isPos = MoveParams::clockwise( newRT );

	glm::vec3 vecRot = vec * m_rotateQuat;
	m_moveType = MoveParams::getMTypeForPars( vecRot, isPos );

	if ( m_moveType != MT_NONE )
	{
//		if ( isMTInvAxis( m_moveType ) )
		if ( glm::dot( vecRot, glm::vec3( 1.0f, 1.0f, 1.0f ) ) < 0 )
			m_moveLayer = 0;
		else m_moveLayer = CUBIE_COUNT - 1;

		m_moveMix = 0;
		float angle = glm::radians( 90.0f );
		m_newMoveQuat = glm::angleAxis( ( isPos ) ? -angle : angle, MoveParams::vec( m_moveType ) );
	}
}

void RCubeObject::setMoveByCoords( const glm::vec3 & pBeg, const glm::vec3 & pEnd )
{
	const float cOffset = CUBIE_COUNT / 2.0f;

	// if the points in different planes or don't lie on the surface of the cube
	if ( std::abs( pBeg.x ) > cOffset + 0.1 || std::abs( pBeg.y ) > cOffset + 0.1 || std::abs( pBeg.z ) > cOffset + 0.1 )
		return;

	// get close rotation axis
	const glm::vec3 rvBeg = pBeg * m_rotateQuat;
	const glm::vec3 rvEnd = pEnd * m_rotateQuat;
	glm::vec3 pRes = glm::cross( rvBeg, rvEnd ) ;

	// get closest rotation axis
    glm::vec3 vAx;
    const float aX = std::abs( pRes.x );
	const float aY = std::abs( pRes.y );
	const float aZ = std::abs( pRes.z );

    if ( aX > aY && aX > aZ )
	{
        if ( pRes.x > 0 )
			vAx = { 1.0f, 0.0f, 0.0f };
		else vAx = { -1.0f, 0.0f, 0.0f };
	}
	else if ( aY > aX && aY > aZ )
	{
        if ( pRes.y > 0 )
			vAx = { 0.0f, 1.0f, 0.0f };
		else vAx = { 0.0f, -1.0f, 0.0f };
	}
	else if ( aZ > aX && aZ > aY )
	{
        if ( pRes.z > 0 )
			vAx = { 0.0f, 0.0f, 1.0f };
		else vAx = { 0.0f, 0.0f, -1.0f };
	}
	else return;

#ifdef NDEBUG
	std::cout << pBeg.x << " " << pBeg.y << " " << pBeg.z << std::endl;
	std::cout << pEnd.x << " " << pEnd.y << " " << pEnd.z << std::endl;
	std::cout << rvBeg.x << " " << rvBeg.y << " " << rvBeg.z << std::endl;
	std::cout << rvEnd.x << " " << rvEnd.y << " " << rvEnd.z << std::endl;
	std::cout << pRes.x << " " << pRes.y << " " << pRes.z << std::endl;
	std::cout << vAx.x  << " " << vAx.y  << " " << vAx.z  << std::endl;
	std::cout.flush();
#endif // NDEBUG

	// check clockwise
	bool isCW;
	if ( glm::dot( glm::cross( vAx, pBeg ), pBeg ) > 0 )
		isCW = true;
	else isCW = false;

	// choose parameters
	m_moveType = MoveParams::getMTypeForPars( vAx, isCW );
	m_moveMix = 0;
	float angle = glm::radians( 90.0f );
	m_newMoveQuat = glm::angleAxis( ( isCW ) ? -angle : angle, vAx );

	// find move layer
	if ( vAx.x != 0 )
		m_moveLayer = floor( rvBeg.x + cOffset );
	else if ( vAx.y != 0 )
		m_moveLayer = floor( rvBeg.y + cOffset );
	else if ( vAx.z != 0 )
		m_moveLayer = floor( rvBeg.z + cOffset );
	else m_moveLayer = 0;
}

void RCubeObject::drawObject( const glm::mat4 & pmv )
{
	const float offCenter = CUBIE_COUNT / 2.0f - 0.5;

	// turn the face of cube
	if ( isMoving() )
	{
		if ( m_moveMix < 1.0 )
		{
			m_moveQuat = glm::mix( m_moveQuat, m_newMoveQuat, m_moveMix );
			m_moveMix += 0.08;
		}
		else
		{
			m_RCModel->moveCubies( m_moveType, m_moveLayer );
			m_moveType = MT_NONE;
			m_moveLayer = -1;

			m_moveQuat = glm::quat();
			m_moveMix = -1;
		}
	}

	// use texture
	glActiveTexture( GL_TEXTURE0 );
	glUniform1i( m_UniTexUnionID, 0 );
	glBindTexture( GL_TEXTURE_2D, m_VBOTexUnionID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	// calculate transformation matrix
	glm::mat4 rotation = glm::mat4_cast( m_rotateQuat );
	glm::mat4 moving = glm::mat4_cast( m_moveQuat );

	glm::mat4 mvpR = pmv * rotation;
	glm::mat4 mvpRM = pmv * rotation * moving;

	for ( int x = 0; x < CUBIE_COUNT; ++x )
		for ( int y = 0; y < CUBIE_COUNT; ++y )
			for ( int z = 0; z < CUBIE_COUNT; ++z )
				// only draw cubies in the outer layer
				if ( x == 0 || x == CUBIE_COUNT - 1 || y == 0 || y == CUBIE_COUNT - 1 || z == 0 || z == CUBIE_COUNT - 1 )
				{
					// update texture index matrix
					for ( int i = 0; i < 4; i++ )
					{
						m_aTexIndex[ 4*AX_FRONT + i ] 	= m_RCModel->cubie( x, y, z ).colInd( AX_FRONT );
						m_aTexIndex[ 4*AX_UP + i ] 		= m_RCModel->cubie( x, y, z ).colInd( AX_UP );
						m_aTexIndex[ 4*AX_BACK + i ] 	= m_RCModel->cubie( x, y, z ).colInd( AX_BACK );
						m_aTexIndex[ 4*AX_DOWN + i ] 	= m_RCModel->cubie( x, y, z ).colInd( AX_DOWN );
						m_aTexIndex[ 4*AX_LEFT + i ] 	= m_RCModel->cubie( x, y, z ).colInd( AX_LEFT );
						m_aTexIndex[ 4*AX_RIGHT + i ] 	= m_RCModel->cubie( x, y, z ).colInd( AX_RIGHT );
					}

					m_VBOTexIndex = loadGLArrayBuffer( m_aTexIndex, sizeof( m_aTexIndex ) );

					// calculate offset of cubie
					glm::mat4 offset = glm::translate( glm::mat4( 1.0f ),
							glm::vec3( x - offCenter, y - offCenter, z - offCenter ) );

					// calculate model view projection matrix
					glm::mat4 mRes;

					if ( ( m_moveType != MT_NONE && m_moveLayer != -1 ) &&
						( 	( z == m_moveLayer && MoveParams::vec( m_moveType ).z != 0 ) ||
							( x == m_moveLayer && MoveParams::vec( m_moveType ).x != 0 ) ||
							( y == m_moveLayer && MoveParams::vec( m_moveType ).y != 0 ) )
						)
					{
						mRes = mvpRM * offset;
					}
					else mRes = mvpR * offset;

					glUniformMatrix4fv( m_UniMVP, 1, GL_FALSE, glm::value_ptr( mRes ) );

					drawCubie( x, y, z );
				}
}

void RCubeObject::drawCubie( const int x, const int y, const int z ) const
{
	glEnableVertexAttribArray( m_attrTexIndex );
	glBindBuffer( GL_ARRAY_BUFFER, m_VBOTexIndex );
	glVertexAttribPointer( m_attrTexIndex, 1, GL_SHORT, GL_FALSE, 0, 0 );

	glEnableVertexAttribArray( m_attrCubeVertices );
	glBindBuffer( GL_ARRAY_BUFFER, m_VBOCubeVertices );
	glVertexAttribPointer( m_attrCubeVertices, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	glEnableVertexAttribArray( m_attrTexCoords );
	glBindBuffer( GL_ARRAY_BUFFER, m_VBOTexCoords );
	glVertexAttribPointer( m_attrTexCoords, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	glDrawArrays( GL_QUADS, 0, 4*6 );

	glDisableVertexAttribArray( m_attrTexIndex );
	glDisableVertexAttribArray( m_attrCubeVertices );
	glDisableVertexAttribArray( m_attrTexCoords );
}
