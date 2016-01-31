#include "all.h"

#include "rcdefines.h"
#include "rcubeobject.h"
#include "rcubeparams.h"
#include "shaderprogram.h"
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
/*	MyQuaternion quatT( MoveParams::vec( newRT ) );

	MyQuaternion quatR = m_rotateQuat * quatT * m_rotateQuat.inverse();
	quatR = quatR.normalize();

	Vector3D vec = Vector3D( quatR.x(), quatR.y(), quatR.z() );
	m_moveType = MoveParams::getMTypeForPars( vec, MoveParams::clockwise( newRT ) );

	if ( isMTInvAxis( m_moveType ) )
		m_moveLayer = 0;
	else m_moveLayer = CUBIE_COUNT - 1;*/
}

RCAxis RCubeObject::getMoveAxis( const Point3D pBeg, const Point3D pEnd ) const
{
/*	Point3D pcDiff = pEnd - pBeg;
	Vector3D vec = Vector3D( pcDiff.x(), pcDiff.y(), pcDiff.z() );

	return vec.getNearestAxis();*/
}

void RCubeObject::setMoveByCoords( const Point3D pBeg, const Point3D pEnd )
{
 /*   // get tangent axis
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
*/
}

void RCubeObject::drawObject()
{
	glActiveTexture( GL_TEXTURE0 );
	glUniform1i( m_UniTexUnionID, 0 );
	glBindTexture( GL_TEXTURE_2D, m_VBOTexUnionID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glm::mat4 view =
		glm::rotate( glm::mat4( 1.0f ), 35.264f, glm::vec3( 1, 0, 0 ) ) * // X axis
		glm::rotate( glm::mat4( 1.0f ), 45.0f, glm::vec3( 0, 1, 0 ) ) * // Y axis
		glm::rotate( glm::mat4( 1.0f ), 0.0f, glm::vec3( 0, 0, 1 ) );  // Z axis

	glm::mat4 rotation = glm::mat4_cast( m_rotateQuat );

	glm::mat4 model = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0, 0.5, -20.0 ) );
	glm::mat4 projection = glm::ortho( -SCREEN_HORIZMARGIN, SCREEN_HORIZMARGIN, -SCREEN_VERTMARGIN, SCREEN_VERTMARGIN, 0.0f, 40.0f );

	const float offCenter = CUBIE_COUNT / 2.0f - 0.5;

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

					glm::mat4 offset = glm::translate( glm::mat4( 1.0f ),
							glm::vec3( x - offCenter, y - offCenter, z - offCenter ) );
					glm::mat4 mvp = projection * model * view * rotation * offset  ;// + offset;

					glUniformMatrix4fv( m_UniMVP, 1, GL_FALSE, glm::value_ptr( mvp ) );

					drawCubie( x, y, z );
				}


//	drawCubie( 0, 0, 0 );

/*	setCubeVertices( 0, 0, 0, CUBIE_COUNT * 2.0 / 3.0 );

	MyQuaternion quatTemp;

	if ( m_moveType != MT_NONE )
	{
		if ( m_moveAngle >= 90 - ANGLE_DIFF )
		{
			m_RCModel->moveCubies( m_moveType, m_moveLayer );

			m_moveAngle = 0;
			m_moveType = MT_NONE;
			m_moveLayer = -1;
			m_moveQuat.reset();
		}
		else
		{
			quatTemp.fromAxisAngle( abs( MoveParams::vec( m_moveType ).x() ),
				abs( MoveParams::vec( m_moveType ).y() ), abs( MoveParams::vec( m_moveType ).z() ),
				( MoveParams::vec( m_moveType ).isFirstOctant() == MoveParams::clockwise( m_moveType ) ) ? ANGLE_DIFF : -ANGLE_DIFF );

			m_moveQuat = m_moveQuat * quatTemp;
			m_moveAngle += ANGLE_DIFF;
		}
	}

	for ( int x = 0; x < CUBIE_COUNT; ++x )
		for ( int y = 0; y < CUBIE_COUNT; ++y )
			for ( int z = 0; z < CUBIE_COUNT; ++z )
				// only draw cubies in the outer layer
				if ( x == 0 || x == CUBIE_COUNT - 1 || y == 0 || y == CUBIE_COUNT - 1 || z == 0 || z == CUBIE_COUNT - 1 )
					drawCubie( x, y, z );
*/
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
/*	const GLfloat centerDiff = ( -1 * CUBIE_COUNT ) / 2.0 + 0.5;

	if ( m_moveType != MT_NONE && m_moveLayer != -1 )
	{
		if (	( z == m_moveLayer && MoveParams::vec( m_moveType ).z() != 0 ) ||
				( x == m_moveLayer && MoveParams::vec( m_moveType ).x() != 0 ) ||
				( y == m_moveLayer && MoveParams::vec( m_moveType ).y() != 0 )
			)
		{
			glPushMatrix();

			GLfloat MatrixRes[ 16 ];
			m_moveQuat.getTrMatrix( MatrixRes );
			glMultMatrixf( MatrixRes );
			setCubieVertices( x + centerDiff, y + centerDiff, z + centerDiff, CUBE_EDGE, x, y, z );

			glPopMatrix();
		}
		else setCubieVertices( x + centerDiff, y + centerDiff, z + centerDiff, CUBE_EDGE, x, y, z );
	}
	else setCubieVertices( x + centerDiff, y + centerDiff, z + centerDiff, CUBE_EDGE, x, y, z );
*/
}

void RCubeObject::setCubeVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize ) const
{
/*	const GLfloat halfSize = cubeSize / 2.0 - 0.1;

	glColor3f( Colors::colR( RC_FG ), Colors::colG( RC_FG ), Colors::colB( RC_FG ) );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBegin( GL_QUADS );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );  // Up
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );

	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );	// Front
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );

	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );	// Down
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );	// Back
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );

	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );	// Left
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );	// Right
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	glEnd();*/
}

void RCubeObject::setCubieVertices( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize,
	const int x, const int y, const int z ) const
{
	/*const GLfloat halfSize = cubeSize / 2 - 0.05;

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBegin( GL_QUADS );

	// Up
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_UP ),
			m_RCModel->cubie( x, y, z ).colG( AX_UP ), m_RCModel->cubie( x, y, z ).colB( AX_UP ) );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );

	if ( y == CUBIE_COUNT - 1 && !isAxisVisible( AX_UP ) )
	{
		glVertex3f( pX + halfSize, pY + 2.0 + halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY + 2.0 + halfSize, pZ - halfSize );
		glVertex3f( pX - halfSize, pY + 2.0 + halfSize, pZ + halfSize );
		glVertex3f( pX + halfSize, pY + 2.0 + halfSize, pZ + halfSize );
	}

	// Front
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_FRONT ),
			m_RCModel->cubie( x, y, z ).colG( AX_FRONT ), m_RCModel->cubie( x, y, z ).colB( AX_FRONT ) );

	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );

	if ( z == CUBIE_COUNT - 1 && !isAxisVisible( AX_FRONT ) )
	{
		glVertex3f( pX + halfSize, pY + halfSize, pZ + 2.0 + halfSize );
		glVertex3f( pX - halfSize, pY + halfSize, pZ + 2.0 + halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ + 2.0 + halfSize );
		glVertex3f( pX + halfSize, pY - halfSize, pZ + 2.0 + halfSize );
	}

	// Down
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_DOWN ),
			m_RCModel->cubie( x, y, z ).colG( AX_DOWN ), m_RCModel->cubie( x, y, z ).colB( AX_DOWN ) );

	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	if ( y == 0 && !isAxisVisible( AX_DOWN ) )
	{
		glVertex3f( pX + halfSize, pY - 2.0 - halfSize, pZ + halfSize );
		glVertex3f( pX - halfSize, pY - 2.0 - halfSize, pZ + halfSize );
		glVertex3f( pX - halfSize, pY - 2.0 - halfSize, pZ - halfSize );
		glVertex3f( pX + halfSize, pY - 2.0 - halfSize, pZ - halfSize );
	}

	// Back
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_BACK ),
			m_RCModel->cubie( x, y, z ).colG( AX_BACK ), m_RCModel->cubie( x, y, z ).colB( AX_BACK ) );

	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );

	if ( z == 0 && !isAxisVisible( AX_BACK ) )
	{
		glVertex3f( pX + halfSize, pY - halfSize, pZ - 2.0 - halfSize );
		glVertex3f( pX - halfSize, pY - halfSize, pZ - 2.0 - halfSize );
		glVertex3f( pX - halfSize, pY + halfSize, pZ - 2.0 - halfSize );
		glVertex3f( pX + halfSize, pY + halfSize, pZ - 2.0 - halfSize );
	}

	// Left
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_LEFT ),
			m_RCModel->cubie( x, y, z ).colG( AX_LEFT ), m_RCModel->cubie( x, y, z ).colB( AX_LEFT ) );

	glVertex3f( pX - halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX - halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ - halfSize );
	glVertex3f( pX - halfSize, pY - halfSize, pZ + halfSize );

	if ( x == 0 && !isAxisVisible( AX_LEFT ) )
	{
		glVertex3f( pX - 2.0 - halfSize, pY + halfSize, pZ - halfSize );
		glVertex3f( pX - 2.0 - halfSize, pY + halfSize, pZ + halfSize );
		glVertex3f( pX - 2.0 - halfSize, pY - halfSize, pZ + halfSize );
		glVertex3f( pX - 2.0 - halfSize, pY - halfSize, pZ - halfSize );
	}

	// Right
	glColor3f( m_RCModel->cubie( x, y, z ).colR( AX_RIGHT ),
			m_RCModel->cubie( x, y, z ).colG( AX_RIGHT ), m_RCModel->cubie( x, y, z ).colB( AX_RIGHT ) );

	glVertex3f( pX + halfSize, pY + halfSize, pZ - halfSize );
	glVertex3f( pX + halfSize, pY + halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ + halfSize );
	glVertex3f( pX + halfSize, pY - halfSize, pZ - halfSize );

	if ( x == CUBIE_COUNT - 1 && !isAxisVisible( AX_RIGHT ) )
	{
		glVertex3f( pX + 2.0 + halfSize, pY + halfSize, pZ - halfSize );
		glVertex3f( pX + 2.0 + halfSize, pY + halfSize, pZ + halfSize );
		glVertex3f( pX + 2.0 + halfSize, pY - halfSize, pZ + halfSize );
		glVertex3f( pX + 2.0 + halfSize, pY - halfSize, pZ - halfSize );
	}

	glEnd();*/
}
