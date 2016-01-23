#ifndef RCUBEPARAMS_H
#define RCUBEPARAMS_H

#include "rcubemodel.h"

namespace RC
{
	enum RCColor
	{
		RC_BG,
		RC_FG,
		RC_COUNT
	};

	enum RCColorScheme
	{
		RCS_S1,
		RCS_S2,
		RCS_COUNT
	};


	class MoveParams	// Singleton
	{
	public:
		static Vector3D vec( const RCMoveType mt ) { return m_p[ mt ]->m_vec; };
		static bool clockwise( const RCMoveType mt ) { return m_p[ mt ]->m_clockwise; };

	private:
		class OneParam	// Hidden class
		{
		public:
			OneParam( const GLfloat x, const GLfloat y, const GLfloat z, const bool clockwise )
				: m_vec { x, y, z }, m_clockwise { clockwise }
			{ };

			const Vector3D m_vec;
			const bool m_clockwise;
		};

		MoveParams() {};
		static std::map< RCMoveType, OneParam * > InitMap();
		static std::map< RCMoveType, OneParam * > m_p;
	};

	class Colors	// Singleton
	{
	public:
		static float colR( const RCColor colId ) { return m_c[ m_schemeId ][ colId ]->m_R; };
		static float colG( const RCColor colId ) { return m_c[ m_schemeId ][ colId ]->m_G; };
		static float colB( const RCColor colId ) { return m_c[ m_schemeId ][ colId ]->m_B; };

		static float colR( const RCAxis colId ) { return m_ax[ m_schemeId ][ colId ]->m_R; };
		static float colG( const RCAxis colId ) { return m_ax[ m_schemeId ][ colId ]->m_G; };
		static float colB( const RCAxis colId ) { return m_ax[ m_schemeId ][ colId ]->m_B; };

	private:
		class OneColor
		{
		public:
			OneColor( const int r, const int g, const int b )
				: m_R( r / 255.0 ), m_G( g / 255.0 ), m_B( b / 255.0 )
			{ };

			const float m_R;
			const float m_G;
			const float m_B;
		};

		Colors() {};

		static std::map< RCColorScheme, std::map< RCColor, OneColor * > > InitMapRC();
		static std::map< RCColorScheme, std::map< RCAxis, OneColor * > > InitMapAX();

		static std::map< RCColorScheme, std::map< RCAxis, OneColor * > > m_ax;
		static std::map< RCColorScheme, std::map< RCColor, OneColor * > > m_c;

		static RCColorScheme m_schemeId;
	};

}
#endif // RCUBEPARAMS_H
