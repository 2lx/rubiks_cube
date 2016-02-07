#ifndef KEYSTATE_H
#define KEYSTATE_H

#include <queue>
#include <stack>
#include <map>
#include "rcubeparams.h"
#include "enumiterator.h"

class KeyQueue
{
public:
    KeyQueue()
    {
		for ( auto it : Enum< RC::GK >() )
			m_map[ it ] = KeyState();
    }

	inline void keyDown( const RC::GK gk, const bool pushQ = true )
	{
		if ( !m_map[ gk ].isHold )
		{
			m_map[ gk ].isNewDown = true;
			m_map[ gk ].isHold = true;

			if ( pushQ )
				m_queue.push( gk );
		}
	};

    inline void keyUp( const RC::GK gk )
    {
		if ( m_map[ gk ].isHold )
			m_map[ gk ].isHold = false;
	};

	inline void processKey( const RC::GK gk )
	{
		if ( m_map[ gk ].isNewDown )
			m_map[ gk ].isNewDown = false;
	};

	inline bool isHold( const RC::GK gk ) const
	{
		return m_map.at( gk ).isHold;
	};

	inline bool isNewDown( const RC::GK gk ) const
	{
		return m_map.at( gk ).isNewDown;
	};

	// queue
	inline RC::GK qCurKey() const
	{
		if ( !m_queue.empty() )
			return m_queue.front();
		else return RC::GK::NONE;
	}

	inline void qPushKey( const RC::GK gk ) { m_queue.push( gk ); }

	inline void qPopKey()
	{
		if ( m_queue.empty() )
			return;

		processKey( m_queue.front() );

		const RC::GK prevGk = RC::GKPar::prevGK( m_queue.front() );
		if ( prevGk != RC::GK::NONE )
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
	inline RC::GK prevPop()
	{
		if ( !m_stack.empty() )
		{
			const RC::GK gk = m_stack.top();
			m_stack.pop();

			return gk;
		}
		return RC::GK::NONE;
	}

	inline void prevPush( const RC::GK gk )
	{
		const RC::GK prevGk = RC::GKPar::prevGK( gk );
		if ( prevGk != RC::GK::NONE )
			m_stack.push( prevGk );
	}

private:
	struct KeyState
	{
		bool isNewDown;
		bool isHold;
	};

	std::map< RC::GK, KeyState > m_map;
//	KeyState m_map[ ( int ) RC::GK::COUNT ];
	std::queue< RC::GK > m_queue;
	std::stack< RC::GK > m_stack;	// previous keys
};

#endif // KEYSTATE_H
