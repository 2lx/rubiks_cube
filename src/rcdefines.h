#ifndef RCDEFINES_H_INCLUDED
#define RCDEFINES_H_INCLUDED

namespace RC
{
	const int CUBIE_COUNT = 16;
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

	RCAxis getInvAxis( const RCAxis ax );
	RCAxis getThirdAxis( const RCAxis ax1, const RCAxis ax2 );
	bool isMTInvAxis( const RCMoveType mt );
}

#endif // RCDEFINES_H_INCLUDED
