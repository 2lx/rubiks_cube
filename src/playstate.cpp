#include "all.h"

#include "gamestate.h"
#include "playstate.h"
#include "gameengine.h"
#include "myquaternion.h"
#include "rubikscube.h"

CPlayState CPlayState::m_PlayState;

void CPlayState::Init()
{
	glClearColor( COLOR_LIGHTGRAY[ 0 ], COLOR_LIGHTGRAY[ 1 ], COLOR_LIGHTGRAY[ 2 ], 0.0f );
	glClearDepth( 1.0 );
	glDepthFunc( GL_LESS );
	glEnable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );
	glMatrixMode( GL_PROJECTION );
	//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLoadIdentity();

//	gluPerspective( 40.0f, ( float ) SCREEN_WIDTH / ( float ) SCREEN_HEIGHT, 0.1f, 20.0f );

//	const float margin = 0.1;
//	glFrustum( -margin * wdh * 1.2, margin* wdh, -margin, margin* 1.2, 0.1, 20.0 );

	const float wdh = ( float ) SCREEN_WIDTH / ( float ) SCREEN_HEIGHT;
	const float margin = 3;
	GLfloat cavalierPMatrix[ 16 ] = {
		1 , 0 , 0 , 0,
		0 , 1 , 0 , 0,
		0.355, -0.355, 1 , 0,
		0 , 0 , 0 , 1
	};

	glOrtho( -margin * wdh, margin * wdh, -margin, margin, 0.0, 20.0 );
	glMultMatrixf( cavalierPMatrix );

	glMatrixMode( GL_MODELVIEW );
}

void CPlayState::Cleanup()
{
//	SDL_FreeSurface(bg);
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
	SDL_Event event;

	if ( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
			case SDL_QUIT:
				game->Quit();
				break;

			case SDL_KEYDOWN:
				switch( event.key.keysym.sym )
				{
				case SDLK_ESCAPE:
					game->Quit();
					break;

				case SDLK_UP:
				case SDLK_w:
					if ( moveDirX == MD_NONE )
					{
						moveDirX = MD_POSITIVE;
					}
					break;

				case SDLK_DOWN:
				case SDLK_s:
					if ( moveDirX == MD_NONE )
					{
						moveDirX = MD_NEGATIVE;
					}
					break;

				case SDLK_LEFT:
				case SDLK_a:
					if ( moveDirY == MD_NONE )
					{
						moveDirY = MD_POSITIVE;
					}
					break;

				case SDLK_RIGHT:
				case SDLK_d:
					if ( moveDirY == MD_NONE )
					{
						moveDirY = MD_NEGATIVE;
					}
					break;

				case SDLK_PAGEDOWN:
				case SDLK_e:
					if ( moveDirZ == MD_NONE )
					{
						moveDirZ = MD_NEGATIVE;
					}
					break;

				case SDLK_DELETE:
				case SDLK_q:
					if ( moveDirZ == MD_NONE )
					{
						moveDirZ = MD_POSITIVE;
					}
					break;

				}
				break;
			}
		}
}

void CPlayState::Update( CGameEngine * game )
{

}

void CPlayState::Draw( CGameEngine * game )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	const float angleDiff = 8;

	glLoadIdentity();
//	glTranslatef( 0.0f, 0.0f, -7.0 );
	glTranslatef( 2.0f, -2.0f, -5.0 );

//	glRotatef( 15, 1.0f, 0.0f, 0.0f );

	m_RCube.rotateObject( moveDirX, moveDirY, moveDirZ );
	moveDirX = MD_NONE;
	moveDirY = MD_NONE;
	moveDirZ = MD_NONE;

	m_RCube.drawObject();

	glFlush();
}

