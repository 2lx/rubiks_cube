#include "all.h"

#include "gameengine.h"
#include "gamestate.h"

void CGameEngine::init( const std::string & title )
{
	// init SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		throw std::runtime_error( "CGameEngine::Init(): Error initializing SDL. " + std::string( SDL_GetError() ) );

	// create window
	m_window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL );

	if( m_window == NULL )
		throw std::runtime_error( "CGameEngine::Init(): Error creating SDL window. " + std::string( SDL_GetError() ) );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	if ( SDL_GL_CreateContext( m_window ) == NULL )
		throw std::runtime_error( "CGameEngine::Init(): Error creating SDL context. " + std::string( SDL_GetError() ) );

	// init GLEW
	GLenum glew_status = glewInit();
	if ( glew_status != GLEW_OK )
		throw std::runtime_error( "CGameEngine::Init(): Error creating SDL window" );
//				glewGetErrorString( glew_status ) ) );

	if ( !GLEW_VERSION_2_0 )
		throw std::runtime_error( "CGameEngine::Init(): your graphic card does not support OpenGL 2.0" );

	m_running = true;
}

void CGameEngine::cleanup()
{
	while ( !states.empty() ) {
		states.back()->cleanup();
		states.pop_back();
	}
#ifdef MY_DEBUG
	printf( "CGameEngine Cleanup\n" );
#endif

	SDL_DestroyWindow( m_window );
	m_window = NULL;

	SDL_Quit();
}

void CGameEngine::changeState( GameState * state )
{
	// cleanup the current state
	if ( !states.empty() )
	{
		states.back()->cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back( state );
	states.back()->init();
}

void CGameEngine::pushState( GameState * state )
{
	// pause current state
	if ( !states.empty() )
	{
		states.back()->pause();
	}

	// store and init the new state
	states.push_back( state );
	states.back()->init();
}

void CGameEngine::popState()
{
	// cleanup the current state
	if ( !states.empty() )
	{
		states.back()->cleanup();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() )
	{
		states.back()->resume();
	}
}

void CGameEngine::handleEvents()
{
	states.back()->handleEvents( this );
}

void CGameEngine::update()
{
	states.back()->update( this );
}

void CGameEngine::draw()
{
	states.back()->draw( this );

	SDL_GL_SwapWindow( m_window );
}
