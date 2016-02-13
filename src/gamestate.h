#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "gameengine.h"

class GameState
{
public:
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void handleEvents( CGameEngine * game ) = 0;
	virtual void update( CGameEngine * game ) = 0;
	virtual void draw( CGameEngine * game ) = 0;

	void ChangeState( CGameEngine * game, GameState * state )
	{
		game->changeState( state );
	}

protected:
	GameState() { }
};

#endif

