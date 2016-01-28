#include "all.h"

#include "shader.h"
#include <fstream>
#include <sstream>

using namespace std;

Shader::Shader( const GLenum & type ) : m_shType ( type )
{
	m_id = glCreateShader( type );
}

char* file_read(const char* filename, int* size) {
	SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
	if (rw == NULL) return NULL;

	Sint64 res_size = SDL_RWsize(rw);
	char* res = (char*)malloc(res_size + 1);

	Sint64 nb_read_total = 0, nb_read = 1;
	char* buf = res;
	while (nb_read_total < res_size && nb_read != 0) {
		nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
		nb_read_total += nb_read;
		buf += nb_read;
	}
	SDL_RWclose(rw);
	if (nb_read_total != res_size) {
		free(res);
		return NULL;
	}

	res[nb_read_total] = '\0';
	if (size != NULL)
		*size = nb_read_total;
	return res;
}

void Shader::loadFromFile( const string & filename ) const
{
/*	std::ifstream file;

	file.open( filename.c_str() );

	if ( !file.good() )
	{
		std::cout << "Failed to open file: " << filename << std::endl;
		return;
	}

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

//	std::string str = { stream.str().c_str() };
//	str += '\0';
	const GLchar * source = { stream.str().c_str() };
//	source[ stream.str().length() ] = '\0';
*/
	const GLchar* source = file_read(filename.c_str(), NULL);

	if ( source == NULL) {
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
					   "Error opening %s: %s", filename.c_str(), SDL_GetError());
		return;
	}

	// GLSL version
	const char* version;
	int profile;
	SDL_GL_GetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, &profile );

	if ( profile == SDL_GL_CONTEXT_PROFILE_ES )
		version = "#version 100\n";  // OpenGL ES 2.0
	else
		version = "#version 120\n";  // OpenGL 2.1

	// GLES2 precision specifiers
	const char* precision;
	precision =
		"#ifdef GL_ES                        \n"
		"#  ifdef GL_FRAGMENT_PRECISION_HIGH \n"
		"     precision highp float;         \n"
		"#  else                             \n"
		"     precision mediump float;       \n"
		"#  endif                            \n"
		"#else                               \n"
		// Ignore unsupported precision specifiers
		"#  define lowp                      \n"
		"#  define mediump                   \n"
		"#  define highp                     \n"
		"#endif                              \n";

	const GLchar* sources[] = {
		version,
		precision,
		source
	};

	glShaderSource( m_id, 3, sources, NULL );

	free( ( void * )source );
//	std::cout << ( * sources ) << std::endl;
}

void Shader::compile() const
{
	glCompileShader( m_id );
	GLint compile_ok = GL_FALSE;
	glGetShaderiv( m_id, GL_COMPILE_STATUS, &compile_ok );

	if ( compile_ok == GL_FALSE )
	{
		std::cout << "Shader " << ( int ) m_id << "complie failed." << std::endl;

//		SDL_LogMessage( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s:\n", " fd" );

//		GLint infoLogLength;
//		glGetShaderiv( m_id, GL_INFO_LOG_LENGTH, &infoLogLength );
//		GLchar * strInfoLog = new GLchar[infoLogLength + 1];
//		glGetShaderInfoLog( m_id, infoLogLength, NULL, strInfoLog )

//		print_log( res );
//		glDeleteShader( m );
		return;
	}
}
