#ifndef RCUBEPARAMS_H
#define RCUBEPARAMS_H

#include <map>
#include <glm/gtc/type_ptr.hpp>

namespace RC
{
	extern const glm::vec3 RAPar[ RA_COUNT ];
	glm::vec3 getVecForRA( const RotAxis ra );

/*	class AxisParams	// Singleton
	{
	public:
	//	static glm::vec3 vec( const RCAxis ax ) { return m_p[ ax ]->m_vec; };
	//	static RCAxis getAxisForVector( const glm::vec3 & vec );

		static void cleanup();

	private:
		const glm::vec3 m_vec;

		AxisParams() {};
		AxisParams( const AxisParams & ) = delete;
		AxisParams & operator= ( const AxisParams & ) = delete;



		static std::map< RotAxis, glm::vec3 > InitMap();
		static std::map< RotAxis, AxisParam * > m_p;
	};
*/
	class MoveParams	// Singleton
	{
	public:
		static glm::vec3 vec( const MoveType mt ) { return m_p[ mt ]->m_vec; };
//		static RCAxis axis( const RCMoveType mt ) { return m_p[ mt ]->m_axis; };
		static bool clockwise( const MoveType mt ) { return m_p[ mt ]->m_clockwise; };
		static MoveType getMTypeForPars( const glm::vec3 & vec, const bool cw );
//		static RCMoveType getMTypeForPars( const RCAxis ax, const bool cw );

		static void cleanup();

	private:
		class OneParam	// Hidden class
		{
		public:
			OneParam( const GLfloat x, const GLfloat y, const GLfloat z,/* const RotAxis ra, */const bool clockwise )
				: m_vec { x, y, z }, m_clockwise { clockwise }//, m_RA{ ra }
			{ };

			const glm::vec3 m_vec;
			const bool m_clockwise;
//			const RotAxis m_RA;
		};

		MoveParams() {};
		MoveParams( const MoveParams & ) = delete;
		MoveParams & operator= ( const MoveParams & ) = delete;

		static std::map< MoveType, OneParam * > InitMap();
		static std::map< MoveType, OneParam * > m_p;
	};
}
#endif // RCUBEPARAMS_H
