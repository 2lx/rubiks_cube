#ifndef RCUBEPARAMS_H
#define RCUBEPARAMS_H

#include <map>
#include <glm/gtc/type_ptr.hpp>

namespace RC
{
	class AxisParams	// Singleton
	{
	public:
	//	static glm::vec3 vec( const RCAxis ax ) { return m_p[ ax ]->m_vec; };
	//	static RCAxis getAxisForVector( const glm::vec3 & vec );

		static void cleanup();

	private:
		class AxisParam	// Hidden class
		{
		public:
			AxisParam( const GLfloat vX, const GLfloat vY, const GLfloat vZ )
				: m_vec { vX, vY, vZ }
			{
			};

			const glm::vec3 m_vec;
		};

		AxisParams() {};
		AxisParams( const AxisParams & ) = delete;
		AxisParams & operator= ( const AxisParams & ) = delete;

		static std::map< RCAxis, AxisParam * > InitMap();
		static std::map< RCAxis, AxisParam * > m_p;
	};

	class MoveParams	// Singleton
	{
	public:
		static glm::vec3 vec( const RCMoveType mt ) { return m_p[ mt ]->m_vec; };
//		static RCAxis axis( const RCMoveType mt ) { return m_p[ mt ]->m_axis; };
		static bool clockwise( const RCMoveType mt ) { return m_p[ mt ]->m_clockwise; };
		static RCMoveType getMTypeForPars( const glm::vec3 & vec, const bool cw );
//		static RCMoveType getMTypeForPars( const RCAxis ax, const bool cw );

		static void cleanup();

	private:
		class OneParam	// Hidden class
		{
		public:
			OneParam( const GLfloat x, const GLfloat y, const GLfloat z, const RCAxis ax, const bool clockwise )
				: m_vec { x, y, z }, m_clockwise { clockwise }, m_axis{ ax }
			{ };

			const glm::vec3 m_vec;
			const bool m_clockwise;
			const RCAxis m_axis;
		};

		MoveParams() {};
		MoveParams( const MoveParams & ) = delete;
		MoveParams & operator= ( const MoveParams & ) = delete;

		static std::map< RCMoveType, OneParam * > InitMap();
		static std::map< RCMoveType, OneParam * > m_p;
	};
}
#endif // RCUBEPARAMS_H
