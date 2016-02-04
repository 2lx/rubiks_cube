#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "gamestate.h"
#include <glm/gtc/type_ptr.hpp>
#include "keystate.h"

class RCubeObject;

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
	enum ProjectionType
	{
		PT_DIMETRIC, PT_ISOMETRIC,
		PT_COUNT
	};

	static CPlayState m_PlayState;
	RCubeObject * m_RCube;
	bool m_needRedraw = { true };
	KeyQueue m_keyQ;
	ProjectionType m_prType = { PT_ISOMETRIC };
	glm::vec3 m_pBegin, m_pEnd;
	glm::mat4 m_matrCamera;
	glm::dmat4 m_mProjection, m_mModel, m_mView;
	int m_screenWidth = 800, m_screenHeight = 600;

	void setProjection( const ProjectionType pType );
	glm::vec3 getGLPos( const int mX, const int mY ) const;

	ShaderProgram * m_shaderPr;
};

#endif
