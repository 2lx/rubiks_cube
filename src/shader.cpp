#include "all.h"

#include "shader.h"
#include <fstream>
#include <sstream>

using namespace std;

Shader::Shader( const GLenum & type ) : m_shType ( type )
{
    m_id = glCreateShader( type );
}

void Shader::loadFromFile( const string & filename ) const
{
    std::ifstream file(filename, std::ifstream::ate);

    if ( file.fail() )
        throw std::runtime_error( "Shader::loadFromFile(): Error opening file " + filename );

    string source;
    source.reserve( file.tellg() );

    file.seekg ( 0, ios::beg );
    source.assign( std::istreambuf_iterator<char>(file),
                    std::istreambuf_iterator<char>() );
    file.close();

    if ( source.length() == 0 )
        throw std::runtime_error( "Shader::loadFromFile(): Error loading data from file" );

    // GLSL version
    int profile;
    SDL_GL_GetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, &profile );

    string version;
    if ( profile == SDL_GL_CONTEXT_PROFILE_ES )
         version = "#version 100\n";  // OpenGL ES 2.0
    else version = "#version 120\n";  // OpenGL 2.1

    // GLES2 precision specifiers
    const string precision =
        "#ifdef GL_ES                        \n"
        "#  ifdef GL_FRAGMENT_PRECISION_HIGH \n"
        "     precision highp float;         \n"
        "     precision highp ushort;        \n"
        "#  else                             \n"
        "     precision mediump float;       \n"
        "     precision mediump ushort;      \n"
        "#  endif                            \n"
        "#else                               \n"
        "#  define lowp                      \n"
        "#  define mediump                   \n"
        "#  define highp                     \n"
        "#endif                              \n";

    const GLchar * sources[] = {
        version.data(),
        precision.data(),
        source.data()
    };

    glShaderSource( m_id, 3, sources, NULL );
}

void Shader::compile() const
{
    glCompileShader( m_id );
    GLint compile_ok = GL_FALSE;
    glGetShaderiv( m_id, GL_COMPILE_STATUS, &compile_ok );

    if ( compile_ok == GL_FALSE )
    {
        GLint infoLogLength;
        glGetShaderiv( m_id, GL_INFO_LOG_LENGTH, &infoLogLength );

        GLchar * strInfoLog = new GLchar[ infoLogLength + 1 ];
        glGetShaderInfoLog( m_id, infoLogLength, NULL, strInfoLog );

        throw std::runtime_error( "Shader::compile(): Error compiling: " + std::string( strInfoLog ) );
    }
}
