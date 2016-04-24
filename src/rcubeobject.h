#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include "gameobject.h"
#include <glm/gtc/type_ptr.hpp>

namespace RC { class CubeModel; }
class ShaderProgram;

class RCubeObject : public GameObject
{
public:
    RCubeObject( ShaderProgram * shaderPr );
    virtual ~RCubeObject();

    void drawObject( const glm::mat4 & pmv ) override;
    void setTurn( const RC::TT rt );
    RC::TT setTurnByCoords( const glm::vec3 & pBeg, const glm::vec3 & pEnd );
    bool isTurning() const { return ( m_turnMix >= 0 ); }
    void update();
    void incCurScheme() { m_texCurScheme = ( m_texCurScheme + 1 ) % m_texCount; }
    void reset();

protected:
private:
    RC::CubeModel * m_RCModel;

    glm::quat m_turnQuat;
    glm::quat m_newTurnQuat;
    float m_turnMix = { -1 };

    RC::TT m_turnType = { RC::TT::None };
    int m_turnLayer = -1;

    GLuint m_VBOTexCoords, m_VBOTexIndex, m_VBOCubeVertices;
    GLint m_attrTexCoords, m_attrTexIndex, m_attrCubeVertices;

    GLuint m_VBOTexUnionID;
    GLint m_UniTexUnionID;

    GLint m_UniMVP, m_UniTexCount, m_UniTexCurScheme;
    int m_texCount;
    int m_texCurScheme;

    void drawCube( const glm::mat4 & pmv, const RC::RA ra = RC::RA::None );
    void drawCubie( const int x, const int y, const int z ) const;

    const GLfloat m_aTexCoords[ 2*4*6 ] = {
        0.0, 0.0,   1.0, 0.0,   1.0, 1.0,   0.0, 1.0,
        0.0, 0.0,   1.0, 0.0,   1.0, 1.0,   0.0, 1.0,
        0.0, 0.0,   1.0, 0.0,   1.0, 1.0,   0.0, 1.0,
        0.0, 0.0,   1.0, 0.0,   1.0, 1.0,   0.0, 1.0,
        0.0, 0.0,   1.0, 0.0,   1.0, 1.0,   0.0, 1.0,
        0.0, 0.0,   1.0, 0.0,   1.0, 1.0,   0.0, 1.0,
    };

    RC::CF m_aTexIndex[ 4*6 ] = {
        RC::CF::Front,  RC::CF::Front,  RC::CF::Front,  RC::CF::Front,
        RC::CF::Up,     RC::CF::Up,     RC::CF::Up,     RC::CF::Up,
        RC::CF::Back,   RC::CF::Back,   RC::CF::Back,   RC::CF::Back,
        RC::CF::Down,   RC::CF::Down,   RC::CF::Down,   RC::CF::Down,
        RC::CF::Left,   RC::CF::Left,   RC::CF::Left,   RC::CF::Left,
        RC::CF::Right,  RC::CF::Right,  RC::CF::Right,  RC::CF::Right
    };

//          5-----4
//         /|    /|
//        3-+---2 |
//        | 7---+-6
//        |/    |/
//        0-----1

    const GLfloat m_aCubeVertices[ 3*4*6 ] = {
        // front
        -0.5, -0.5,  0.5, // 0
        0.5, -0.5,  0.5, // 1
            0.5,  0.5,  0.5, // 2
        -0.5,  0.5,  0.5, // 3
        // top
        -0.5,  0.5,  0.5, // 3
        0.5,  0.5,  0.5, // 2
        0.5,  0.5, -0.5, // 4
        -0.5,  0.5, -0.5, // 5
        // back
        0.5, -0.5, -0.5, // 6
        -0.5, -0.5, -0.5, // 7
        -0.5,  0.5, -0.5, // 5
        0.5,  0.5, -0.5, // 4
        // bottom
        0.5, -0.5,  0.5, // 1
        -0.5, -0.5,  0.5, // 0
        -0.5, -0.5, -0.5, // 7
        0.5, -0.5, -0.5, // 6
        // left
        -0.5, -0.5, -0.5, // 7
        -0.5, -0.5,  0.5, // 0
        -0.5,  0.5,  0.5, // 3
        -0.5,  0.5, -0.5, // 5
        // right
        0.5, -0.5,  0.5, // 1
        0.5, -0.5, -0.5, // 6
        0.5,  0.5, -0.5, // 4
        0.5,  0.5,  0.5, // 2
    };
};

#endif // RUBIKSCUBE_H
