#ifndef KEYSTATE_H
#define KEYSTATE_H

#include <queue>
#include <stack>
#include "enumhash.h"
#include <unordered_map>
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
        else return RC::GK::None;
    }

    inline void qPushKey( const RC::GK gk ) { m_queue.push( gk ); }

    inline void qPopKey()
    {
        if ( m_queue.empty() )
            return;

        processKey( m_queue.front() );

        const RC::GK prevGk = RC::GKPar::prevGK( m_queue.front() );
        if ( prevGk != RC::GK::None )
            m_stack.push( m_queue.front() );

        m_queue.pop();
    }

    inline int qSize() const { return m_queue.size(); }

    // prev stack
    inline void prevClear()
    {
        while ( !m_stack.empty() )
            m_stack.pop();
    }

    inline RC::GK prevPop()
    {
        if ( !m_stack.empty() )
        {
            const RC::GK gk = m_stack.top();
            m_stack.pop();

            return gk;
        }
        return RC::GK::None;
    }

    inline void prevPush( const RC::GK gk )
    {
        const RC::GK prevGk = RC::GKPar::prevGK( gk );
        if ( prevGk != RC::GK::None )
            m_stack.push( gk );
    }

private:
    struct KeyState
    {
        bool isNewDown;
        bool isHold;
    };

    std::unordered_map< RC::GK, KeyState, EnumHash > m_map;
    std::queue< RC::GK > m_queue;
    std::stack< RC::GK > m_stack;   // previous keys
};

#endif // KEYSTATE_H
