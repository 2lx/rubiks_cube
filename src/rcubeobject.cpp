#include "all.h"

#include "rcdefines.h"
#include "rcubeobject.h"
#include "rcubeparams.h"
#include "shaderprogram.h"
#include "rcubemodel.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

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
		if ( isMTInvAxis( m_moveType ) )
			m_moveLayer = 0;
		else m_moveLayer = CUBIE_COUNT - 1;

		m_moveMix = 0;
		float angle = glm::radians( 90.0f );
		m_newMoveQuat = glm::angleAxis( ( isPos ) ? -angle : angle, MoveParams::vec( m_moveType ) );
	}

//	m_moveType = MT_FRONT;

/*	MyQuaternion quatT( MoveParams::vec( newRT ) );

	MyQuaternion quatR = m_rotateQuat * quatT * m_rotateQuat.inverse();
	quatR = quatR.normalize();

	Vector3D vec = Vector3D( quatR.x(), quatR.y(), quatR.z() );
	m_moveType = MoveParams::getMTypeForPars( vec, MoveParams::clockwise( newRT ) );

	if ( isMTInvAxis( m_moveType ) )
		m_moveLayer = 0;
	else m_moveLayer = CUBIE_COUNT - 1;*/
}
/*
RCAxis RCubeObject::getMoveAxis( const Point3D pBeg, const Point3D pEnd ) const
{
	Point3D pcDiff = pEnd - pBeg;
	Vector3D vec = Vector3D( pcDiff.x(), pcDiff.y(), pcDiff.z() );

	return vec.getNearestAxis();
}

void RCubeObject::setMoveByCoords( const Point3D pBeg, const Point3D pEnd )
{
    // get tangent axis
	const float coorSurface = ( CUBIE_COUNT / 2.0 ) - 0.05;
	RCAxis axBeg = pBeg.getTangentAxis( coorSurface, 0.14 );
	RCAxis axTangent = pEnd.getTangentAxis( coorSurface, 0.14 );

	// if the points in different planes or don't lie on the surface of the cube
	if ( axBeg != axTangent || axBeg == AX_NONE || axTangent == AX_NONE )
		return;

	// get mouse move axis
    RCAxis axMove = getMoveAxis( pBeg, pEnd );
	if ( axMove == AX_NONE || axMove == axTangent )
		return;

	// get rotation axis
    RCAxis axResult = getThirdAxis( axTangent, axMove );
    if ( axResult == AX_NONE )
		return;

	// rotate the cube to the rotation axis looked at front
	MyQuaternion rQt = AxisParams::quat( axResult );
	Point3D pnBeg = pBeg.rotateByQuat( rQt );
	Point3D pnEnd = pEnd.rotateByQuat( rQt );

	// check clockwise
	bool isCW = ( pnEnd.x() * pnBeg.y() - pnBeg.x() * pnEnd.y() ) >= 0;

	m_moveType = MoveParams::getMTypeForPars( axResult, isCW );

	// find move layer
	Point3D pp = pBeg + Point3D( CUBIE_COUNT / 2.0, CUBIE_COUNT / 2.0, CUBIE_COUNT / 2.0 );
	if ( axResult == AX_RIGHT || axResult == AX_LEFT )
		m_moveLayer = floor( pp.x() );
	else if ( axResult == AX_UP || axResult == AX_DOWN )
		m_moveLayer = floor( pp.y() );
	else if ( axResult == AX_FRONT || axResult == AX_BACK )
		m_moveLayer = floor( pp.z() );
}
*/
void RCubeObject::drawObject( const glm::mat4 & pmv )
{
	const float offCenter = CUBIE_COUNT / 2.0f - 0.5;

	if ( isMoving() )
	{
		if ( m_moveMix < 1.0 )
		{
			m_moveQuat = glm::mix( m_moveQuat, m_newMoveQuat, m_moveMix );
			m_moveMix += 0.04;
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

	glActiveTexture( GL_TEXTURE0 );
	glUniform1i( m_UniTexUnionID, 0 );
	glBindTexture( GL_TEXTURE_2D, m_VBOTexUnionID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

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
