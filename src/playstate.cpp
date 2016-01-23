#include "all.h"

#include "gamestate.h"
#include "playstate.h"
#include "gameengine.h"
#include "myquaternion.h"
#include "rcubeparams.h"

CPlayState CPlayState::m_PlayState;

GLfloat cavalierPMatrix[ 16 ] = {
	1 , 0 , 0 , 0,
	0 , 1 , 0 , 0,
	0.3345, -0.3345, 1 , 0,
	0 , 0 , 0 , 1
};

void CPlayState::Init()
{
	m_RCube = new RCubeObject;

	glClearDepth( 1.0 );
	glDepthFunc( GL_LESS );
	glEnable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

	setProjection( m_prType );
/*	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

//	gluPerspective( 40.0f, ( float ) SCREEN_WIDTH / ( float ) SCREEN_HEIGHT, 0.1f, 20.0f );

	glOrtho( -SCREEN_HORIZMARGIN, SCREEN_HORIZMARGIN, -SCREEN_VERTMARGIN, SCREEN_VERTMARGIN, 0.0, 10.0 );
	glMultMatrixf( cavalierPMatrix );

	glMatrixMode( GL_MODELVIEW );
*/
}

void CPlayState::Cleanup()
{
	delete m_RCube;

	MoveParams::cleanup();
	Colors::cleanup();
	AxisParams::cleanup();
}

void CPlayState::setProjection( const ProjectionType pType ) const
{
//	gluPerspective( 40.0f, ( float ) SCREEN_WIDTH / ( float ) SCREEN_HEIGHT, 0.1f, 20.0f );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	if ( pType == PT_DIMETRIC )
	{
		glOrtho( -SCREEN_HORIZMARGIN, SCREEN_HORIZMARGIN, -SCREEN_VERTMARGIN, SCREEN_VERTMARGIN, 0.0, 20.0 );
		glMultMatrixf( cavalierPMatrix );
	}
	else if ( pType == PT_ISOMETRIC )
	{
		glOrtho( -SCREEN_HORIZMARGIN, SCREEN_HORIZMARGIN, -SCREEN_VERTMARGIN, SCREEN_VERTMARGIN, 0.0, 20.0 );
	};

	glMatrixMode( GL_MODELVIEW );
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

		case SDL_KEYDOWN:
			lastEvent = true;
			switch( event.key.keysym.sym )
			{
			case SDLK_ESCAPE:
				game->Quit();
				break;

			case SDLK_UP:
			case SDLK_w:
				m_gkStates[ GK_LOOKUP ].setDown();
				break;

			case SDLK_DOWN:
			case SDLK_s:
				m_gkStates[ GK_LOOKDOWN ].setDown();
				break;

			case SDLK_LEFT:
			case SDLK_a:
				m_gkStates[ GK_LOOKLEFT ].setDown();
				break;

			case SDLK_RIGHT:
			case SDLK_d:
				m_gkStates[ GK_LOOKRIGHT ].setDown();
				break;

			case SDLK_PAGEDOWN:
			case SDLK_e:
				m_gkStates[ GK_ROTATECOUNTERCLOCKWISE ].setDown();
				break;

			case SDLK_DELETE:
			case SDLK_q:
				m_gkStates[ GK_ROTATECLOCKWISE ].setDown();
				break;

			case SDLK_i:
				m_gkStates[ GK_MOVEFRONT ].setDown();
				break;

			case SDLK_u:
				m_gkStates[ GK_MOVEFRONTINV ].setDown();
				break;

			case SDLK_p:
				m_gkStates[ GK_MOVEBACK ].setDown();
				break;

			case SDLK_o:
				m_gkStates[ GK_MOVEBACKINV ].setDown();
				break;

			case SDLK_k:
				m_gkStates[ GK_MOVELEFT ].setDown();
				break;

			case SDLK_j:
				m_gkStates[ GK_MOVELEFTINV ].setDown();
				break;

			case SDLK_SEMICOLON:
				m_gkStates[ GK_MOVERIGHT ].setDown();
				break;

			case SDLK_l:
				m_gkStates[ GK_MOVERIGHTINV ].setDown();
				break;

			case SDLK_m:
				m_gkStates[ GK_MOVEUP ].setDown();
				break;

			case SDLK_n:
				m_gkStates[ GK_MOVEUPINV ].setDown();
				break;

			case SDLK_PERIOD:
				m_gkStates[ GK_MOVEDOWN ].setDown();
				break;

			case SDLK_COMMA:
				m_gkStates[ GK_MOVEDOWNINV ].setDown();
				break;

			case SDLK_SPACE:
				m_gkStates[ GK_CHANGECOLOR ].setDown();
				break;

			case SDLK_RETURN:
				m_gkStates[ GK_CHANGEPROJ ].setDown();
				break;
			}
			break;
		case SDL_KEYUP:
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch( event.button.button )
			{
			case SDL_BUTTON_LEFT:
				std::cout << event.button.x << " " << event.button.y << std::endl;
				std::cout.flush();
				break;
			case SDL_BUTTON_RIGHT:
				break;
			}
			break;
		default:
			if ( lastEvent )
			{
				for ( int i = 0; i < GK_COUNT; ++i )
					m_gkStates[ i ].releasePress();

				lastEvent = false;
				allEventsRunOut = true;
			}

			break;
		}
	}
}

void CPlayState::Update( CGameEngine * game )
{
	if ( !m_RCube->isRotating() )
	{
        if ( m_gkStates[ GK_LOOKDOWN ].isNewDown() )
		{
			m_RCube->setRotates( 1, 0, 0, false );
			m_gkStates[ GK_LOOKDOWN ].releaseNewDown();
		}
		else if ( m_gkStates[ GK_LOOKUP ].isNewDown() )
		{
			m_RCube->setRotates( 1, 0, 0, true );
			m_gkStates[ GK_LOOKUP ].releaseNewDown();
		}
		else if ( m_gkStates[ GK_LOOKRIGHT ].isNewDown() )
		{
			m_RCube->setRotates( 0, 1, 0, false );
			m_gkStates[ GK_LOOKRIGHT ].releaseNewDown();
		}
		else if ( m_gkStates[ GK_LOOKLEFT ].isNewDown() )
		{
			m_RCube->setRotates( 0, 1, 0, true );
			m_gkStates[ GK_LOOKLEFT ].releaseNewDown();
		}
		else if ( m_gkStates[ GK_ROTATECOUNTERCLOCKWISE ].isNewDown() )
		{
			m_RCube->setRotates( 0, 0, 1, false );
			m_gkStates[ GK_ROTATECOUNTERCLOCKWISE ].releaseNewDown();
		}
		else if ( m_gkStates[ GK_ROTATECLOCKWISE ].isNewDown() )
		{
			m_RCube->setRotates( 0, 0, 1, true );
			m_gkStates[ GK_ROTATECLOCKWISE ].releaseNewDown();
		}
	}

	if ( !m_RCube->isMoving() && !m_RCube->isRotating() )
	{
		for ( int i = 0; i < GK_MOVELAST - GK_MOVEFIRST + 1; i++ )
		{
			if ( m_gkStates[ GK_MOVEFRONT + i ].isNewDown() )
			{
				m_RCube->setMove( ( RCMoveType ) ( MT_FRONT + i ) );
				m_gkStates[ GK_MOVEFRONT + i ].releaseNewDown();
				break;
			}
		}
	}

	if ( m_gkStates[ GK_CHANGECOLOR ].isNewDown() )
	{
		RC::Colors::incScheme();
		m_gkStates[ GK_CHANGECOLOR ].releaseNewDown();
		m_needRedraw = true;
	}

	if ( m_gkStates[ GK_CHANGEPROJ ].isNewDown() )
	{
        m_prType = ProjectionType ( ( m_prType + 1 ) % PT_COUNT);
        setProjection( m_prType );
		m_gkStates[ GK_CHANGEPROJ ].releaseNewDown();
		m_needRedraw = true;
	}

//	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &amp;worldX, &amp;worldY, &amp;worldZ);
}

void CPlayState::Draw( CGameEngine * game )
{
	static int drCount = 0;

	if ( m_needRedraw || m_RCube->isRotating() || m_RCube->isMoving() )
	{
		Uint32 start = SDL_GetTicks();

		glClearColor( Colors::colR( RC_BG ), Colors::colG( RC_BG ), Colors::colB( RC_BG ), 0.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glLoadIdentity();

		if ( m_prType == PT_DIMETRIC )
		{
			glTranslatef( 0.0f, 0.0, -2.0 );
		}
		else if ( m_prType == PT_ISOMETRIC )
		{
			glTranslatef( 0.0f, 0.0, -3.0 );

			glRotatef( 35.264f, 1.0f, 0.0f, 0.0f );
			glRotatef( 45.0f, 0.0f, 1.0f, 0.0f );
		}

		m_RCube->rotateObject();
		m_RCube->drawObject();

		glFlush();

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

