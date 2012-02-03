#include "pong_deathwall.h"
#include "pong_ball.h"

void BindDeathWall()
{
	Sqrat::Class<CPongDeathWall> def;

	def.Func("Init", &CPongDeathWall::Init);

	Sqrat::RootTable().Bind("PongDeathWall", def);
}

ClassBinder deathWallBinder(&BindDeathWall);


CPongDeathWall::CPongDeathWall() : CBaseObject()
{
	m_pBody = NULL;
}

void CPongDeathWall::Init(float x, float y, float width, float height)
{
	b2BodyDef def;
	def.position.Set(x, y);
	def.userData = this;
	
	b2PolygonShape shape;
	shape.SetAsBox(0.5f*width, 0.5f*height);

	m_pBody = Physics()->World()->CreateBody(&def);
	
	b2FixtureDef fdef;
	fdef.shape = &shape;
	fdef.isSensor = true;
	fdef.density = 0.0f;
	
	m_pBody->CreateFixture(&fdef);
}


