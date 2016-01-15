#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "SDL2/SDL.h"
#include "gamestate.h"

struct MyCube
{
	uint colInd;
};


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
	void vertexCube( const float pX, const float pY, const float pZ, const float cubeSize );
	void drawCube( const float pX, const float pY, const float pZ, const float cubeSize, const int colInd );

	static CPlayState m_PlayState;

	MoveDirection moveDirX = MD_NONE;
	MoveDirection moveDirY = MD_NONE;
	MoveDirection moveDirZ = MD_NONE;

	MyCube cubes[ CUBE_COUNT ][ CUBE_COUNT ][ CUBE_COUNT ];

	SDL_Surface * bg;
};

#endif
