#include "all.h"
#include <iostream>

SDL_Window * gWindow = NULL;
SDL_Surface * screen;

struct MyCube
{
	uint colInd;
};

MyCube cubes[ CUBE_EDGE ][ CUBE_EDGE ][ CUBE_EDGE ];

bool init()
{
	// Init SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) //SDL_INIT_EVERYTHING ??
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
	glClearColor( COLOR_LIGHTGRAY[ 0 ], COLOR_LIGHTGRAY[ 1 ], COLOR_LIGHTGRAY[ 2 ], 0.0f );
	glClearDepth( 1.0 );
	glDepthFunc( GL_LESS );
	glEnable( GL_DEPTH_TEST );
	glShadeModel( GL_SMOOTH );
	glMatrixMode( GL_PROJECTION );
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLoadIdentity();
	gluPerspective( 40.0f, ( float ) SCREEN_WIDTH / ( float ) SCREEN_HEIGHT, 0.1f, 100.0f );
	glMatrixMode( GL_MODELVIEW );

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

void display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	static float cubeAngle = 0;
	static float cubeSize = 0.7;
	static int cubeSizeSign = -1;
	const float angleDiff = 2;

	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -7.0f);

//	glRotatef( 45, 1.0f, 1.0f, 1.0f );

//	drawAxis();

//	glRotatef( cubeAngle, 1.0f, 1.0f, 0.0f );
//	cubeAngle = ( cubeAngle > 360 ) ? cubeAngle - 360 + angleDiff : cubeAngle + angleDiff;

	cubeSize = cubeSize + cubeSizeSign * 0.01;
	if ( cubeSize > 1.1 ) cubeSizeSign = -1;
	if ( cubeSize < 0.7 ) cubeSizeSign = 1;
	cubeSize = cubeSize + cubeSizeSign * 0.01;

	for ( int x = 0; x < CUBE_EDGE; ++x )
		for ( int y = 0; y < CUBE_EDGE; ++y )
			for ( int z = 0; z < CUBE_EDGE; ++z )
				drawCube( x - 1, y - 1, z - 1, ( cubeSize > 1.0 ) ? 1.0 : cubeSize, cubes[ x ][ y ][ z ].colInd );
}

int main( int argc, char* args[] )
{
	srand( time( 0 ) );

    for( int x = 0; x < CUBE_EDGE; ++x )
		for( int y = 0; y < CUBE_EDGE; ++y )
			for( int z = 0; z < CUBE_EDGE; ++z )
				cubes[ x ][ y ][ z ].colInd = rand() % MAX_COLORS;

    bool running = true;
    Uint32 start;
	SDL_Event event;

	bool isMove = false;

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
							if ( !isMove )
								isMove = true;
						break;
					}
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
	glBegin(GL_QUADS);

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
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE ); // Borders

		glColor3f( 0.1F, 0.1F, 0.1F );
		vertexCube( pX, pY, pZ, cubeSize );
	}

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // Fill

	glColor3f( COLOR_MATR[ colInd ][ 0 ], COLOR_MATR[ colInd ][ 1 ], COLOR_MATR[ colInd ][ 2 ] );
	vertexCube( pX, pY, pZ, cubeSize );
}
