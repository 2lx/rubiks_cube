#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader
{
public:
    Shader( const GLenum & type );

    GLuint getId() const { return m_id; }

    void loadFromFile( const std::string & filename ) const;

    void compile() const;

private:
    GLuint m_id;
    const GLenum m_shType;
};

#endif // SHADER_H
