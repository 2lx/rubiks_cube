#ifndef KEYSTATE_H
#define KEYSTATE_H

#include <queue>
#include <map>
#include "rcdefines.h"

class KeyQueue
{
public:
    KeyQueue() { }

	inline void keyDown( const RC::GameKeys gk )
	{
		if ( !m_map[ gk ].isHold )
		{
			m_map[ gk ].isNewDown = true;
			m_map[ gk ].isHold = true;

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

	inline RC::GameKeys getKey()
	{
		const RC::GameKeys gk = m_queue.front();
		m_queue.pop();
		return gk;
	}

	inline int qSize() const { return m_queue.size(); }

private:
	struct KeyState
	{
		bool isNewDown;
		bool isHold;
	};

	KeyState m_map[ RC::GK_COUNT ];
	std::queue< RC::GameKeys > m_queue;
};

#endif // KEYSTATE_H
