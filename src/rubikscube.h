#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <GL/gl.h>
#include "gameobject.h"
#include "shapes.h"

const int PIECE_COUNT = 3;

class RubiksCube : public GameObject
{
	public:
		RubiksCube();
		virtual ~RubiksCube();

		void drawObject();
	protected:
	private:
		CubePiece m_pieces[ PIECE_COUNT ][ PIECE_COUNT ][ PIECE_COUNT ];

		void vertexCube( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize );
		void vertexPiece( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize, const int x, const int y, const int z );
};

#endif // RUBIKSCUBE_H
