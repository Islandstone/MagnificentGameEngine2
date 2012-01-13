#ifndef _PONG_DEATHWALL_HEADER
#define _PONG_DEATHWALL_HEADER

#include "physics.h"
#include "base_object.h"

class CPongDeathWall : public CBaseObject
{
public:
	CPongDeathWall();

	String Name() { return String(L"pong_deathwall"); }

	void Init(float x, float y, float width, float height);



private:

	b2Body *m_pBody;
};

#endif