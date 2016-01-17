#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <GL/gl.h>
#include "gameobject.h"
#include "shapes.h"

enum RCRotateType
{
	RT_LEFT,
	RT_LEFTI, // I (inverse) - counter-clockwise move
	RT_RIGHT,
	RT_RIGHTI,
	RT_BACK,
	RT_BACKI,
	RT_FRONT,
	RT_FRONTI,
	RT_UP,
	RT_UPI,
	RT_DOWN,
	RT_DOWNI,

	RT_COUNT
};

const int PIECE_COUNT = 3;
const float CUBE_EDGE = 1.0;

class RubiksCube : public GameObject
{
	public:
		RubiksCube();
		virtual ~RubiksCube();

		void drawObject();
		void moveCube( const RCRotateType rt );
	protected:
	private:
		CubePiece m_pieces[ PIECE_COUNT ][ PIECE_COUNT ][ PIECE_COUNT ];

		void vertexCube( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize );
		void vertexPiece( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize, const int x, const int y, const int z );
};

#endif // RUBIKSCUBE_H
