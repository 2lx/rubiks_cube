#ifndef RCUBEMODEL_H
#define RCUBEMODEL_H

#include <map>
#include "shapes.h"

const int CUBIE_COUNT = 3;
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
	MT_NONE,
	MT_FIRST = MT_FRONT,
	MT_LAST = MT_DOWNINV
};

class RCMoveTable	// Singleton
{
public:
	static Vector3D vec( const RCMoveType mt ) { return m_p[ mt ]->m_vec; };
	static bool clockwise( const RCMoveType mt ) { return m_p[ mt ]->m_isClockwise; };

//	static RCMoveTable * par( const RCMoveType mt )	{ return m_p[ mt ]; };

private:
	RCMoveTable( const GLfloat vX, const GLfloat vY, const GLfloat vZ, const bool nisClockwise )
		: m_vec { vX, vY, vZ }, m_isClockwise { nisClockwise }
	{ };

	static std::map< RCMoveType, RCMoveTable * > InitMap()
	{
		std::map< RCMoveType, RCMoveTable * > mp;
		mp[ MT_FRONT ] 		= new RCMoveTable(  0,  0,  1, true );
		mp[ MT_FRONTINV ] 	= new RCMoveTable(  0,  0,  1, false );
		mp[ MT_BACK ] 		= new RCMoveTable(  0,  0, -1, true );
		mp[ MT_BACKINV ] 	= new RCMoveTable(  0,  0, -1, false );
		mp[ MT_RIGHT ] 		= new RCMoveTable(  1,  0,  0, true );
		mp[ MT_RIGHTINV ] 	= new RCMoveTable(  1,  0,  0, false );
		mp[ MT_LEFT ] 		= new RCMoveTable( -1,  0,  0, true );
		mp[ MT_LEFTINV ] 	= new RCMoveTable( -1,  0,  0, false );
		mp[ MT_UP ] 		= new RCMoveTable(  0,  1,  0, true );
		mp[ MT_UPINV ] 		= new RCMoveTable(  0,  1,  0, false );
		mp[ MT_DOWN ] 		= new RCMoveTable(  0, -1,  0, true );
		mp[ MT_DOWNINV ] 	= new RCMoveTable(  0, -1,  0, false );

		return mp;
	};

	static std::map< RCMoveType, RCMoveTable * > m_p;
	const Vector3D m_vec = { 0, 0, 0 };
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
		RCCubie m_cubies[ CUBIE_COUNT ][ CUBIE_COUNT ][ CUBIE_COUNT ];
};

#endif // RCUBEMODEL_H
