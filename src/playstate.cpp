#include "all.h"

#include "gamestate.h"
#include "playstate.h"
#include "gameengine.h"
#include "myquaternion.h"
#include "rubikscube.h"

CPlayState CPlayState::m_PlayState;

void CPlayState::Init()
{
	glClearDepth( 1.0 );
	glDepthFunc( GL_LESS );
	glEnable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

//	gluPerspective( 40.0f, ( float ) SCREEN_WIDTH / ( float ) SCREEN_HEIGHT, 0.1f, 20.0f );

//	const float margin = 0.1;
//	glFrustum( -margin * wdh * 1.2, margin* wdh, -margin, margin* 1.2, 0.1, 20.0 );

	const float wdh = ( float ) SCREEN_WIDTH / ( float ) SCREEN_HEIGHT;
	const float margin = 3;
	GLfloat cavalierPMatrix[ 16 ] = {
		1 , 0 , 0 , 0,
		0 , 1 , 0 , 0,
		0.3345, -0.3345, 1 , 0,
		0 , 0 , 0 , 1
	};

	glOrtho( -margin * wdh, margin * wdh, -margin, margin, 0.0, 20.0 );
	glMultMatrixf( cavalierPMatrix );

	glMatrixMode( GL_MODELVIEW );

	m_RCube = new RubiksCube;
}

void CPlayState::Cleanup()
{
//	SDL_FreeSurface(bg);
	delete m_RCube;
#ifdef MY_DEBUG
	printf("CPlayState Cleanup\n");
#endif
}

void CPlayState::Pause()
{
#ifdef MY_DEBUG
	printf("CPlayState Pause\n");
#endif
}

void CPlayState::Resume()
{
#ifdef MY_DEBUG
	printf("CPlayState Resume\n");
#endif
}

void CPlayState::HandleEvents( CGameEngine* game )
{
	static bool lastEvent = false;
	SDL_Event event;

	const Uint32 start = SDL_GetTicks();
	bool breakWhile = false;

	while ( SDL_PollEvent( &event ) && !breakWhile && ( SDL_GetTicks() - start ) < 15 )
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
				m_gkStates[ GK_MOVEUP ].setDown();
				break;

			case SDLK_DOWN:
			case SDLK_s:
				m_gkStates[ GK_MOVEDOWN ].setDown();
				break;

			case SDLK_LEFT:
			case SDLK_a:
				m_gkStates[ GK_MOVELEFT ].setDown();
				break;

			case SDLK_RIGHT:
			case SDLK_d:
				m_gkStates[ GK_MOVERIGHT ].setDown();
				break;

			case SDLK_PAGEDOWN:
			case SDLK_e:
				m_gkStates[ GK_MOVERIGHTSIDE ].setDown();
				break;

			case SDLK_DELETE:
			case SDLK_q:
				m_gkStates[ GK_MOVELEFTSIDE ].setDown();
				break;

			}
			break;
		default:
			if ( lastEvent )
			{
				for ( int i = 0; i < GK_COUNT; ++i )
					m_gkStates[ i ].releasePress();

				lastEvent = false;
				breakWhile = true;
			}

			break;
		}
	}
}

void CPlayState::Update( CGameEngine * game )
{
	if ( !m_RCube->isMoved() )
	{
        if ( m_gkStates[ GK_MOVEUP ].isNewDown() )
		{
			m_RCube->setMoves( MD_POSITIVE, MD_NONE, MD_NONE );
			m_gkStates[ GK_MOVEUP ].releaseNewDown();
		}

        if ( m_gkStates[ GK_MOVEDOWN ].isNewDown() )
		{
			m_RCube->setMoves( MD_NEGATIVE, MD_NONE, MD_NONE );
			m_gkStates[ GK_MOVEDOWN ].releaseNewDown();
		}

        if ( m_gkStates[ GK_MOVELEFT ].isNewDown() )
		{
			m_RCube->setMoves( MD_NONE, MD_POSITIVE, MD_NONE );
			m_gkStates[ GK_MOVELEFT ].releaseNewDown();
		}

        if ( m_gkStates[ GK_MOVERIGHT ].isNewDown() )
		{
			m_RCube->setMoves( MD_NONE, MD_NEGATIVE, MD_NONE );
			m_gkStates[ GK_MOVERIGHT ].releaseNewDown();
		}

        if ( m_gkStates[ GK_MOVELEFTSIDE ].isNewDown() )
		{
			m_RCube->setMoves( MD_NONE, MD_NONE, MD_POSITIVE );
			m_gkStates[ GK_MOVELEFTSIDE ].releaseNewDown();
		}

        if ( m_gkStates[ GK_MOVERIGHTSIDE ].isNewDown() )
		{
			m_RCube->setMoves( MD_NONE, MD_NONE, MD_NEGATIVE );
			m_gkStates[ GK_MOVERIGHTSIDE ].releaseNewDown();
		}
	}
}

void CPlayState::Draw( CGameEngine * game )
{
#ifdef MY_DEBUG
	static int drCount = 0;
#endif // MY_DEBUG

	if ( m_RCube->isMoved() || !m_firstDraw )
	{
		Uint32 start = SDL_GetTicks();

		glClearColor( COLOR_LIGHTGRAY[ 0 ], COLOR_LIGHTGRAY[ 1 ], COLOR_LIGHTGRAY[ 2 ], 0.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		const float angleDiff = 8;

		glLoadIdentity();
//		glTranslatef( 0.0f, 0.0f, -7.0 );
		glTranslatef( 2.7f, -2.7f, -8.0 );

//		glRotatef( 15, 1.0f, 0.0f, 0.0f );

		m_RCube->moveObject();
		m_RCube->drawObject();

		glFlush();

		if ( !m_firstDraw ) m_firstDraw = true;

		if ( SDL_GetTicks() - start < SCREEN_TICK_PER_FRAME )
			SDL_Delay( SCREEN_TICK_PER_FRAME - ( SDL_GetTicks() - start ) );

#ifdef MY_DEBUG
		drCount++;
		if ( drCount % 5 == 0 )
			std::cout << "DrawCount: " << drCount << std::endl;
#endif // MY_DEBUG
	}
}

