#ifndef RCUBEMODEL_H
#define RCUBEMODEL_H

#include "rcdefines.h"

namespace RC
{
	class Cubie
	{
	public:
		Cubie() { };

		void rotateCubie( const RCMoveType rt );
		RCAxis colInd( const RCAxis ax ) const { return m_colourInd[ ax ]; };
		inline void setColourInd( const RCAxis ax, const RCAxis cind ) { m_colourInd[ ax ] = cind; };

	private:
		RCAxis m_colourInd[ AX_COUNT ] = { AX_NONE, AX_NONE, AX_NONE, AX_NONE, AX_NONE, AX_NONE };
	};

	class CubeModel
	{
		public:
			CubeModel();
			virtual ~CubeModel();

			void moveCubies( const RCMoveType rt, const int mLayer );
			const Cubie cubie( const int x, const int y, const int z ) const { return m_cubies[ x ][ y ][ z ]; };
			void reset();
		protected:

		private:
			Cubie m_cubies[ CUBIE_COUNT ][ CUBIE_COUNT ][ CUBIE_COUNT ];
	};
}

#endif // RCUBEMODEL_H
