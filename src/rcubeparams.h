#ifndef RCUBEPARAMS_H
#define RCUBEPARAMS_H

#include <map>
#include <glm/gtc/type_ptr.hpp>

namespace RC
{
	namespace RCDetail
	{
		extern const glm::vec3 m_RAPar[ RA_COUNT ];
	}

	namespace RAPar
	{
		glm::vec3 getVecForRA( const RotAxis ra );
		RotAxis getClosestAxis( glm::vec3 vec );
	}


	class MoveParams	// singleton
	{
	public:
//		static RotAxis rotAxis( const MoveType mt ) { return m_p[ mt ]->m_RA; };
		static glm::vec3 vec( const MoveType mt ) { return RAPar::getVecForRA( m_p[ mt ]->m_RA ); };
		static bool clockwise( const MoveType mt ) { return m_p[ mt ]->m_clockwise; };
		static int layer( const MoveType mt ) { return m_p[ mt ]->m_layer; };

		static MoveType getMTypeForPars( const glm::vec3 & vec, const bool cw, const int lay );

		static void cleanup();

	private:
		class OneParam	// hidden class
		{
		public:
			OneParam( const RotAxis ra, const int lay, const bool clockwise )
				: m_clockwise { clockwise }, m_RA{ ra }, m_layer( lay )
			{ };

			const bool m_clockwise;
			const RotAxis m_RA;
			const int m_layer;
		};

		MoveParams() {};
		MoveParams( const MoveParams & ) = delete;
		MoveParams & operator = ( const MoveParams & ) = delete;

		static std::map< MoveType, OneParam * > InitMap();
		static std::map< MoveType, OneParam * > m_p;
	};
}
#endif // RCUBEPARAMS_H
