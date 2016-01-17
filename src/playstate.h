#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "SDL2/SDL.h"
#include "gamestate.h"
#include "rubikscube.h"
#include "shapes.h"

enum GameKeys
{
    GK_MOVEUP, GK_MOVEDOWN, GK_MOVELEFT, GK_MOVERIGHT, GK_MOVELEFTSIDE, GK_MOVERIGHTSIDE,
    GK_COUNT
};

class GameKeysStates
{
public:
	inline void setDown() {
		if ( !m_isPressed )
		{
			m_isNewDown = true;
			m_isPressed = true;
		}
	};
	inline void releasePress() { if ( m_isPressed ) m_isPressed = false; };
	inline bool releaseNewDown() { if ( m_isNewDown ) m_isNewDown = false; };
	inline bool isNewDown() const { return m_isNewDown; };

private:
    bool m_isNewDown = { 0 };
    bool m_isPressed = { 0 };
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
	static CPlayState m_PlayState;
	RubiksCube * m_RCube;
//	SDL_Surface * bg;

	bool m_firstDraw = { false };
	GameKeysStates m_gkStates[ GK_COUNT ];
};

#endif
