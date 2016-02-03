#ifndef KEYSTATE_H
#define KEYSTATE_H

class KeyState
{
public:
	KeyState() : m_isNewDown{ 0 }, m_isHold{ 0 }  {}

	inline void keyDown()
	{
		if ( !m_isHold )
		{
			m_isNewDown = true;
			m_isHold = true;
		}
	};
	inline void keyUp() { if ( m_isHold ) m_isHold = false; };
	inline void processKey() { if ( m_isNewDown ) m_isNewDown = false; };
	inline bool isHold() const { return m_isHold; };
	inline bool isNewDown() const { return m_isNewDown; };

private:
    bool m_isNewDown;
    bool m_isHold;
};

#endif // KEYSTATE_H
