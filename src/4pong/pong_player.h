#ifndef _PONG_PLAYER_HEADER
#define _PONG_PLAYER_HEADER

#include "base_object.h"
#include "physics.h"

class CPongPlayer : public CBaseObject
{
public:
	CPongPlayer();

	String Name() { return String("pong_player"); }

    void Init( bool topdown, float width, float thickness, 
                 float x, float y, float movespeed, 
                 int key_minus, int key_plus );

	void Think();

	b2Body* GetBody() { return m_pBody; }

	bool IsTopdown() { return m_bTopdown; }

private:    

	bool m_bTopdown;
	float m_flLastThinkTime;
	float m_flMovespeed;
	int m_iKeyMinus; // Left, down
	int m_iKeyPlus; // right, up
	
    b2Body *m_pBody;
};

#endif
