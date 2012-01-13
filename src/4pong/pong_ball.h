#ifndef _PONG_BALL_HEADER
#define _PONG_BALL_HEADER

#include "physics.h"
#include "base_object.h"


class CPongBall : public CBaseObject
{
public:
	CPongBall();

	String Name() { return String(L"pong_ball"); }

	void Init( float x, float y, float radius );

	void Spawn();

	void Start();

	void Think();

	void Die();

	b2Body* GetBody() { return m_pBall; }
	
private:
	bool m_bActive;
	b2Body *m_pBall;
	b2Body *m_pCenter;

	b2Vec2 m_vInitialPos;

	bool m_bIsDead;
	float m_flCenterActiveTime;

};

#endif