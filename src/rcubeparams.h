#ifndef RCUBEPARAMS_H
#define RCUBEPARAMS_H

#include <map>
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
		RCS_S3,
		RCS_COUNT
	};


	class AxisParams	// Singleton
	{
	public:
		static Vector3D vec( const RCAxis ax ) { return * m_p[ ax ]; };
		static RCAxis getAxisForVector( const Vector3D & vec );

		static void cleanup();

	private:
		AxisParams() {};
		AxisParams( const AxisParams & ) = delete;
		AxisParams & operator= ( const AxisParams & ) = delete;

		static std::map< RCAxis, Vector3D * > InitMap();
		static std::map< RCAxis, Vector3D * > m_p;
	};

	class MoveParams	// Singleton
	{
	public:
		static Vector3D vec( const RCMoveType mt ) { return m_p[ mt ]->m_vec; };
		static bool clockwise( const RCMoveType mt ) { return m_p[ mt ]->m_clockwise; };
		static RCMoveType getMTypeForPars( const Vector3D & vec, const bool cw );

		static void cleanup();

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
		MoveParams( const MoveParams & ) = delete;
		MoveParams & operator= ( const MoveParams & ) = delete;

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

		static void setScheme( const RCColorScheme sc ) { m_schemeId = sc; };
		static void incScheme() { m_schemeId = RCColorScheme( ( m_schemeId + 1 ) % RCS_COUNT ); };
		static void cleanup();

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
		Colors( const Colors & ) = delete;
		Colors & operator= ( const Colors & ) = delete;

		static std::map< RCColorScheme, std::map< RCColor, OneColor * > > InitMapRC();
		static std::map< RCColorScheme, std::map< RCAxis, OneColor * > > InitMapAX();

		static std::map< RCColorScheme, std::map< RCAxis, OneColor * > > m_ax;
		static std::map< RCColorScheme, std::map< RCColor, OneColor * > > m_c;

		static RCColorScheme m_schemeId;
	};

}
#endif // RCUBEPARAMS_H
