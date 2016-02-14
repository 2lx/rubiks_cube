#include "all.h"

#include "shaderprogram.h"

#include "shader.h"
#include <iostream>

using namespace std;

ShaderProgram::ShaderProgram()
{
    m_id = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram( m_id );
}

void ShaderProgram::attachShader( const Shader & shader )
{
    glAttachShader( m_id, shader.getId() );
}

void ShaderProgram::linkProgram()
{
    glLinkProgram( m_id );

    GLint linkStatus = GL_FALSE;
    glGetProgramiv( m_id, GL_LINK_STATUS, &linkStatus );

    if ( linkStatus == GL_FALSE )
        throw std::runtime_error( "ShaderProgram::linkProgram(): Error linking shader program" );
}

GLint ShaderProgram::addAttribute( const std::string & attributeName )
{
    GLint ind = glGetAttribLocation( m_id, attributeName.c_str() );

    if ( ind == -1 )
        throw std::runtime_error( "ShaderProgram::addAttribute(): Error adding attribute: " + attributeName );
    else m_attributes[ attributeName ] = ind;

    return ind;
}

GLint ShaderProgram::addUniform( const string & uniformName )
{
    GLint ind = glGetUniformLocation( m_id, uniformName.c_str() );

    if ( ind == -1 )
        throw std::runtime_error( "ShaderProgram::addUniform(): Could not add uniform: " + uniformName );
    else m_uniforms[ uniformName ] = ind;

    return ind;
}
