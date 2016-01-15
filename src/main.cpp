#include "all.h"

#include "gameengine.h"
#include "playstate.h"

int main( int argc, char * args[] )
{
	CGameEngine game;

	// initialize the engine
	game.Init( "Engine Test v1.0" );

	// load the intro
	game.ChangeState( CPlayState::Instance() );

	Uint32 start;
	SDL_Event event;

	while ( game.Running() )
	{
		start = SDL_GetTicks();

		game.HandleEvents();
		game.Update();
		game.Draw();

		if ( SDL_GetTicks() - start < SCREEN_TICK_PER_FRAME )
			SDL_Delay( SCREEN_TICK_PER_FRAME - ( SDL_GetTicks() - start ) );
	}

	game.Cleanup();

	return 0;
}


