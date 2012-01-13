#include "pong_ball.h"

#include "timer.h"
#include "input.h"
#include "script.h"
#include "engine.h"


CPongBall::CPongBall() 
{
	m_pBall = NULL;
	m_bActive = false;

	srand( Timer()->RandomSeed() );
	m_flCenterActiveTime = 0.0f;
}

void CPongBall::Init( float x, float y, float radius )
{
	b2CircleShape circle;
	b2BodyDef def;

	m_vInitialPos.Set(x, y);

	circle.m_radius = radius;
	def.position.Set( x, y ); 
	def.linearDamping = 0.0f;
	def.active = true;
	def.type = b2_dynamicBody;
	def.fixedRotation = false;
	def.userData = this;
	m_pBall = Physics()->World()->CreateBody(&def);

	b2FixtureDef ballDef;
	ballDef.density = 0.8f;
	ballDef.restitution = 1.0f;
	ballDef.friction = 0.8f;
	ballDef.shape = &circle;

	m_pBall->CreateFixture(&ballDef);
	m_bActive = false;
	m_bIsDead = false;

	b2PolygonShape shape;
	b2BodyDef cdef;
	cdef.position.Set(50.0f/Physics()->Scale(), 50.0f / Physics()->Scale() );
	cdef.type = b2_staticBody;
	shape.SetAsBox( 7/Physics()->Scale(), 7/Physics()->Scale() );

	m_pCenter = Physics()->CreateBody(&shape, &cdef);

	m_pCenter->SetActive(false);
}

void CPongBall::Spawn()
{

}

void CPongBall::Start()
{
	float a = b2_pi * 2 * (float)rand()/(float)RAND_MAX;
	b2Vec2 vel( cos(a), sin(a) );
	vel *= 1.0f;

	m_pBall->SetLinearVelocity( vel );
	m_bActive = true;
	m_bIsDead = false;

	m_flCenterActiveTime = Timer()->CurrentTime() + 1.0f;
}

void CPongBall::Think()
{
	if ( m_bIsDead )
	{
		m_pCenter->SetActive( false );
		m_pBall->SetTransform( m_vInitialPos, 0.0f );
		m_pBall->SetLinearVelocity( b2Vec2(0,0) );
		m_pBall->SetAngularVelocity( 0.0f );
		m_bActive = false;
	}

	if ( !m_bActive && Input()->KeyReleased(KEY_SPACE) )
	{
		Start();
	}

	if ( m_bActive && Timer()->CurrentTime() >= m_flCenterActiveTime )
	{
		m_pCenter->SetActive( true );
	}
}

void CPongBall::Die()
{
	/*
	m_pBall->SetTransform( m_vInitialPos, 0.0f );
	m_pBall->SetLinearVelocity( b2Vec2(0,0) );
	*/

	m_bIsDead = true;
}

// ------------------------------------------------------------

void BindPongBall()
{
	Sqrat::Class<CPongBall> def;
	def.Func(L"Init", &CPongBall::Init);

	Sqrat::RootTable().Bind(L"PongBall", def);
}

ClassBinder pongBallBinder(&BindPongBall);