#include "all.h"

#include "gameengine.h"
#include "playstate.h"

int main( int argc, char * args[] )
{
	CGameEngine game;

	try
	{
		game.Init( "Rubik's cube. Game_v0.7" );

		game.ChangeState( CPlayState::Instance() );
	}
	catch ( std::exception &e )
	{
		std::cerr << "Initialization: " << e.what() << std::endl;
		char ck;
		std::cin >> ck;
	}
	catch ( ... )
	{
		std::cerr << "Unknown exception" << std::endl;

		return EXIT_FAILURE;
	}

#ifdef MY_DEBUG
	Uint32 startHE;
	Uint32 startDR;
	Uint32 endAll;
#endif // MY_DEBUG

	while ( game.Running() )
	{
		try
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

		} // try
		catch( std::exception & e )
		{
            std::cerr << "Exception: " << e.what() << std::endl;
		}
		catch( ... )
		{
            std::cerr << "Unknown error " << std::endl;
            return EXIT_FAILURE;
		}

		std::cout.flush();
	}

	game.Cleanup();

	return EXIT_SUCCESS;
}


