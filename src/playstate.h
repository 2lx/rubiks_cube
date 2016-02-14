#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "gamestate.h"
#include <glm/gtc/type_ptr.hpp>
#include "keystate.h"

class RCubeObject;

class PlayState : public GameState
{
public:
    void init() override;
    void cleanup() override;

    void pause() override;
    void resume() override;

    void handleEvents( GameEngine * game ) override;
    void update( GameEngine * game ) override;
    void draw( GameEngine * game ) override;

    static PlayState * instance() { return &m_PlayState; }

protected:
    PlayState() { }

private:
    enum ProjectionType
    {
        PT_DIMETRIC, PT_ISOMETRIC,
        PT_COUNT
    };

    static PlayState m_PlayState;
    RCubeObject * m_RCube;
    bool m_needRedraw = { true };
    bool m_trBG = { true };
    KeyQueue m_keyQ;
    ProjectionType m_prType = { PT_ISOMETRIC };
    glm::vec3 m_pMBegin, m_pMEnd;
    glm::vec3 m_pRBegin, m_pREnd;
    glm::mat4 m_matrCamera;
    glm::dmat4 m_mProjection, m_mModel, m_mView;
    int m_screenWidth = 800, m_screenHeight = 600;

    void drawBackground();
    void setProjection( const ProjectionType pType );
    glm::vec3 getGLPos( const int mX, const int mY ) const;

    ShaderProgram * m_shaderPr;

    GLuint m_VBOScreenVertices;
    GLuint m_attrScreenVertices;
    GLint m_UniIsBG;
    GLint m_UniPlasmaSpeed;

    const GLfloat m_aScreenVertices[ 2 * 4 ] = {
        -1.0, -1.0,
         1.0, -1.0,
         1.0,  1.0,
        -1.0,  1.0
    };
};

#endif
