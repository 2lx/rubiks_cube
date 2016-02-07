#include "all.h"

#include "gameengine.h"
#include "playstate.h"

int main( int argc, char * args[] )
{
	CGameEngine game;

	game.Init( "Rubik's cube. Game_v0.7" );

	game.ChangeState( CPlayState::Instance() );

#ifdef MY_DEBUG
	Uint32 startHE;
	Uint32 startDR;
	Uint32 endAll;
#endif // MY_DEBUG

	while ( game.Running() )
	{
#ifdef MY_DEBUG
		startHE = SDL_GetTicks();
#endif // MY_DEBUG
		game.HandleEvents();

		game.Update();

#ifdef MY_DEBUG
		startDR = SDL_GetTicks();
#endif // MY_DEBUG
		game.Draw();

#ifdef MY_DEBUG
		endAll = SDL_GetTicks();

		if ( endAll - startDR > SCREEN_TICK_PER_FRAME + 1 )
		std::cout << "Handles: " << startDR - startHE << "    Draw: " << endAll - startDR << std::endl;
#endif // MY_DEBUG
		std::cout.flush();
	}

	game.Cleanup();

	return EXIT_SUCCESS;
}


