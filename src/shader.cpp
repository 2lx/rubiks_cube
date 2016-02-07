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
	std::ifstream file;

	file.open( filename.c_str() );
	if ( file.fail() )
	{
		throw std::runtime_error( "Shader::loadFromFile(): Error opening file " + filename );
		return;
	}

	file.seekg ( 0, ios::end );
	int length = file.tellg();
	file.seekg ( 0, ios::beg );

	char * source = new char[ length + 1 ];
	source[ length ] = '\0';
	file.read ( source,length );
	file.close();

	if ( source == NULL) {
		throw std::runtime_error( "Shader::loadFromFile(): Error loading data from file" );
		return;
	}

	// GLSL version
	const char * version;
	int profile;
	SDL_GL_GetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, &profile );

	if ( profile == SDL_GL_CONTEXT_PROFILE_ES )
		version = "#version 100\n";  // OpenGL ES 2.0
	else
		version = "#version 120\n";  // OpenGL 2.1

	// GLES2 precision specifiers
	const char * precision =
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
		version,
		precision,
		source
	};

	glShaderSource( m_id, 3, sources, NULL );
	delete [] source;
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
