#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <GL/gl.h>
#include "gameobject.h"
#include "shapes.h"
#include <map>

const int PIECE_COUNT = 3;
const float CUBE_EDGE = 1.0;

class RubiksCube : public GameObject
{
public:
	RubiksCube();
	virtual ~RubiksCube();

	void drawObject();
	void movePieces( const RCMoveType rt );
	void setMove( const RCMoveType newRT );
	bool isMoving() const { return m_moveType != MT_NONE; }

protected:
private:
	CubePiece m_pieces[ PIECE_COUNT ][ PIECE_COUNT ][ PIECE_COUNT ];

	MyQuaternion m_moveQuat;
	GLfloat m_moveAngle = { 0 };
	RCMoveType m_moveType = { MT_NONE };

	void vertexCube( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize );
	void vertexPiece( const GLfloat pX, const GLfloat pY, const GLfloat pZ, const GLfloat cubeSize, const int x, const int y, const int z );

	std::map< RCMoveType, RCMoveParam * > m_paramsMap;
};

#endif // RUBIKSCUBE_H
