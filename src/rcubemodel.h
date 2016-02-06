#ifndef RCUBEMODEL_H
#define RCUBEMODEL_H

#include "rcdefines.h"

namespace RC
{
	class Cubie
	{
	public:
		Cubie() { };

		void rotateCubie( const MoveType rt );
		inline CubeFace colInd( const CubeFace cf ) const { return m_colourInd[ cf ]; };
		inline void setColourInd( const CubeFace cf, const CubeFace cind ) { m_colourInd[ cf ] = cind; };

	private:
		CubeFace m_colourInd[ CF_COUNT ] = { CF_NONE, CF_NONE, CF_NONE, CF_NONE, CF_NONE, CF_NONE };
	};

	class CubeModel
	{
		public:
			CubeModel();
			virtual ~CubeModel();

			void moveCubies( const MoveType rt, const int mLayer );
			const Cubie cubie( const int x, const int y, const int z ) const { return m_cubies[ x ][ y ][ z ]; };
			void reset();
		protected:

		private:
			Cubie m_cubies[ CUBIE_COUNT ][ CUBIE_COUNT ][ CUBIE_COUNT ];
	};
}

#endif // RCUBEMODEL_H
