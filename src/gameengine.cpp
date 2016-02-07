#include "all.h"

#include "gameengine.h"
#include "gamestate.h"

void CGameEngine::Init( const std::string & title )
{
	// init SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return;
	}

	// create window
	m_window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL );

	if( m_window == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return;
	}

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	if ( SDL_GL_CreateContext( m_window ) == NULL )
	{
		std::cout << "Error: SDL_GL_CreateContext: " << SDL_GetError() << std::endl;
		return;
	}

	// init GLEW
	GLenum glew_status = glewInit();
	if ( glew_status != GLEW_OK )
	{
		std::cout << "Error: glewInit: " << glewGetErrorString( glew_status ) << std::endl;
		return;
	}

	if ( !GLEW_VERSION_2_0 )
	{
		/*cerr*/ std::cout << "Error: your graphic card does not support OpenGL 2.0" << std::endl;
		return;
	}

//	std::cout.flush();

/*	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );



	SDL_GLContext glcontext = SDL_GL_CreateContext( gWindow );
	if ( glcontext == NULL )
	{
		printf( "SDL Error: %s\n", SDL_GetError() );
		return;
	}
*/
	m_running = true;
}

void CGameEngine::Cleanup()
{
	while ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}
#ifdef MY_DEBUG
	printf( "CGameEngine Cleanup\n" );
#endif

	SDL_DestroyWindow( m_window );
	m_window = NULL;

	SDL_Quit();
}

void CGameEngine::ChangeState( CGameState * state )
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back( state );
	states.back()->Init();
}

void CGameEngine::PushState( CGameState * state )
{
	// pause current state
	if ( !states.empty() )
	{
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back( state );
	states.back()->Init();
}

void CGameEngine::PopState()
{
	// cleanup the current state
	if ( !states.empty() )
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() )
	{
		states.back()->Resume();
	}
}

void CGameEngine::HandleEvents()
{
	states.back()->HandleEvents( this );
}

void CGameEngine::Update()
{
	states.back()->Update( this );
}

void CGameEngine::Draw()
{
	states.back()->Draw( this );

	SDL_GL_SwapWindow( m_window );
}
