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
			case SDLK_w: 	m_gkStates[ GK_LOOKUP ].keyDown(); break;
			case SDLK_DOWN:
			case SDLK_s: 	m_gkStates[ GK_LOOKDOWN ].keyDown(); break;
			case SDLK_LEFT:
			case SDLK_a:	m_gkStates[ GK_LOOKLEFT ].keyDown(); break;
			case SDLK_RIGHT:
			case SDLK_d:	m_gkStates[ GK_LOOKRIGHT ].keyDown(); break;
			case SDLK_PAGEDOWN:
			case SDLK_e:	m_gkStates[ GK_ROTATECOUNTERCLOCKWISE ].keyDown(); break;
			case SDLK_q:	m_gkStates[ GK_ROTATECLOCKWISE ].keyDown(); break;
			case SDLK_i:	m_gkStates[ GK_MOVEFRONT ].keyDown(); break;
			case SDLK_u:	m_gkStates[ GK_MOVEFRONTINV ].keyDown(); break;
			case SDLK_p:	m_gkStates[ GK_MOVEBACK ].keyDown(); break;
			case SDLK_o:	m_gkStates[ GK_MOVEBACKINV ].keyDown(); break;
			case SDLK_k:	m_gkStates[ GK_MOVELEFT ].keyDown(); break;
			case SDLK_j:	m_gkStates[ GK_MOVELEFTINV ].keyDown();	break;
			case SDLK_SEMICOLON: m_gkStates[ GK_MOVERIGHT ].keyDown(); break;
			case SDLK_l:	m_gkStates[ GK_MOVERIGHTINV ].keyDown(); break;
			case SDLK_m:	m_gkStates[ GK_MOVEUP ].keyDown(); break;
			case SDLK_n:	m_gkStates[ GK_MOVEUPINV ].keyDown(); break;
			case SDLK_PERIOD: m_gkStates[ GK_MOVEDOWN ].keyDown(); break;
			case SDLK_COMMA: m_gkStates[ GK_MOVEDOWNINV ].keyDown(); break;
			case SDLK_RETURN: m_gkStates[ GK_CHANGEPROJ ].keyDown(); break;
			}
			break;
		case SDL_KEYUP:
			switch( event.key.keysym.sym )
			{
			case SDLK_UP:
			case SDLK_w: 	m_gkStates[ GK_LOOKUP ].keyUp(); break;
			case SDLK_DOWN:
			case SDLK_s: 	m_gkStates[ GK_LOOKDOWN ].keyUp(); break;
			case SDLK_LEFT:
			case SDLK_a:	m_gkStates[ GK_LOOKLEFT ].keyUp(); break;
			case SDLK_RIGHT:
			case SDLK_d:	m_gkStates[ GK_LOOKRIGHT ].keyUp(); break;
			case SDLK_PAGEDOWN:
			case SDLK_e:	m_gkStates[ GK_ROTATECOUNTERCLOCKWISE ].keyUp(); break;
			case SDLK_q:	m_gkStates[ GK_ROTATECLOCKWISE ].keyUp(); break;
			case SDLK_i:	m_gkStates[ GK_MOVEFRONT ].keyUp(); break;
			case SDLK_u:	m_gkStates[ GK_MOVEFRONTINV ].keyUp(); break;
			case SDLK_p:	m_gkStates[ GK_MOVEBACK ].keyUp(); break;
			case SDLK_o:	m_gkStates[ GK_MOVEBACKINV ].keyUp(); break;
			case SDLK_k:	m_gkStates[ GK_MOVELEFT ].keyUp(); break;
			case SDLK_j:	m_gkStates[ GK_MOVELEFTINV ].keyUp();	break;
			case SDLK_SEMICOLON: m_gkStates[ GK_MOVERIGHT ].keyUp(); break;
			case SDLK_l:	m_gkStates[ GK_MOVERIGHTINV ].keyUp(); break;
			case SDLK_m:	m_gkStates[ GK_MOVEUP ].keyUp(); break;
			case SDLK_n:	m_gkStates[ GK_MOVEUPINV ].keyUp(); break;
			case SDLK_PERIOD: m_gkStates[ GK_MOVEDOWN ].keyUp(); break;
			case SDLK_COMMA: m_gkStates[ GK_MOVEDOWNINV ].keyUp(); break;
			case SDLK_RETURN: m_gkStates[ GK_CHANGEPROJ ].keyUp(); break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch( event.button.button )
			{
			case SDL_BUTTON_LEFT:
				m_pBegin = getGLPos( event.button.x, event.button.y );

				m_gkStates[ GK_MOVEMOUSE ].keyDown();
				break;
			case SDL_BUTTON_RIGHT:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
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
			switch( event.button.button )
			{
			case SDL_BUTTON_LEFT:

				m_gkStates[ GK_MOVEMOUSE ].keyUp();
				break;
			case SDL_BUTTON_RIGHT:
				break;
			}
			break;
		default:
			if ( lastEvent )
			{
	//			for ( int i = 0; i < GK_COUNT; ++i )
	//				m_gkStates[ i ].keyUp();

//				m_pBegin = { 0.0f, 0.0f, 0.0f };
//				m_pEnd = { 0.0f, 0.0f, 0.0f };

				lastEvent = false;
				allEventsRunOut = true;
			}

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
	if ( /*!m_RCube->isRotating() &&*/ !m_gkStates[ GK_MOVEMOUSE ].isHold() )
	{
        if ( m_gkStates[ GK_LOOKDOWN ].isNewDown() )
		{
			m_RCube->setRotates( 1, 0, 0, false );
			m_gkStates[ GK_LOOKDOWN ].processKey();
		}
		if ( m_gkStates[ GK_LOOKUP ].isNewDown() )
		{
			m_RCube->setRotates( 1, 0, 0, true );
			m_gkStates[ GK_LOOKUP ].processKey();
		}
		if ( m_gkStates[ GK_LOOKRIGHT ].isNewDown() )
		{
			m_RCube->setRotates( 0, 1, 0, false );
			m_gkStates[ GK_LOOKRIGHT ].processKey();
		}
		if ( m_gkStates[ GK_LOOKLEFT ].isNewDown() )
		{
			m_RCube->setRotates( 0, 1, 0, true );
			m_gkStates[ GK_LOOKLEFT ].processKey();
		}
		if ( m_gkStates[ GK_ROTATECOUNTERCLOCKWISE ].isNewDown() )
		{
			m_RCube->setRotates( 0, 0, 1, true );
			m_gkStates[ GK_ROTATECOUNTERCLOCKWISE ].processKey();
		}
		if ( m_gkStates[ GK_ROTATECLOCKWISE ].isNewDown() )
		{
			m_RCube->setRotates( 0, 0, 1, false );
			m_gkStates[ GK_ROTATECLOCKWISE ].processKey();
		}
	}

	if ( m_gkStates[ GK_MOVEMOUSE ].isNewDown() && !m_RCube->isMoving() && !m_RCube->isRotating() )
	{
		if ( glm::distance( m_pBegin, m_pEnd ) > 0.5 && glm::length( m_pBegin ) > 0 && glm::length( m_pEnd ) > 0 )
		{
//			std::cout << m_pBegin.x << " " << m_pBegin.y << " " << m_pBegin.z << " " << std::endl;
//			std::cout << m_pEnd.x << " " << m_pEnd.y << " " << m_pEnd.z << " " << std::endl;
			m_RCube->setMoveByCoords( m_pBegin, m_pEnd );
			m_gkStates[ GK_MOVEMOUSE ].processKey();

			m_pBegin = { 0.0f, 0.0f, 0.0f };
			m_pEnd = { 0.0f, 0.0f, 0.0f };
		}
		else if ( !m_gkStates[ GK_MOVEMOUSE ].isHold() )
		{
			m_gkStates[ GK_MOVEMOUSE ].processKey();
			m_pBegin = { 0.0f, 0.0f, 0.0f };
			m_pEnd = { 0.0f, 0.0f, 0.0f };
		}
	}

	if ( !m_RCube->isMoving() && !m_RCube->isRotating() && !m_gkStates[ GK_MOVEMOUSE ].isHold() )
	{
		for ( int i = 0; i < GK_MOVELAST - GK_MOVEFIRST + 1; i++ )
		{
			if ( m_gkStates[ GK_MOVEFRONT + i ].isNewDown() )
			{
				m_RCube->setMove( ( RCMoveType ) ( MT_FRONT + i ) );
				m_gkStates[ GK_MOVEFRONT + i ].processKey();
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
	if ( m_gkStates[ GK_CHANGEPROJ ].isNewDown() && !m_gkStates[ GK_MOVEMOUSE ].isHold() )
	{
        m_prType = ProjectionType ( ( m_prType + 1 ) % PT_COUNT );
        setProjection( m_prType );
		m_gkStates[ GK_CHANGEPROJ ].processKey();
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

