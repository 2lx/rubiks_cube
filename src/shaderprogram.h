#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <map>

class Shader;

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();
    void attachShader( const Shader & shader );
    void linkProgram();

    GLint addAttribute( const std::string & attributeName );
    GLint addUniform( const std::string & uniformName );

    GLuint id() { return m_id; }
    void disable() { glUseProgram( 0 ); }
private:
    GLuint m_id;

    std::map< std::string, GLint > m_attributes;
    std::map< std::string, GLint > m_uniforms;
};

#endif // SHADERPROGRAM_H
