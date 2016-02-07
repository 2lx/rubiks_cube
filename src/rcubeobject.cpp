#include "all.h"

#include "rcubeobject.h"
#include "rcubeparams.h"
#include "shaderprogram.h"
#include "rcubemodel.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

RCubeObject::RCubeObject( ShaderProgram * shaderPr )
{
	m_RCModel = new RC::CubeModel();

	m_VBOCubeVertices = loadGLArrayBuffer( m_aCubeVertices, sizeof( m_aCubeVertices ) );
	m_VBOTexCoords = loadGLArrayBuffer( m_aTexCoords, sizeof( m_aTexCoords ) );
	m_VBOTexIndex = loadGLArrayBuffer( m_aTexIndex, sizeof( m_aTexIndex ), GL_STREAM_DRAW );

	m_attrCubeVertices = shaderPr->addAttribute( "cVertex" );
	m_attrTexCoords = shaderPr->addAttribute( "texCoord" );
	m_attrTexIndex = shaderPr->addAttribute( "texIndex" );

	// get texture params
	SDL_Surface * res_texture = IMG_Load( "glsl/texture.png" );
	if ( res_texture == NULL )
		std::cout << "IMG_Load: " << SDL_GetError() << std::endl;
	else
	{
		m_texCount = res_texture->h / ( res_texture->w / 6 );
		m_texCurScheme = 1;

		SDL_FreeSurface( res_texture );

		// load texture
		m_VBOTexUnionID = loadGLTexture2D( "glsl/texture.png" );
		m_UniTexUnionID = shaderPr->addUniform( "texUnion" );
	}

	m_UniMVP = shaderPr->addUniform( "mvp" );
	m_UniTexCount = shaderPr->addUniform( "texCount" );
	m_UniTexCurScheme = shaderPr->addUniform( "texCurScheme" );
}

RCubeObject::~RCubeObject()
{
	glDeleteBuffers( 1, &m_VBOCubeVertices );
	glDeleteBuffers( 1, &m_VBOTexCoords );
	glDeleteBuffers( 1, &m_VBOTexIndex );
	glDeleteTextures( 1, &m_VBOTexUnionID );

	delete m_RCModel;
}

void RCubeObject::setMove( const RC::MT mt )
{
	// calculate new move type
	const glm::vec3 vec = RC::MTPar::vec( mt );
	bool cw = RC::MTPar::clockwise( mt );
	int lay = RC::MTPar::layer( mt );

	const glm::vec3 vecRot = vec * m_rotateQuat;

	if ( glm::dot( vecRot, glm::vec3( 1.0f, 1.0f, 1.0f ) ) < 0 )
	{
		lay = RC::CUBIE_COUNT - 1 - lay;
		cw = !cw;
	}

	const RC::RA ax = RC::RAPar::closestRA( vecRot );
	m_moveType = RC::MTPar::equalMT( ax, lay, cw );
	if ( m_moveType == RC::MT::NONE )
		return;

	// calculate move params
	m_moveLayer = lay;
	m_moveMix = 0;
	float angle = glm::radians( 90.0f );
	m_newMoveQuat = glm::angleAxis( ( cw ) ? -angle : angle, RC::MTPar::vec( m_moveType ) );
}

RC::MT RCubeObject::setMoveByCoords( const glm::vec3 & pBeg, const glm::vec3 & pEnd )
{
	const float cOffset = RC::CUBIE_COUNT / 2.0f;

	// if 1st point don't lie on the surface of the cube
	if ( std::abs( pBeg.x ) > cOffset + 0.1 || std::abs( pBeg.y ) > cOffset + 0.1 || std::abs( pBeg.z ) > cOffset + 0.1 )
		return RC::MT::NONE;

	// get close rotation axis
	const glm::vec3 rvBeg = pBeg * m_rotateQuat;
	const glm::vec3 rvEnd = pEnd * m_rotateQuat;
	glm::vec3 pRes = glm::cross( rvBeg, rvEnd ) ;

	// get closest rotation axis vector
    const RC::RA ra = RC::RAPar::closestRA( pRes );
	if ( ra == RC::RA::NONE )
		return RC::MT::NONE;

	const glm::vec3 vAx = RC::RAPar::vec( ra );

#ifdef NDEBUG
	std::cout << pBeg.x << " " << pBeg.y << " " << pBeg.z << std::endl;
	std::cout << pEnd.x << " " << pEnd.y << " " << pEnd.z << std::endl;
	std::cout << rvBeg.x << " " << rvBeg.y << " " << rvBeg.z << std::endl;
	std::cout << rvEnd.x << " " << rvEnd.y << " " << rvEnd.z << std::endl;
	std::cout << pRes.x << " " << pRes.y << " " << pRes.z << std::endl;
	std::cout << vAx.x  << " " << vAx.y  << " " << vAx.z  << std::endl;
#endif // NDEBUG

	// calculate clockwise
	bool cw;
	if ( glm::dot( glm::cross( vAx, rvEnd ), rvBeg ) > 0 )
		cw = true;
	else cw = false;

	// find move layer
	int lay = 0;

	if ( ra == RC::RA::X )
		lay = floor( rvBeg.x + cOffset );
	else if ( ra == RC::RA::Y )
		lay = floor( rvBeg.y + cOffset );
	else if ( ra == RC::RA::Z )
		lay = floor( rvBeg.z + cOffset );
	else lay = -1;

	if ( lay < 0 || lay > RC::CUBIE_COUNT - 1 )
		return RC::MT::NONE;

	// get move type
	RC::MT nMT = RC::MTPar::equalMT( ra, lay, cw );
	if ( nMT == RC::MT::NONE )
		return RC::MT::NONE;

	m_moveType = nMT;
	m_moveLayer = lay;
	m_moveMix = 0;
	float angle = glm::radians( 90.0f );
	m_newMoveQuat = glm::angleAxis( ( cw ) ? -angle : angle, vAx );

	return m_moveType;
}

void RCubeObject::reset()
{
	m_RCModel->reset();
}

void RCubeObject::update()
{
	glUniform1f( m_UniTexCount, m_texCount );
	glUniform1f( m_UniTexCurScheme, m_texCurScheme );

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
			m_moveType = RC::MT::NONE;
			m_moveLayer = -1;

			m_moveQuat = glm::quat();
			m_moveMix = -1;
		}
	}
}

void RCubeObject::drawObject( const glm::mat4 & pmv )
{
	const float offCenter = RC::CUBIE_COUNT / 2.0f - 0.5;

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

	for ( int x = 0; x < RC::CUBIE_COUNT; ++x )
		for ( int y = 0; y < RC::CUBIE_COUNT; ++y )
			for ( int z = 0; z < RC::CUBIE_COUNT; ++z )
				// only draw cubies in the outer layer
				if ( x == 0 || x == RC::CUBIE_COUNT - 1 || y == 0 || y == RC::CUBIE_COUNT - 1 || z == 0 || z == RC::CUBIE_COUNT - 1 )
				{
					// update texture index matrix
					for ( int i = 0; i < 4; i++ )
					{
						m_aTexIndex[ 4*( int )RC::CF::FRONT + i ] 	= m_RCModel->cubie( x, y, z ).colInd( RC::CF::FRONT );
						m_aTexIndex[ 4*( int )RC::CF::UP + i ]		= m_RCModel->cubie( x, y, z ).colInd( RC::CF::UP );
						m_aTexIndex[ 4*( int )RC::CF::BACK + i ] 	= m_RCModel->cubie( x, y, z ).colInd( RC::CF::BACK );
						m_aTexIndex[ 4*( int )RC::CF::DOWN + i ] 	= m_RCModel->cubie( x, y, z ).colInd( RC::CF::DOWN );
						m_aTexIndex[ 4*( int )RC::CF::LEFT + i ] 	= m_RCModel->cubie( x, y, z ).colInd( RC::CF::LEFT );
						m_aTexIndex[ 4*( int )RC::CF::RIGHT + i ] 	= m_RCModel->cubie( x, y, z ).colInd( RC::CF::RIGHT );
					}

					m_VBOTexIndex = loadGLArrayBuffer( m_aTexIndex, sizeof( m_aTexIndex ) );

					// calculate offset of cubie
					glm::mat4 offset = glm::translate( glm::mat4( 1.0f ),
							glm::vec3( x - offCenter, y - offCenter, z - offCenter ) );

					// calculate model view projection matrix
					glm::mat4 mRes;

					if ( m_moveType != RC::MT::NONE && m_moveLayer != -1 )
					{
						const RC::RA ax = RC::MTPar::axis( m_moveType );

						if  (	( x == m_moveLayer && ax == RC::RA::X ) ||
								( y == m_moveLayer && ax == RC::RA::Y ) ||
								( z == m_moveLayer && ax == RC::RA::Z ) )
							mRes = mvpRM * offset;
						else mRes = mvpR * offset;
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
	glVertexAttribPointer( m_attrTexIndex, 1, GL_INT, GL_FALSE, 0, 0 );

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
