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
	gluPerspective( 40.0f, ( float ) SCREEN_WIDTH / ( float ) SCREEN_HEIGHT, 0.1f, 20.0f );

	glMatrixMode( GL_MODELVIEW );
}

void CPlayState::Cleanup()
{
//	SDL_FreeSurface(bg);

	printf("CPlayState Cleanup\n");
}

void CPlayState::Pause()
{
	printf("CPlayState Pause\n");
}

void CPlayState::Resume()
{
	printf("CPlayState Resume\n");
}

void CPlayState::HandleEvents(CGameEngine* game)
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

void CPlayState::Update(CGameEngine* game)
{

}

void CPlayState::Draw(CGameEngine* game)
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	static float cubeXSmoothAngle = 0;
	static float cubeYSmoothAngle = 0;
	static float cubeZSmoothAngle = 0;

	const float angleDiff = 8;

	static MyQuaternion quatCur;

	glLoadIdentity();

	glTranslatef( 0.0f, 0.0f, -7.0 );

//	glRotatef( 15, 1.0f, 1.0f, 0.0f );
//	drawAxis();

	MyQuaternion quatTempX;
	MyQuaternion quatTempY;
	MyQuaternion quatTempZ;

	if ( ( moveDirX != MD_NONE ) && ( ( ( moveDirY == MD_NONE ) && ( moveDirZ == MD_NONE ) ) || ( cubeXSmoothAngle > 0 ) ) )
	{
		if ( cubeXSmoothAngle >= 90 - angleDiff )
		{
			quatTempX.fromAxisAngle( 1.0, 0.0, 0.0, ( moveDirX == MD_POSITIVE ) ? -( 90 - cubeXSmoothAngle ) : ( 90 - cubeXSmoothAngle ) );
			quatCur = quatCur * quatTempX;

			cubeXSmoothAngle = 0;
			moveDirX = MD_NONE;
		}
		else
		{
			quatTempX.fromAxisAngle( 1.0, 0.0, 0.0, ( moveDirX == MD_POSITIVE ) ? -angleDiff : angleDiff );
			quatCur = quatCur * quatTempX;

			cubeXSmoothAngle += angleDiff;
		}

#ifdef MY_DEBUG
		GLfloat Matrix[16];
		quatCur.getTrMatrix( Matrix );
		writeMatrix( Matrix, 16 );
#endif
	}
	else if ( ( moveDirY != MD_NONE ) && ( ( moveDirZ == MD_NONE ) || ( cubeYSmoothAngle > 0 ) ) )
	{
		if ( cubeYSmoothAngle >= 90 - angleDiff )
		{
			quatTempY.fromAxisAngle( 0.0, 1.0, 0.0, ( moveDirY == MD_POSITIVE ) ? -( 90 - cubeYSmoothAngle ) : ( 90 - cubeYSmoothAngle ) );
			quatCur = quatCur * quatTempY;

			cubeYSmoothAngle = 0;
			moveDirY = MD_NONE;
		}
		else
		{
			quatTempY.fromAxisAngle( 0.0, 1.0, 0.0, ( moveDirY == MD_POSITIVE ) ? -angleDiff : angleDiff );
			quatCur = quatCur * quatTempY;

			cubeYSmoothAngle += angleDiff;
		}
#ifdef MY_DEBUG
		GLfloat Matrix[16];
		quatCur.getTrMatrix( Matrix );
		writeMatrix( Matrix, 16 );
#endif
	}
	else if ( moveDirZ != MD_NONE )
	{
		if ( cubeZSmoothAngle >= 90 - angleDiff )
		{
			quatTempZ.fromAxisAngle( 0.0, 0.0, 1.0, ( moveDirZ == MD_POSITIVE ) ? -( 90 - cubeZSmoothAngle ) : ( 90 - cubeZSmoothAngle ) );
			quatCur = quatCur * quatTempZ;

			cubeZSmoothAngle = 0;
			moveDirZ = MD_NONE;
		}
		else
		{
			quatTempZ.fromAxisAngle( 0.0, 0.0, 1.0, ( moveDirZ == MD_POSITIVE ) ? -angleDiff : angleDiff );
			quatCur = quatCur * quatTempZ;

			cubeZSmoothAngle += angleDiff;
		}
#ifdef MY_DEBUG
		GLfloat Matrix[16];
		quatCur.getTrMatrix( Matrix );
		writeMatrix( Matrix, 16 );
#endif
	}

	GLfloat MatrixRes[16];
	quatCur.getTrMatrix( MatrixRes );
	glMultMatrixf( MatrixRes );

	m_RCube.drawCube();

	glFlush();

//	SDL_BlitSurface(bg, NULL, game->screen, NULL);
//	SDL_UpdateRect(game->screen, 0, 0, 0, 0);
}

