#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>

struct SDL_Surface;
struct SDL_Window;

class GameState;
class ShaderProgram;

class GameEngine
{
public:

	void init( const std::string & title );
	void cleanup();

	void changeState( GameState * state );
	void pushState( GameState * state );
	void popState();

	void handleEvents();
	void update();
	void draw();

	bool running() { return m_running; }
	void quit() { m_running = false; }

private:
	std::vector< GameState * > states;
	bool m_running;

	SDL_Window * m_window;
};

#endif
