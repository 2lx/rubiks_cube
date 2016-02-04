#include "all.h"

#include "playstate.h"
#include "rcubeparams.h"
#include "rcubeobject.h"
#include "shader.h"
#include "shaderprogram.h"

#include <glm/gtc/matrix_transform.hpp>

CPlayState CPlayState::m_PlayState;

void CPlayState::Init()
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

	std::cout.flush();

	glEnable( GL_BLEND );
	glEnable(GL_DEPTH_TEST);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	setProjection( m_prType );
}

void CPlayState::Cleanup()
{
	delete m_RCube;
	delete m_shaderPr;

	MoveParams::cleanup();
//	Colors::cleanup();
	AxisParams::cleanup();
}

void CPlayState::setProjection( const ProjectionType pType )
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

void CPlayState::Pause()
{

}

void CPlayState::Resume()
{

}

void CPlayState::HandleEvents( CGameEngine* game )
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
			game->Quit();
			break;

		case SDL_WINDOWEVENT:
			if ( event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
			{
				m_screenWidth = event.window.data1;
				m_screenHeight = event.window.data2;

				glViewport( 0, 0, m_screenWidth, m_screenHeight );
				m_needRedraw = true;
			}
			break;

		case SDL_KEYDOWN:
			lastEvent = true;
			switch( event.key.keysym.sym )
			{
			case SDLK_ESCAPE:
				game->Quit();
				break;

			case SDLK_UP:
			case SDLK_w: 	m_keyQ.keyDown( GK_LOOKUP ); break;
			case SDLK_DOWN:
			case SDLK_s: 	m_keyQ.keyDown( GK_LOOKDOWN ); break;
			case SDLK_LEFT:
			case SDLK_a:	m_keyQ.keyDown( GK_LOOKLEFT ); break;
			case SDLK_RIGHT:
			case SDLK_d:	m_keyQ.keyDown( GK_LOOKRIGHT ); break;
			case SDLK_PAGEDOWN:
			case SDLK_e:	m_keyQ.keyDown( GK_ROTATECOUNTERCLOCKWISE ); break;
			case SDLK_q:	m_keyQ.keyDown( GK_ROTATECLOCKWISE ); break;
			case SDLK_i:	m_keyQ.keyDown( GK_MOVEFRONT ); break;
			case SDLK_u:	m_keyQ.keyDown( GK_MOVEFRONTINV ); break;
			case SDLK_p:	m_keyQ.keyDown( GK_MOVEBACK ); break;
			case SDLK_o:	m_keyQ.keyDown( GK_MOVEBACKINV ); break;
			case SDLK_k:	m_keyQ.keyDown( GK_MOVELEFT ); break;
			case SDLK_j:	m_keyQ.keyDown( GK_MOVELEFTINV );	break;
			case SDLK_SEMICOLON: m_keyQ.keyDown( GK_MOVERIGHT ); break;
			case SDLK_l:	m_keyQ.keyDown( GK_MOVERIGHTINV ); break;
			case SDLK_m:	m_keyQ.keyDown( GK_MOVEUP ); break;
			case SDLK_n:	m_keyQ.keyDown( GK_MOVEUPINV ); break;
			case SDLK_PERIOD: m_keyQ.keyDown( GK_MOVEDOWN ); break;
			case SDLK_COMMA: m_keyQ.keyDown( GK_MOVEDOWNINV ); break;
			case SDLK_RETURN: m_keyQ.keyDown( GK_CHANGEPROJ ); break;
			}
			break;
		case SDL_KEYUP:
			lastEvent = true;
			switch( event.key.keysym.sym )
			{
			case SDLK_UP:
			case SDLK_w: 	m_keyQ.keyUp( GK_LOOKUP ); break;
			case SDLK_DOWN:
			case SDLK_s: 	m_keyQ.keyUp( GK_LOOKDOWN ); break;
			case SDLK_LEFT:
			case SDLK_a:	m_keyQ.keyUp( GK_LOOKLEFT ); break;
			case SDLK_RIGHT:
			case SDLK_d:	m_keyQ.keyUp( GK_LOOKRIGHT ); break;
			case SDLK_PAGEDOWN:
			case SDLK_e:	m_keyQ.keyUp( GK_ROTATECOUNTERCLOCKWISE ); break;
			case SDLK_q:	m_keyQ.keyUp( GK_ROTATECLOCKWISE ); break;
			case SDLK_i:	m_keyQ.keyUp( GK_MOVEFRONT ); break;
			case SDLK_u:	m_keyQ.keyUp( GK_MOVEFRONTINV ); break;
			case SDLK_p:	m_keyQ.keyUp( GK_MOVEBACK ); break;
			case SDLK_o:	m_keyQ.keyUp( GK_MOVEBACKINV ); break;
			case SDLK_k:	m_keyQ.keyUp( GK_MOVELEFT ); break;
			case SDLK_j:	m_keyQ.keyUp( GK_MOVELEFTINV );	break;
			case SDLK_SEMICOLON: m_keyQ.keyUp( GK_MOVERIGHT ); break;
			case SDLK_l:	m_keyQ.keyUp( GK_MOVERIGHTINV ); break;
			case SDLK_m:	m_keyQ.keyUp( GK_MOVEUP ); break;
			case SDLK_n:	m_keyQ.keyUp( GK_MOVEUPINV ); break;
			case SDLK_PERIOD: m_keyQ.keyUp( GK_MOVEDOWN ); break;
			case SDLK_COMMA: m_keyQ.keyUp( GK_MOVEDOWNINV ); break;
			case SDLK_RETURN: m_keyQ.keyUp( GK_CHANGEPROJ ); break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			lastEvent = true;
			switch( event.button.button )
			{
			case SDL_BUTTON_LEFT:
				m_pBegin = getGLPos( event.button.x, event.button.y );

				m_keyQ.keyDown( GK_MOVEMOUSE );
				break;
			case SDL_BUTTON_RIGHT:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			lastEvent = true;
			switch( event.button.button )
			{
			case SDL_BUTTON_LEFT:
				if ( glm::length( m_pBegin ) > 0 )
					m_pEnd = getGLPos( event.button.x, event.button.y );

				break;
			case SDL_BUTTON_RIGHT:
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			lastEvent = true;
			switch( event.button.button )
			{
			case SDL_BUTTON_LEFT:

				m_keyQ.keyUp( GK_MOVEMOUSE );
				break;
			case SDL_BUTTON_RIGHT:
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

glm::vec3 CPlayState::getGLPos( const int mX, const int mY ) const
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

//	std::cout << posX << " " << posY << " " << posZ << std::endl;
	return glm::vec3( posX, posY, posZ );
}

void CPlayState::Update( CGameEngine * game )
{
	if ( /*!m_RCube->isRotating() &&*/ !m_keyQ.isHold( GK_MOVEMOUSE ) )
	{
        if ( m_keyQ.isNewDown( GK_LOOKDOWN ) )
		{
			m_RCube->setRotates( 1, 0, 0, false );
			m_keyQ.processKey( GK_LOOKDOWN );
		}
		if ( m_keyQ.isNewDown( GK_LOOKUP ) )
		{
			m_RCube->setRotates( 1, 0, 0, true );
			m_keyQ.processKey( GK_LOOKUP );
		}
		if ( m_keyQ.isNewDown( GK_LOOKRIGHT ) )
		{
			m_RCube->setRotates( 0, 1, 0, false );
			m_keyQ.processKey( GK_LOOKRIGHT );
		}
		if ( m_keyQ.isNewDown( GK_LOOKLEFT ) )
		{
			m_RCube->setRotates( 0, 1, 0, true );
			m_keyQ.processKey( GK_LOOKLEFT );
		}
		if ( m_keyQ.isNewDown( GK_ROTATECOUNTERCLOCKWISE ) )
		{
			m_RCube->setRotates( 0, 0, 1, true );
			m_keyQ.processKey( GK_ROTATECOUNTERCLOCKWISE );
		}
		if ( m_keyQ.isNewDown( GK_ROTATECLOCKWISE ) )
		{
			m_RCube->setRotates( 0, 0, 1, false );
			m_keyQ.processKey( GK_ROTATECLOCKWISE );
		}
	}

	if ( m_keyQ.isNewDown( GK_MOVEMOUSE ) && !m_RCube->isMoving() && !m_RCube->isRotating() )
	{
		if ( glm::distance( m_pBegin, m_pEnd ) > 0.5 && glm::length( m_pBegin ) > 0 && glm::length( m_pEnd ) > 0 )
		{
//			std::cout << m_pBegin.x << " " << m_pBegin.y << " " << m_pBegin.z << " " << std::endl;
//			std::cout << m_pEnd.x << " " << m_pEnd.y << " " << m_pEnd.z << " " << std::endl;
			m_RCube->setMoveByCoords( m_pBegin, m_pEnd );
			m_keyQ.processKey( GK_MOVEMOUSE );

			m_pBegin = { 0.0f, 0.0f, 0.0f };
			m_pEnd = { 0.0f, 0.0f, 0.0f };
		}
		else if ( !m_keyQ.isHold( GK_MOVEMOUSE ) )
		{
			m_keyQ.processKey( GK_MOVEMOUSE );
			m_pBegin = { 0.0f, 0.0f, 0.0f };
			m_pEnd = { 0.0f, 0.0f, 0.0f };
		}
	}

	if ( !m_RCube->isMoving() && !m_RCube->isRotating() && !m_keyQ.isHold( GK_MOVEMOUSE ) )
	{
		for ( int i = 0; i < GK_MOVELAST - GK_MOVEFIRST + 1; i++ )
		{
			if ( m_keyQ.isNewDown( GameKeys ( GK_MOVEFRONT + i ) ) )
			{
				m_RCube->setMove( ( RCMoveType ) ( MT_FRONT + i ) );
				m_keyQ.processKey( GameKeys ( GK_MOVEFRONT + i ) );
				break;
			}
		}
	}
/*
	if ( m_gkStates[ GK_CHANGECOLOR ].isNewDown() )
	{
		RC::Colors::incScheme();
		m_gkStates[ GK_CHANGECOLOR ].releaseNewDown();
		m_needRedraw = true;
	}
*/
	if ( m_keyQ.isNewDown( GK_CHANGEPROJ ) && !m_keyQ.isHold( GK_MOVEMOUSE ) )
	{
        m_prType = ProjectionType ( ( m_prType + 1 ) % PT_COUNT );
        setProjection( m_prType );
		m_keyQ.processKey( GK_CHANGEPROJ );
		m_needRedraw = true;
	}

	m_RCube->Update();
}

void CPlayState::Draw( CGameEngine * game )
{
	static int drCount = 0;

	if ( m_needRedraw || m_RCube->isRotating() || m_RCube->isMoving() )
	{
		Uint32 start = SDL_GetTicks();

		glClearColor( 0.3f, 0.4f, 0.4f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( m_shaderPr->id() );

		m_RCube->rotateObject();
		m_RCube->drawObject( m_matrCamera );

		if ( SDL_GetTicks() - start < SCREEN_TICK_PER_FRAME )
			SDL_Delay( SCREEN_TICK_PER_FRAME - ( SDL_GetTicks() - start ) );

		drCount++;
		if ( drCount > 1 ) m_needRedraw = false;
#ifdef MY_DEBUG
//		if ( drCount % 5 == 0 )
//			std::cout << "DrawCount: " << drCount << std::endl;
#endif // MY_DEBUG
	}
}

