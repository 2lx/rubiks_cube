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

	inline void keyDown( const RC::GameKey gk, const bool pushQ = true )
	{
		if ( !m_map[ gk ].isHold )
		{
			m_map[ gk ].isNewDown = true;
			m_map[ gk ].isHold = true;

			if ( pushQ )
				m_queue.push( gk );
		}
	};

    inline void keyUp( const RC::GameKey gk )
    {
		if ( m_map[ gk ].isHold )
			m_map[ gk ].isHold = false;
	};

	inline void processKey( const RC::GameKey gk )
	{
		if ( m_map[ gk ].isNewDown )
			m_map[ gk ].isNewDown = false;
	};

	inline bool isHold( const RC::GameKey gk ) const
	{
		return m_map[ gk ].isHold;
	};

	inline bool isNewDown( const RC::GameKey gk ) const
	{
		return m_map[ gk ].isNewDown;
	};

	// queue
	inline RC::GameKey qCurKey() const
	{
		if ( !m_queue.empty() )
			return m_queue.front();
		else return RC::GK_NONE;
	}

	inline void qPushKey( const RC::GameKey gk ) { m_queue.push( gk ); }

	inline void qPopKey()
	{
		if ( m_queue.empty() )
			return;

		processKey( m_queue.front() );

		const RC::GameKey prevGk = getPrevGKey( m_queue.front() );
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
	inline RC::GameKey prevPop()
	{
		if ( !m_stack.empty() )
		{
			RC::GameKey gk = m_stack.top();
			m_stack.pop();

			return gk;
		}
		return RC::GK_NONE;
	}

	inline void prevPush( const RC::GameKey gk )
	{
		const RC::GameKey prevGk = getPrevGKey( gk );
		if ( prevGk != RC::GK_NONE )
			m_stack.push( prevGk );
	}

	bool isEnableWithMove( const RC::GameKey gk )
	{
        if ( ( gk < RC::GK_MOVEFIRST || gk > RC::GK_MOVELAST )
				&& gk != RC::GK_CUBEMIX && gk != RC::GK_CUBERESET && gk != RC::GK_CUBEUNDO )
			return true;
		else return false;
	}

	RC::GameKey getPrevGKey( const RC::GameKey gk )
	{
		switch( gk )
		{
		case RC::GK_ROTATEDOWN: 	return RC::GK_ROTATEUP;
		case RC::GK_ROTATEUP: 		return RC::GK_ROTATEDOWN;
		case RC::GK_ROTATELEFT: 	return RC::GK_ROTATERIGHT;
		case RC::GK_ROTATERIGHT: 	return RC::GK_ROTATELEFT;
		case RC::GK_ROTATECW: 		return RC::GK_ROTATEACW;
		case RC::GK_ROTATEACW: 		return RC::GK_ROTATECW;

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
	std::queue< RC::GameKey > m_queue;
	std::stack< RC::GameKey > m_stack;	// previous keys
};

#endif // KEYSTATE_H
