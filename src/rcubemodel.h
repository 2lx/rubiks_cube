#ifndef RCUBEMODEL_H
#define RCUBEMODEL_H

const int PIECE_COUNT = 3;
const float CUBE_EDGE = 1.0;

enum RCAxis
{
	AX_FRONT, AX_UP, AX_BACK, AX_DOWN, AX_LEFT, AX_RIGHT,
	AX_COUNT
};

enum RCMoveType
{
	MT_FRONT, MT_FRONTINV,
	MT_BACK, MT_BACKINV,
	MT_LEFT, MT_LEFTINV,
	MT_RIGHT, MT_RIGHTINV,
	MT_UP, MT_UPINV,
	MT_DOWN, MT_DOWNINV,

	MT_COUNT,
	MT_NONE
};

class RCMoveParam
{
public:
	RCMoveParam( const short int vX, const short int vY, const short int vZ, const bool nisClockwise )
		: m_x { vX }, m_y { vY }, m_z { vZ }, m_isClockwise { nisClockwise }
	{ };
	short int axisN() const {
		if ( m_x != 0 ) return 0;
		else if ( m_y != 0 ) return 1;
		else return 2;
		};
	short int x() const { return m_x; };
	short int y() const { return m_y; };
	short int z() const { return m_z; };
	bool isClockwise() const { return m_isClockwise; };
	bool isClockwiseAbs() const { if ( m_x + m_y + m_z > 0 ) return m_isClockwise; else return !m_isClockwise; };
	bool isFront() const { return ( m_x + m_y + m_z > 0 ); };

private:
	const short int m_x = 0;
	const short int m_y = 0;
	const short int m_z = 0;
    const bool m_isClockwise = true;
};

class RCCubie
{
public:
	RCCubie() { };

	void rotateCubie( const RCMoveType rt );
	float colR( const RCAxis ax ) const;
	float colG( const RCAxis ax ) const;
	float colB( const RCAxis ax ) const;
	inline void setColourInd( const RCAxis ax, const short int cind ) { m_colourInd[ ax ] = cind; };

private:
	short int m_colourInd[ AX_COUNT ] = { -1, -1, -1, -1, -1, -1 };
};

class RCubeModel
{
	public:
		RCubeModel();
		virtual ~RCubeModel();

		void moveCubies( const RCMoveType rt );
		const RCCubie cubie( const int x, const int y, const int z ) const { return m_cubies[ x ][ y ][ z ]; };
	protected:

	private:
		RCCubie m_cubies[ PIECE_COUNT ][ PIECE_COUNT ][ PIECE_COUNT ];
};

#endif // RCUBEMODEL_H
