#include "all.h"

#include "gameengine.h"
#include "gamestate.h"

void CGameEngine::Init(const char* title, int width, int height,
						 int bpp, bool fullscreen)
{
	// Init SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return;
	}

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

	gWindow = SDL_CreateWindow( "Rubik's cube game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
								SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	if( gWindow == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return;
	}

	SDL_GLContext glcontext = SDL_GL_CreateContext( gWindow );
	if ( glcontext = NULL )
	{
		printf( "SDL Error: %s\n", SDL_GetError() );
		return;
	}

//	m_fullscreen = fullscreen;
	m_running = true;

	printf("CGameEngine Init\n");
}

void CGameEngine::Cleanup()
{
	// cleanup the all states
	while ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// switch back to windowed mode so other
	// programs won't get accidentally resized
/*	if ( m_fullscreen ) {
		screen = SDL_SetVideoMode(640, 480, 0, 0);
	}
*/
	printf("CGameEngine Cleanup\n");

	// shutdown SDL
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	SDL_Quit();
}

void CGameEngine::ChangeState(CGameState* state)
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void CGameEngine::PushState(CGameState* state)
{
	// pause current state
	if ( !states.empty() ) {
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void CGameEngine::PopState()
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() ) {
		states.back()->Resume();
	}
}

void CGameEngine::HandleEvents()
{
	states.back()->HandleEvents(this);
}

void CGameEngine::Update()
{
	states.back()->Update(this);
}

void CGameEngine::Draw()
{
	Uint32 start;
	start = SDL_GetTicks();

	states.back()->Draw(this);
	SDL_GL_SwapWindow( gWindow );

	if ( SDL_GetTicks() - start < SCREEN_TICK_PER_FRAME )
		SDL_Delay( SCREEN_TICK_PER_FRAME - ( SDL_GetTicks() - start ) );
}
