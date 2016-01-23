#ifndef RCUBEMODEL_H
#define RCUBEMODEL_H

#include "shapes.h"

namespace RC
{
	const int CUBIE_COUNT = 3;
	const float CUBE_EDGE = 1.0;

	enum RCAxis
	{
		AX_FRONT, AX_UP, AX_BACK, AX_DOWN, AX_LEFT, AX_RIGHT,
		AX_COUNT, AX_NONE,
		AX_FIRST = AX_FRONT
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
		MT_FIRST = MT_FRONT
	};

	class Cubie
	{
	public:
		Cubie() { };

		void rotateCubie( const RCMoveType rt );
		float colR( const RCAxis ax ) const;
		float colG( const RCAxis ax ) const;
		float colB( const RCAxis ax ) const;
		inline void setColourInd( const RCAxis ax, const RCAxis cind ) { m_colourInd[ ax ] = cind; };

	private:
		RCAxis m_colourInd[ AX_COUNT ] = { AX_NONE, AX_NONE, AX_NONE, AX_NONE, AX_NONE, AX_NONE };
	};

	class CubeModel
	{
		public:
			CubeModel();
			virtual ~CubeModel();

			void moveCubies( const RCMoveType rt );
			const Cubie cubie( const int x, const int y, const int z ) const { return m_cubies[ x ][ y ][ z ]; };
		protected:

		private:
			Cubie m_cubies[ CUBIE_COUNT ][ CUBIE_COUNT ][ CUBIE_COUNT ];
	};
}

#endif // RCUBEMODEL_H
