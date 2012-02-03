#include "pong_player.h"
#include "physics.h"
#include "script.h"
#include "engine.h"
#include "timer.h"
#include "input.h"

CPongPlayer::CPongPlayer() : CBaseObject() 
{
	m_flLastThinkTime = 0.0f;
	m_flMovespeed = 0.0f;
	m_bTopdown = true;
}

void CPongPlayer::Init( bool topdown, float width, float thickness, 
						float x, float y, float movespeed, 
						int key_minus, int key_plus )
{
	m_bTopdown = topdown;

	m_flMovespeed = movespeed;

	m_iKeyMinus = key_minus;
	m_iKeyPlus = key_plus;
		
    b2BodyDef def;
	b2PolygonShape shape;
	shape.SetAsBox( 0.5f*width, 0.5f*thickness, b2Vec2(0.0f, 0.0f), topdown ? b2_pi/2 : 0.0f);
		
	b2Vec2 pos(x, y);
	def.position = pos;
	def.type = b2_dynamicBody;
	def.fixedRotation = true;
	def.userData = this;
		
	m_pBody = Physics()->CreateBody(&shape, &def);		
}

void CPongPlayer::Think()
{
	if ( m_pBody == NULL )
		return;

	float dt = Timer()->CurrentTime() - m_flLastThinkTime;

	float move = 0.0f;

	if ( Input()->KeyHeld( m_iKeyMinus ) )
		move = -m_flMovespeed;
	if ( Input()->KeyHeld( m_iKeyPlus ) )
		move = m_flMovespeed;

	if ( m_bTopdown )
		m_pBody->SetLinearVelocity( b2Vec2( 0.0f, -move ) );
	else
		m_pBody->SetLinearVelocity( b2Vec2( move, 0.0f) );

	m_flLastThinkTime = Timer()->CurrentTime();
}

void BindPongPlayer()
{
	Sqrat::Class<CPongPlayer> def;
	def.Func("Init", &CPongPlayer::Init);

	Sqrat::RootTable().Bind("PongPlayer", def);
}

ClassBinder pongBinder(&BindPongPlayer);
