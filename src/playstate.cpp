#include "all.h"

#include "gamestate.h"
#include "playstate.h"
#include "gameengine.h"
#include "myquaternion.h"

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

	srand( time( 0 ) );

	for( int x = 0; x < CUBE_COUNT; ++x )
		for( int y = 0; y < CUBE_COUNT; ++y )
			for( int z = 0; z < CUBE_COUNT; ++z )
				cubes[ x ][ y ][ z ].colInd = rand() % COLOR_COUNT;
}

void CPlayState::Cleanup()
{
	SDL_FreeSurface(bg);

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

#ifdef MY_DEBUG
void writeMatrix( GLfloat * Matrix, const int length )
{
	for ( int i = 0; i < length; i++ )
	{
		if ( Matrix[ i ] >= 0 )
			std::cout << " ";

		std::cout << round( Matrix[ i ] * 1000000 ) / 1000000 << " ";

		if ( ( i % 4 ) == 3 )
			std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout.flush();
}
#endif

void CPlayState::vertexCube( const float pX, const float pY, const float pZ, const float cubeSize )
{
	glBegin( GL_QUADS );

	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );  // Up
	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );

	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );	// Front
	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );

	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );	// Bottom
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );

	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );	// Back
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );

	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );	// Left
	glVertex3f( pX - cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );
	glVertex3f( pX - cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );

	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ - cubeSize / 2 );	// Right
	glVertex3f( pX + cubeSize / 2, pY + cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ + cubeSize / 2 );
	glVertex3f( pX + cubeSize / 2, pY - cubeSize / 2, pZ - cubeSize / 2 );

	glEnd();
}

void CPlayState::drawCube( const float pX, const float pY, const float pZ, const float cubeSize, const int colInd )
{
	if ( cubeSize < 1.0 )
	{
		glLineWidth( 1.5 );
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // Borders

		glColor3f( 0.1F, 0.1F, 0.1F );
		vertexCube( pX, pY, pZ, cubeSize );
	}

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // Fill

	glColor3f( COLOR_MATR[ colInd ][ 0 ], COLOR_MATR[ colInd ][ 1 ], COLOR_MATR[ colInd ][ 2 ] );
	vertexCube( pX, pY, pZ, cubeSize );
}


void CPlayState::Draw(CGameEngine* game)
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	static float cubeXSmoothAngle = 0;
	static float cubeYSmoothAngle = 0;
	static float cubeZSmoothAngle = 0;

	static float cubeEdge = 1.0;
	static int cubeSizeSign = -1;
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

	cubeEdge = cubeEdge + cubeSizeSign * 0.01;
	if ( cubeEdge > 1.3 ) cubeSizeSign = -1;
	if ( cubeEdge < 0.7 ) cubeSizeSign = 1;
	cubeEdge = cubeEdge + cubeSizeSign * 0.01;

	for ( int x = 0; x < CUBE_COUNT; ++x )
		for ( int y = 0; y < CUBE_COUNT; ++y )
			for ( int z = 0; z < CUBE_COUNT; ++z )
				drawCube( x - 1, y - 1, z - 1, ( cubeEdge > 1.0 ) ? 1.0 : cubeEdge,
						  cubes[ x ][ y ][ /*( isReversePerspective ) ? CUBE_COUNT - z - 1 :*/ z  ].colInd );

	glFlush();

//	SDL_BlitSurface(bg, NULL, game->screen, NULL);
//	SDL_UpdateRect(game->screen, 0, 0, 0, 0);
}

