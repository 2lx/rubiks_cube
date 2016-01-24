#include "all.h"

#include "rcdefines.h"

using namespace RC;

RCAxis RC::getInvAxis( const RCAxis ax )
{
	if ( ax == AX_FRONT )
		return AX_BACK;
	else if ( ax == AX_BACK )
		return AX_FRONT;
	else if ( ax == AX_LEFT )
		return AX_RIGHT;
	else if ( ax == AX_RIGHT )
		return AX_LEFT;
	else if ( ax == AX_UP )
		return AX_DOWN;
	else if ( ax == AX_DOWN )
		return AX_UP;
	else return AX_NONE;
}

RCAxis RC::getThirdAxis( const RCAxis ax1, const RCAxis ax2 )
{
	if ( ax1 == AX_FRONT || ax1 == AX_BACK )
	{
		if ( ax2 == AX_LEFT || ax2 == AX_RIGHT )
			return AX_UP;
		else if ( ax2 == AX_UP || ax2 == AX_DOWN )
			return AX_RIGHT;
		else return AX_NONE;
	}
	else if ( ax1 == AX_LEFT || ax1 == AX_RIGHT )
	{
		if ( ax2 == AX_FRONT || ax2 == AX_BACK )
			return AX_UP;
		else if ( ax2 == AX_UP || ax2 == AX_DOWN )
			return AX_FRONT;
		else return AX_NONE;
	}
	else if ( ax1 == AX_UP || ax1 == AX_DOWN )
	{
		if ( ax2 == AX_FRONT || ax2 == AX_BACK )
			return AX_RIGHT;
		else if ( ax2 == AX_LEFT || ax2 == AX_RIGHT )
			return AX_FRONT;
		else return AX_NONE;
	}
	else return AX_NONE;
}

bool RC::isMTInvAxis( const RCMoveType mt )
{
	if ( mt == MT_FRONTINV || mt == MT_FRONT || mt == MT_RIGHTINV || mt == MT_RIGHT || mt == MT_UPINV || mt == MT_UP )
		return false;
	else return true;
}
