#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "SDL2/SDL.h"
#include "gamestate.h"
#include "rubikscube.h"
#include "shapes.h"

class CPlayState : public CGameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents( CGameEngine * game );
	void Update( CGameEngine * game );
	void Draw( CGameEngine * game );

	static CPlayState * Instance() { return &m_PlayState; }

protected:
	CPlayState() { }

private:
	static CPlayState m_PlayState;
	RubiksCube m_RCube;
//	SDL_Surface * bg;

	bool m_firstDraw = { false };
	MoveDirection moveDirX = { MD_NONE };
	MoveDirection moveDirY = { MD_NONE };
	MoveDirection moveDirZ = { MD_NONE };
};

#endif
