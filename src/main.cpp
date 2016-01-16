#include "all.h"

#include "gameengine.h"
#include "playstate.h"

int main( int argc, char * args[] )
{
	CGameEngine game;

	game.Init( "Rubik's cube. Game_v0.1" );

	game.ChangeState( CPlayState::Instance() );

	Uint32 startHE;
	Uint32 startDR;
	Uint32 endAll;

	while ( game.Running() )
	{
		startHE = SDL_GetTicks();
		game.HandleEvents();

		game.Update();

		startDR = SDL_GetTicks();
		game.Draw();

		endAll = SDL_GetTicks();
#ifdef MY_DEBUG
		if ( startDR - startHE > 0 )
			std::cout << "Handles: " << startDR - startHE << "    Draw: " << endAll - startDR << std::endl;
#endif // MY_DEBUG
	}

	game.Cleanup();

	return 0;
}


