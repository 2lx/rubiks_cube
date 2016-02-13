#include "all.h"

#include "playstate.h"
#include "rcubeparams.h"
#include "rcubeobject.h"
#include "shader.h"
#include "shaderprogram.h"

#include <glm/gtc/matrix_transform.hpp>

PlayState PlayState::m_PlayState;

void PlayState::init()
{
	// Setup shader program
	Shader vertexShader( GL_VERTEX_SHADER );
	vertexShader.loadFromFile( "glsl/shader.v.glsl" );
	vertexShader.compile();

	Shader fragmentShader( GL_FRAGMENT_SHADER );
	fragmentShader.loadFromFile( "glsl/shader.f.glsl" );
	fragmentShader.compile();

	m_shaderPr = new ShaderProgram();
	m_shaderPr->attachShader( vertexShader );
	m_shaderPr->attachShader( fragmentShader );
	m_shaderPr->linkProgram();

	m_RCube = new RCubeObject( m_shaderPr );

	glGenBuffers( 1, &m_VBOScreenVertices );
	glBindBuffer( GL_ARRAY_BUFFER, m_VBOScreenVertices );
	glBufferData( GL_ARRAY_BUFFER, sizeof( m_aScreenVertices ), m_aScreenVertices, GL_STATIC_DRAW );

	m_attrScreenVertices = m_shaderPr->addAttribute( "scrVertex" );
	m_UniIsBG = m_shaderPr->addUniform( "isBG" );
	m_UniPlasmaSpeed = m_shaderPr->addUniform( "plasmaSpeed" );

	glEnable( GL_BLEND );
	glEnable(GL_DEPTH_TEST);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	setProjection( m_prType );
}

void PlayState::cleanup()
{
	delete m_RCube;
	delete m_shaderPr;
}

void PlayState::setProjection( const ProjectionType pType )
{
	const float vMargin = ( 1 + RC::CUBIE_COUNT );
	const float hMargin = vMargin * ( float ) m_screenWidth / ( float ) m_screenHeight;

	m_mProjection = glm::ortho( -hMargin, hMargin, -vMargin, vMargin, 0.0f, 40.0f );
	m_mModel = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.5f, -20.0f ) );

	if ( pType == PT_ISOMETRIC )
		m_mView =
			glm::rotate( glm::mat4( 1.0f ), glm::radians( 35.264f ), glm::vec3( 1, 0, 0 ) ) * // X axis
			glm::rotate( glm::mat4( 1.0f ), glm::radians( 45.0f ), glm::vec3( 0, 1, 0 ) ) * // Y axis
			glm::rotate( glm::mat4( 1.0f ), glm::radians( 0.0f ), glm::vec3( 0, 0, 1 ) );  // Z axis
	else if ( pType == PT_DIMETRIC )
		m_mView = { // cavalier view
			1.0f, 		0.0f, 0.0f, 0.0f,
			0.0f, 		1.0f, 0.0f, 0.0f,
		 0.3345f,	-0.3345f, 1.0f, 0.0f,
			0.0f, 		0.0f, 0.0f, 1.0f };

	m_matrCamera = m_mProjection *  m_mModel * m_mView;
}

void PlayState::pause()
{

}

void PlayState::resume()
{

}

void PlayState::handleEvents( CGameEngine* game )
{
	static bool lastEvent = false;
	SDL_Event event;

	const Uint32 start = SDL_GetTicks();
	bool allEventsRunOut = false;

	while ( SDL_PollEvent( &event ) && !allEventsRunOut && ( SDL_GetTicks() - start ) < 15 )
	{
		switch( event.type )
		{
		case SDL_QUIT:
			game->quit();
			break;

		case SDL_WINDOWEVENT:
			if ( event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
			{
				m_screenWidth = event.window.data1;
				m_screenHeight = event.window.data2;

				glViewport( 0, 0, m_screenWidth, m_screenHeight );
				setProjection( m_prType );
				m_needRedraw = true;
			}
			break;

		case SDL_KEYDOWN:
			lastEvent = true;
			switch( event.key.keysym.sym )
			{
			case SDLK_ESCAPE:
				game->quit();
				break;

			case SDLK_UP:
			case SDLK_w: 		m_keyQ.keyDown( RC::GK::ROTATEUP ); break;
			case SDLK_DOWN:
			case SDLK_s: 		m_keyQ.keyDown( RC::GK::ROTATEDOWN ); break;
			case SDLK_LEFT:
			case SDLK_a:		m_keyQ.keyDown( RC::GK::ROTATELEFT ); break;
			case SDLK_RIGHT:
			case SDLK_d:		m_keyQ.keyDown( RC::GK::ROTATERIGHT ); break;
			case SDLK_PAGEDOWN:
			case SDLK_e:		m_keyQ.keyDown( RC::GK::ROTATEACW ); break;
			case SDLK_q:		m_keyQ.keyDown( RC::GK::ROTATECW ); break;
			case SDLK_i:		m_keyQ.keyDown( RC::GK::MOVEF ); break;
			case SDLK_u:		m_keyQ.keyDown( RC::GK::MOVEFI ); break;
			case SDLK_p:		m_keyQ.keyDown( RC::GK::MOVEB ); break;
			case SDLK_o:		m_keyQ.keyDown( RC::GK::MOVEBI ); break;
			case SDLK_k:		m_keyQ.keyDown( RC::GK::MOVEL ); break;
			case SDLK_j:		m_keyQ.keyDown( RC::GK::MOVELI );	break;
			case SDLK_SEMICOLON:m_keyQ.keyDown( RC::GK::MOVER ); break;
			case SDLK_l:		m_keyQ.keyDown( RC::GK::MOVERI ); break;
			case SDLK_m:		m_keyQ.keyDown( RC::GK::MOVEU ); break;
			case SDLK_n:		m_keyQ.keyDown( RC::GK::MOVEUI ); break;
			case SDLK_PERIOD: 	m_keyQ.keyDown( RC::GK::MOVED ); break;
			case SDLK_COMMA: 	m_keyQ.keyDown( RC::GK::MOVEDI ); break;
			case SDLK_RETURN: 	m_keyQ.keyDown( RC::GK::GAMEPROJ ); break;
			case SDLK_SPACE: 	m_keyQ.keyDown( RC::GK::GAMECOLOR ); break;
			case SDLK_F1: 		m_keyQ.keyDown( RC::GK::GAMERESET ); break;
			case SDLK_F4: 		m_keyQ.keyDown( RC::GK::GAMEMIX ); break;
			case SDLK_z: 		m_keyQ.keyDown( RC::GK::GAMEUNDO ); break;
			}
			break;
		case SDL_KEYUP:
			lastEvent = true;
			switch( event.key.keysym.sym )
			{
			case SDLK_UP:
			case SDLK_w: 		m_keyQ.keyUp( RC::GK::ROTATEUP ); break;
			case SDLK_DOWN:
			case SDLK_s: 		m_keyQ.keyUp( RC::GK::ROTATEDOWN ); break;
			case SDLK_LEFT:
			case SDLK_a:		m_keyQ.keyUp( RC::GK::ROTATELEFT ); break;
			case SDLK_RIGHT:
			case SDLK_d:		m_keyQ.keyUp( RC::GK::ROTATERIGHT ); break;
			case SDLK_PAGEDOWN:
			case SDLK_e:		m_keyQ.keyUp( RC::GK::ROTATEACW ); break;
			case SDLK_q:		m_keyQ.keyUp( RC::GK::ROTATECW ); break;
			case SDLK_i:		m_keyQ.keyUp( RC::GK::MOVEF ); break;
			case SDLK_u:		m_keyQ.keyUp( RC::GK::MOVEFI ); break;
			case SDLK_p:		m_keyQ.keyUp( RC::GK::MOVEB ); break;
			case SDLK_o:		m_keyQ.keyUp( RC::GK::MOVEBI ); break;
			case SDLK_k:		m_keyQ.keyUp( RC::GK::MOVEL ); break;
			case SDLK_j:		m_keyQ.keyUp( RC::GK::MOVELI );	break;
			case SDLK_SEMICOLON:m_keyQ.keyUp( RC::GK::MOVER ); break;
			case SDLK_l:		m_keyQ.keyUp( RC::GK::MOVERI ); break;
			case SDLK_m:		m_keyQ.keyUp( RC::GK::MOVEU ); break;
			case SDLK_n:		m_keyQ.keyUp( RC::GK::MOVEUI ); break;
			case SDLK_PERIOD: 	m_keyQ.keyUp( RC::GK::MOVED ); break;
			case SDLK_COMMA: 	m_keyQ.keyUp( RC::GK::MOVEDI ); break;
			case SDLK_RETURN: 	m_keyQ.keyUp( RC::GK::GAMEPROJ ); break;
			case SDLK_SPACE: 	m_keyQ.keyUp( RC::GK::GAMECOLOR ); break;
			case SDLK_F1: 		m_keyQ.keyUp( RC::GK::GAMERESET ); break;
			case SDLK_F4: 		m_keyQ.keyUp( RC::GK::GAMEMIX ); break;
			case SDLK_z: 		m_keyQ.keyUp( RC::GK::GAMEUNDO ); break;
			}
			break;
		// mouse events
		case SDL_MOUSEBUTTONDOWN:
			lastEvent = true;
			switch( event.button.button )
			{
			case SDL_BUTTON_LEFT:
				m_pMBegin = getGLPos( event.button.x, event.button.y );

				m_keyQ.keyDown( RC::GK::MOUSEMOVE, false );
				break;
			case SDL_BUTTON_RIGHT:
				m_pRBegin = getGLPos( event.button.x, event.button.y );
				m_keyQ.keyDown( RC::GK::MOUSEROTATE, false );
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			lastEvent = true;
			switch( event.button.button )
			{
			case SDL_BUTTON_LEFT:
				if ( glm::length( m_pMBegin ) > 0 )
					m_pMEnd = getGLPos( event.button.x, event.button.y );
				break;
			case SDL_BUTTON( SDL_BUTTON_RIGHT ): // SDL2 bug
				if ( glm::length( m_pRBegin ) > 0 )
					m_pREnd = getGLPos( event.button.x, event.button.y );
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			lastEvent = true;
			switch( event.button.button )
			{
			case SDL_BUTTON_LEFT:
				m_keyQ.keyUp( RC::GK::MOUSEMOVE );
				m_pMBegin = { 0.0f, 0.0f, 0.0f };
				m_pMEnd = { 0.0f, 0.0f, 0.0f };
				break;
			case SDL_BUTTON_RIGHT:
				m_keyQ.keyUp( RC::GK::MOUSEROTATE );
				m_pRBegin = { 0.0f, 0.0f, 0.0f };
				m_pREnd = { 0.0f, 0.0f, 0.0f };
				break;
			}
			break;
		default:
			if ( lastEvent )
				allEventsRunOut = true;

			break;
		}
	}
}

glm::vec3 PlayState::getGLPos( const int mX, const int mY ) const
{
	GLint viewport[ 4 ];
	glGetIntegerv( GL_VIEWPORT, viewport ); // 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT

	glm::dmat4 modelView = m_mModel * m_mView;

	GLfloat winX, winY, winZ;
	winX = ( float ) mX;
	winY = ( float ) viewport[ 3 ] - ( float ) mY;
	glReadPixels( winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	GLdouble posX, posY, posZ;
	gluUnProject( winX, winY, winZ,
		glm::value_ptr( modelView ), glm::value_ptr( m_mProjection ), viewport,
		&posX, &posY, &posZ );

	return glm::vec3( posX, posY, posZ );
}

void PlayState::update( CGameEngine * game )
{
	// primarily processing mouse events
	if ( m_keyQ.isHold( RC::GK::MOUSEMOVE ) )
	{
		// processing cube moves with mouse
		if ( !m_RCube->isMoving() && !m_RCube->isRotating() )
		{
			if ( glm::distance( m_pMBegin, m_pMEnd ) > 0.5 && glm::length( m_pMBegin ) > 0 && glm::length( m_pMEnd ) > 0 )
			{
				RC::MT mt = m_RCube->setMoveByCoords( m_pMBegin, m_pMEnd );
				m_keyQ.prevPush( RC::GKPar::fromMT( mt ) );
				m_keyQ.processKey( RC::GK::MOUSEMOVE );

				m_pMBegin = { 0.0f, 0.0f, 0.0f };
				m_pMEnd = { 0.0f, 0.0f, 0.0f };
			}
		}
	}
	else if ( m_keyQ.isHold( RC::GK::MOUSEROTATE ) )
	{
		// processing cube rotates with mouse
		if ( !m_RCube->isRotating() )
		{
			if ( glm::distance( m_pRBegin, m_pREnd ) > 0.5 && glm::length( m_pRBegin ) > 0 && glm::length( m_pREnd ) > 0 )
			{
				RC::RT rt = m_RCube->setRotateByCoords( m_pRBegin, m_pREnd );
				m_keyQ.prevPush( RC::GKPar::fromRT( rt ) );
				m_keyQ.processKey( RC::GK::MOUSEROTATE );

				m_pRBegin = { 0.0f, 0.0f, 0.0f };
				m_pREnd = { 0.0f, 0.0f, 0.0f };
			}
		}
	}
	else if ( m_keyQ.qSize() > 0 )
	{
		const RC::GK gk = m_keyQ.qCurKey();
		const bool enableAll = !m_RCube->isMoving() && !m_RCube->isRotating();

		if ( RC::GKPar::enableWithMove( gk ) || enableAll )
		{
			m_keyQ.qPopKey();

			switch ( gk )
			{
			// processing cube rotates
			case RC::GK::ROTATEDOWN:
			case RC::GK::ROTATEUP:
			case RC::GK::ROTATERIGHT:
			case RC::GK::ROTATELEFT:
			case RC::GK::ROTATEACW:
			case RC::GK::ROTATECW:
				m_RCube->setRotate( RC::GKPar::toRT( gk ) );
				break;
			// processing cube moves
			case RC::GK::MOVEB:
			case RC::GK::MOVEBI:
			case RC::GK::MOVED:
			case RC::GK::MOVEDI:
			case RC::GK::MOVEF:
			case RC::GK::MOVEFI:
			case RC::GK::MOVEL:
			case RC::GK::MOVELI:
			case RC::GK::MOVER:
			case RC::GK::MOVERI:
			case RC::GK::MOVEU:
			case RC::GK::MOVEUI:
			case RC::GK::MOVEFM:
			case RC::GK::MOVEFMI:
			case RC::GK::MOVEUM:
			case RC::GK::MOVEUMI:
			case RC::GK::MOVERM:
			case RC::GK::MOVERMI:
				m_RCube->setMove( RC::GKPar::toMT( gk ) );
				break;
			// processing projection setup
			case RC::GK::GAMEPROJ:
				m_prType = ProjectionType ( ( m_prType + 1 ) % PT_COUNT );
				setProjection( m_prType );
				m_needRedraw = true;
				break;
			// processing colors setup
			case RC::GK::GAMECOLOR:
				m_RCube->incCurScheme();
				m_needRedraw = true;
				break;
			// other
			case RC::GK::GAMERESET:
				m_RCube->reset();
				m_needRedraw = true;
				break;
			case RC::GK::GAMEMIX:
			{
				srand( time( 0 ) );
				const int mCount = 60;

				for ( int i = 0; i < mCount; i++ )
				{
					if ( rand() % 10 > 6 )
					{
						const int rRand = rand() % int( RC::RT::COUNT );
						if ( RC::RTPar::isRT( rRand ) )
							m_keyQ.qPushKey( RC::GKPar::fromRT( RC::RT( rRand ) ) );
					}
					else
					{
						const int mRand = rand() % int( RC::MT::COUNT );
						if ( RC::MTPar::isMT( mRand ) )
							m_keyQ.qPushKey( RC::GKPar::fromMT( RC::MT( mRand ) ) );
					}
				}

				break;
			}
			case RC::GK::GAMEUNDO:
			{
				const RC::GK pgk = m_keyQ.prevPop();
				const RC::GK gk = RC::GKPar::prevGK( pgk );
				if ( gk == RC::GK::NONE )
					break;

				std::cout << RC::GKPar::str( pgk ) << " -> " << RC::GKPar::str( gk ) << std::endl;

				if ( RC::GKPar::toRT( gk ) != RC::RT::NONE )
					m_RCube->setRotate( RC::GKPar::toRT( gk ) );
				else if ( RC::GKPar::toMT( gk ) != RC::MT::NONE )
					m_RCube->setMove( RC::GKPar::toMT( gk ) );

				break;
			}
			default:
				break;
			}
		}
	}

	m_RCube->update();
}

void PlayState::drawBackground()
{
	glUniform1f( m_UniIsBG, 1.0 );

	glEnableVertexAttribArray( m_attrScreenVertices );
	glBindBuffer( GL_ARRAY_BUFFER, m_VBOScreenVertices );
	glVertexAttribPointer( m_attrScreenVertices, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	glDrawArrays( GL_QUADS, 0, 4 );

	glDisableVertexAttribArray( m_attrScreenVertices );

	glUniform1f( m_UniIsBG, 0.0 );
}

void PlayState::draw( CGameEngine * game )
{
	static int drCount = 0;

//	if ( m_needRedraw || m_RCube->isRotating() || m_RCube->isMoving() )
	{
		Uint32 start = SDL_GetTicks();

	//	glClearColor( 47.0 / 255.0, 47.0 / 255.0, 47.0 / 255.0, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( m_shaderPr->id() );

		m_RCube->rotateObject();

		drawBackground();
		m_RCube->drawObject( m_matrCamera );

		if ( SDL_GetTicks() - start < SCREEN_TICK_PER_FRAME )
			SDL_Delay( SCREEN_TICK_PER_FRAME - ( SDL_GetTicks() - start ) );

		drCount++;
		glUniform1f( m_UniPlasmaSpeed, float( drCount ) );
		if ( drCount > 1 ) m_needRedraw = false;
#ifdef MY_DEBUG
//		if ( drCount % 5 == 0 )
//			std::cout << "DrawCount: " << drCount << std::endl;
#endif // MY_DEBUG
	}
}

