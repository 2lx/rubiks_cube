#include "all.h"

#include "gameengine.h"
#include "playstate.h"

int main( int argc, char * args[] )
{
	CGameEngine game;

	game.Init( "Rubik's cube. Game_v0.1" );

	game.ChangeState( CPlayState::Instance() );

	while ( game.Running() )
	{
		game.HandleEvents();
//		game.Update();
		game.Draw();
	}

	game.Cleanup();

	return 0;
}


