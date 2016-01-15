#include "all.h"
#include <iostream>
//#include "glm/gtc/quaternion.hpp"
#include "myquaternion.h"

SDL_Window * gWindow = NULL;
SDL_Surface * screen;
bool isReversePerspective = false;

enum MoveDirection
{
	MD_NONE, MD_POSITIVE, MD_NEGATIVE
};

MoveDirection moveDirX = MD_NONE;
MoveDirection moveDirY = MD_NONE;
MoveDirection moveDirZ = MD_NONE;

struct MyCube
{
	uint colInd;
};

MyCube cubes[ CUBE_COUNT ][ CUBE_COUNT ][ CUBE_COUNT ];

void setPerspective( const bool newPerspective )
{
	isReversePerspective = newPerspective;

	glClearColor( COLOR_LIGHTGRAY[ 0 ], COLOR_LIGHTGRAY[ 1 ], COLOR_LIGHTGRAY[ 2 ], 0.0f );
	glClearDepth( 1.0 );
	glDepthFunc( GL_LESS );
	glEnable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );
	glMatrixMode( GL_PROJECTION );
	//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLoadIdentity();
	if ( !isReversePerspective )
		gluPerspective( 40.0f, ( float ) SCREEN_WIDTH / ( float ) SCREEN_HEIGHT, 0.1f, 100.0f );
	else gluPerspective( 10.0f, ( float ) SCREEN_WIDTH / ( float ) SCREEN_HEIGHT, 100.0f, 0.1f );
	glMatrixMode( GL_MODELVIEW );
}

bool init()
{
	// Init SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

	gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
								SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	if( gWindow == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	SDL_GLContext glcontext = SDL_GL_CreateContext( gWindow );
	if ( glcontext = NULL )
	{
		printf( "SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	// Init OpenGL
	setPerspective( false );

	return true;
}

void close( )
{
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	SDL_Quit();
}

void drawAxis()
{
	glColor3f( 1.0f, 1.0f, 1.0f );

	glBegin( GL_LINES );
	glVertex3f( 0, 0, 0 );
	glVertex3f( 5, 0, 0 );

	glVertex3f( 0, 0, 0 );
	glVertex3f( 0, 5, 0 );

	glVertex3f( 0, 0, 0 );
	glVertex3f( 0, 0, 5 );
	glEnd();
}

void drawCube( const float pX, const float pY, const float pZ, const float cubeSize, const int colInd );

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

void display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	static float cubeXMoveAngle = 0;
	static float cubeYMoveAngle = 0;
	static float cubeZMoveAngle = 0;

	static float cubeEdge = 1.0;
	static int cubeSizeSign = -1;
	const float angleDiff = 12;

	static MyQuaternion quatCur;

	glLoadIdentity();

//	if ( !isReversePerspective )
		glTranslatef( 0.0f, 0.0f, -7.0f );
//	else glTranslatef( 0.0f, 0.0f, -22.0f );

//	glRotatef( 45, 1.0f, 1.0f, 1.0f );
//	drawAxis();

	MyQuaternion quatTempX;
	MyQuaternion quatTempY;
	MyQuaternion quatTempZ;

	if ( moveDirX != MD_NONE )
	{
		if ( cubeXMoveAngle >= 90 )
		{
			quatTempX.fromAxisAngle( 1.0, 0.0, 0.0, ( moveDirX == MD_POSITIVE ) ? -( 90 - cubeXMoveAngle ) : ( 90 - cubeXMoveAngle ) );
			quatCur = quatCur * quatTempX;

			cubeXMoveAngle = 0;
			moveDirX = MD_NONE;
		}
		else
		{
			quatTempX.fromAxisAngle( 1.0, 0.0, 0.0, ( moveDirX == MD_POSITIVE ) ? -angleDiff : angleDiff );
			quatCur = quatCur * quatTempX;

			cubeXMoveAngle += angleDiff;
		}

#ifdef MY_DEBUG
		GLfloat Matrix[16];
		quatCur.getTrMatrix( Matrix );
		writeMatrix( Matrix, 16 );
#endif
	}

	if ( moveDirY != MD_NONE )
	{
		if ( cubeYMoveAngle >= 90 )
		{
			quatTempY.fromAxisAngle( 0.0, 1.0, 0.0, ( moveDirY == MD_POSITIVE ) ? -( 90 - cubeYMoveAngle ) : ( 90 - cubeYMoveAngle ) );
			quatCur = quatCur * quatTempY;

			cubeYMoveAngle = 0;
			moveDirY = MD_NONE;
		}
		else
		{
			quatTempY.fromAxisAngle( 0.0, 1.0, 0.0, ( moveDirY == MD_POSITIVE ) ? -angleDiff : angleDiff );
			quatCur = quatCur * quatTempY;

			cubeYMoveAngle += angleDiff;
		}
#ifdef MY_DEBUG
		GLfloat Matrix[16];
		quatCur.getTrMatrix( Matrix );
		writeMatrix( Matrix, 16 );
#endif
	}

	if ( moveDirZ != MD_NONE )
	{
		if ( cubeZMoveAngle >= 90 )
		{
			quatTempZ.fromAxisAngle( 0.0, 0.0, 1.0, ( moveDirZ == MD_POSITIVE ) ? -( 90 - cubeZMoveAngle ) : ( 90 - cubeZMoveAngle ) );
			quatCur = quatCur * quatTempZ;

			cubeZMoveAngle = 0;
			moveDirZ = MD_NONE;
		}
		else
		{
			quatTempZ.fromAxisAngle( 0.0, 0.0, 1.0, ( moveDirZ == MD_POSITIVE ) ? -angleDiff : angleDiff );
			quatCur = quatCur * quatTempZ;

			cubeZMoveAngle += angleDiff;
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

//	cubeEdge = cubeEdge + cubeSizeSign * 0.01;
//	if ( cubeEdge > 1.3 ) cubeSizeSign = -1;
//	if ( cubeEdge < 0.7 ) cubeSizeSign = 1;
//	cubeEdge = cubeEdge + cubeSizeSign * 0.01;

	for ( int x = 0; x < CUBE_COUNT; ++x )
		for ( int y = 0; y < CUBE_COUNT; ++y )
			for ( int z = 0; z < CUBE_COUNT; ++z )
				drawCube( x - 1, y - 1, z - 1, ( cubeEdge > 1.0 ) ? 1.0 : cubeEdge,
						  cubes[ x ][ y ][ /*( isReversePerspective ) ? CUBE_COUNT - z - 1 :*/ z  ].colInd );
}

int main( int argc, char * args[] )
{
	srand( time( 0 ) );

	for( int x = 0; x < CUBE_COUNT; ++x )
		for( int y = 0; y < CUBE_COUNT; ++y )
			for( int z = 0; z < CUBE_COUNT; ++z )
				cubes[ x ][ y ][ z ].colInd = rand() % COLOR_COUNT;

	bool running = true;
	Uint32 start;
	SDL_Event event;

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
		exit( 1 );
	}

	while( running )
	{
		start = SDL_GetTicks();
		SDL_Event event;

		while ( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				switch( event.key.keysym.sym )
				{
				case SDLK_ESCAPE:
					running = false;
					break;

				case SDLK_UP:
				case SDLK_w:
					if ( moveDirX == MD_NONE )
					{
	//					setPerspective( false );
						moveDirX = MD_POSITIVE;
					}
					break;

				case SDLK_DOWN:
				case SDLK_s:
					if ( moveDirX == MD_NONE )
					{
	//					setPerspective( false );
						moveDirX = MD_NEGATIVE;
					}
					break;

				case SDLK_LEFT:
				case SDLK_a:
					if ( moveDirY == MD_NONE )
					{
	//					setPerspective( false );
						moveDirY = MD_POSITIVE;
					}
					break;

				case SDLK_RIGHT:
				case SDLK_d:
					if ( moveDirY == MD_NONE )
					{
	//					setPerspective( false );
						moveDirY = MD_NEGATIVE;
					}
					break;

				case SDLK_PAGEDOWN:
				case SDLK_e:
					if ( moveDirZ == MD_NONE )
					{
	//					setPerspective( false );
						moveDirZ = MD_NEGATIVE;
					}
					break;

				case SDLK_DELETE:
				case SDLK_q:
					if ( moveDirZ == MD_NONE )
					{
	//					setPerspective( false );
						moveDirZ = MD_POSITIVE;
					}
					break;

/*				case SDLK_SPACE:
					if ( !isMoveX )
						setPerspective( !isReversePerspective );
					break;
*/				}
				break;
			}
		}

		display();

		glFlush();
		SDL_GL_SwapWindow( gWindow );

		if ( SDL_GetTicks() - start < SCREEN_TICK_PER_FRAME )
			SDL_Delay( SCREEN_TICK_PER_FRAME - ( SDL_GetTicks() - start ) );
	}

	close();

	return 0;
}


void vertexCube( const float pX, const float pY, const float pZ, const float cubeSize )
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

void drawCube( const float pX, const float pY, const float pZ, const float cubeSize, const int colInd )
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
