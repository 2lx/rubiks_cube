#ifndef KEYSTATE_H
#define KEYSTATE_H

#include <queue>
#include <stack>
#include <map>
#include "rcdefines.h"

class KeyQueue
{
public:
    KeyQueue() { }

	inline void keyDown( const RC::GameKeys gk, const bool pushQ = true )
	{
		if ( !m_map[ gk ].isHold )
		{
			m_map[ gk ].isNewDown = true;
			m_map[ gk ].isHold = true;

			if ( pushQ )
				m_queue.push( gk );
		}
	};

    inline void keyUp( const RC::GameKeys gk )
    {
		if ( m_map[ gk ].isHold )
			m_map[ gk ].isHold = false;
	};

	inline void processKey( const RC::GameKeys gk )
	{
		if ( m_map[ gk ].isNewDown )
			m_map[ gk ].isNewDown = false;
	};

	inline bool isHold( const RC::GameKeys gk ) const
	{
		return m_map[ gk ].isHold;
	};

	inline bool isNewDown( const RC::GameKeys gk ) const
	{
		return m_map[ gk ].isNewDown;
	};

	// queue
	inline RC::GameKeys qCurKey() const
	{
		if ( !m_queue.empty() )
			return m_queue.front();
		else return RC::GK_NONE;
	}

	inline void qPushKey( const RC::GameKeys gk ) { m_queue.push( gk ); }

	inline void qPopKey()
	{
		if ( m_queue.empty() )
			return;

		processKey( m_queue.front() );

		const RC::GameKeys prevGk = getPrevGKey( m_queue.front() );
		if ( prevGk != RC::GK_NONE )
			m_stack.push( prevGk );

		m_queue.pop();
	}

	inline int qSize() const { return m_queue.size(); }

	// prev stack
	inline void prevClear()
	{
		while ( !m_stack.empty() )
			m_stack.pop();
	}
/*
	inline RC::GameKeys prevTop() const
	{
		if ( !m_stack.empty() )
			return m_stack.top();
		else GK_NONE;
	}
*/
	inline RC::GameKeys prevPop()
	{
		if ( !m_stack.empty() )
		{
			RC::GameKeys gk = m_stack.top();
			m_stack.pop();

			return gk;
		}
		return RC::GK_NONE;
	}

	inline void prevPush( const RC::GameKeys gk )
	{
		const RC::GameKeys prevGk = getPrevGKey( gk );
		if ( prevGk != RC::GK_NONE )
			m_stack.push( prevGk );
	}

	bool isEnableWithMove( const RC::GameKeys gk )
	{
        if ( ( gk < RC::GK_MOVEFIRST || gk > RC::GK_MOVELAST )
				&& gk != RC::GK_CUBEMIX && gk != RC::GK_CUBERESET && gk != RC::GK_CUBEUNDO )
			return true;
		else return false;
	}

	RC::GameKeys getPrevGKey( const RC::GameKeys gk )
	{
		switch( gk )
		{
		case RC::GK_LOOKDOWN: 		return RC::GK_LOOKUP;
		case RC::GK_LOOKUP: 		return RC::GK_LOOKDOWN;
		case RC::GK_LOOKLEFT: 		return RC::GK_LOOKRIGHT;
		case RC::GK_LOOKRIGHT: 		return RC::GK_LOOKLEFT;
		case RC::GK_LOOKCW: 		return RC::GK_LOOKACW;
		case RC::GK_LOOKACW: 		return RC::GK_LOOKCW;

		case RC::GK_MOVEBACK: 		return RC::GK_MOVEBACKINV;
		case RC::GK_MOVEBACKINV: 	return RC::GK_MOVEBACK;
		case RC::GK_MOVEDOWN: 		return RC::GK_MOVEDOWNINV;
		case RC::GK_MOVEDOWNINV: 	return RC::GK_MOVEDOWN;
		case RC::GK_MOVEFRONT: 		return RC::GK_MOVEFRONTINV;
		case RC::GK_MOVEFRONTINV: 	return RC::GK_MOVEFRONT;
		case RC::GK_MOVELEFT: 		return RC::GK_MOVELEFTINV;
		case RC::GK_MOVELEFTINV: 	return RC::GK_MOVELEFT;
		case RC::GK_MOVERIGHT: 		return RC::GK_MOVERIGHTINV;
		case RC::GK_MOVERIGHTINV: 	return RC::GK_MOVERIGHT;
		case RC::GK_MOVEUP: 		return RC::GK_MOVEUPINV;
		case RC::GK_MOVEUPINV:		return RC::GK_MOVEUP;
		default: return RC::GK_NONE;
		}
	}

private:
	struct KeyState
	{
		bool isNewDown;
		bool isHold;
	};

	KeyState m_map[ RC::GK_COUNT ];
	std::queue< RC::GameKeys > m_queue;
	std::stack< RC::GameKeys > m_stack;	// previous keys
};

#endif // KEYSTATE_H
