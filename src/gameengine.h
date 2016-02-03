#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>

struct SDL_Surface;
struct SDL_Window;

class CGameState;
class ShaderProgram;

class CGameEngine
{
public:

	void Init( const std::string & title );
	void Cleanup();

	void ChangeState( CGameState * state );
	void PushState( CGameState * state );
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false; }

private:
	std::vector< CGameState * > states;
	bool m_running;

	SDL_Window * m_window;
};

#endif
