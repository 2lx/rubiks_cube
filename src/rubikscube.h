#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

const unsigned int PIECE_COUNT = 3;

struct CubePiece
{
	unsigned int colInd;
};

class RubiksCube
{
	public:
		RubiksCube();
		virtual ~RubiksCube();

		void drawCube();
	protected:
	private:
		CubePiece m_pieces[ PIECE_COUNT ][ PIECE_COUNT ][ PIECE_COUNT ];

		void vertexCube( const float pX, const float pY, const float pZ, const float cubeSize );
		void drawPiece( const float pX, const float pY, const float pZ, const float cubeSize, const int colInd );
};

#endif // RUBIKSCUBE_H
