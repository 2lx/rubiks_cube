#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "GL/glew.h"
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
	int m_shaderCount;

	std::map< std::string, GLint > attributeLocList;
	std::map< std::string, GLint > uniformLocList;

};

#endif // SHADERPROGRAM_H
