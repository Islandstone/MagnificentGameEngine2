#include "physics.h"
#include "qt_gl_interface.h"
#include "engine.h"
#include "input.h"
#include "timer.h"
#include "script.h"

uint32 flagsDraw = GLDebugDraw::e_shapeBit|GLDebugDraw::e_jointBit|/*GLDebugDraw::e_aabbBit|*/GLDebugDraw::e_centerOfMassBit;
GLDebugDraw DebugDraw;

PongCollision pong_collision;

void BindPhysics() 
{
	using namespace Sqrat;

	Class<CPhysicsSystem, NoConstructor> def;
	def.Func("Scale", &CPhysicsSystem::Scale);

	def.Func("CreateBody", &CPhysicsSystem::CreateBody);
	def.Func("CreateWorldOutline", &CPhysicsSystem::CreateWorldOutline);

	RootTable().Bind("PhysicsSystem", def);

	// Push the singleton to squirrel
	sq_pushroottable( Sqrat::DefaultVM::Get() );
	sq_pushstring( Sqrat::DefaultVM::Get(), "PhysicsSystem", -1 );
	sq_get(  Sqrat::DefaultVM::Get(), -2 );
	sq_pushstring(  Sqrat::DefaultVM::Get(), "Physics", -1 );
	sq_createinstance(  Sqrat::DefaultVM::Get(), -2 );
	sq_setinstanceup(  Sqrat::DefaultVM::Get(), -1, (SQUserPointer)Input() );
	sq_newslot(  Sqrat::DefaultVM::Get(), -4, SQFalse );
	sq_pop(  Sqrat::DefaultVM::Get(), 2 );

	// Box2D

	// b2Vec2
	Class<b2Vec2> vec2;
	vec2.Func("Set", &b2Vec2::Set);
	RootTable().Bind("b2Vec2", vec2);

	// b2ChainShape
	Class<b2ChainShape> chain;
	chain.Func("CreateLoop", &b2ChainShape::CreateLoop );
	RootTable().Bind("b2ChainShape", chain);

	// b2CircleShape
    /*
	Class<b2CircleShape> circle;
	circle.Var("radius", &b2CircleShape::m_radius);
	RootTable().Bind("b2CircleShape", circle);
    */

	// b2BodyDef
	Class<b2BodyDef> bodydef;
	bodydef.Var("position", &b2BodyDef::position);
	bodydef.Var("linearDamping", &b2BodyDef::linearDamping);
	bodydef.Var("type", &b2BodyDef::type);
	RootTable().Bind("b2BodyDef", bodydef);

	// b2BodyType
	Enumeration bodyType;
	bodyType.Const("b2_dynamicBody", (int)b2_dynamicBody);
	ConstTable().Enum( "bodyType", bodyType);

	/*
	ConstTable().Enum("b2_staticBody", (int)b2_staticBody);
	ConstTable().Const("b2_kinematicBody", (int)b2_kinematicBody);
	ConstTable().Const("b2_dynamicBody", (int)b2_dynamicBody);
	*/
}

ClassBinder physicsBinder(&BindPhysics);


CPhysicsSystem::CPhysicsSystem() : CGameSystem()
{
	m_pWorld = NULL;
	m_pBall = NULL;
	m_flLastPhysicsUpdate = 0.0f;
}

bool CPhysicsSystem::Init()
{
	// Make a world 
	m_pWorld = new b2World( b2Vec2(0.0f / PHYSICS_SCALE, 0.0f / PHYSICS_SCALE) );
	m_pWorld->SetDebugDraw(&DebugDraw);
	m_pWorld->SetContactListener(&pong_collision);

	/*
	b2ChainShape chain;
	b2BodyDef grounddef;
	b2Body* ground;

	b2Vec2 loop[] = 
	{
		b2Vec2( 1.0f / Scale(), 1.0f / Scale() ),
		b2Vec2( 1.0f / Scale(), 99.0f / Scale() ),
		b2Vec2( 99.0f / Scale(), 99.0f / Scale() ),
		b2Vec2( 99.0f / Scale(), 1.0f / Scale() ),
	};

	chain.CreateLoop(loop, 4);
	grounddef.position.Set(0,0);
	ground = m_pWorld->CreateBody(&grounddef);
	ground->CreateFixture(&chain, 0.0f);
	*/
	
	/*
	b2CircleShape circle;
	b2BodyDef def;

	circle.m_radius = 3.0f / PHYSICS_SCALE;
	def.position.Set( 50.0f / PHYSICS_SCALE, 10.0f / PHYSICS_SCALE ); 
	def.linearDamping = 0.0f;
	def.active = true;
	def.type = b2_dynamicBody;
	def.fixedRotation = false;
	m_pBall = m_pWorld->CreateBody(&def);

	b2FixtureDef ballDef;
	ballDef.density = 0.8f;
	ballDef.restitution = 0.8f;
	ballDef.friction = 0.5f;
	ballDef.shape = &circle;

	m_pBall->CreateFixture(&ballDef);
	*/

	b2PolygonShape shape; b2BodyDef def;
	shape.SetAsBox(3.25f/Scale(), 3.25f/Scale());

	def.position.Set(4.125f/Scale(), 4.125f/Scale());
	CreateBody(&shape, &def);

	def.position.Set( (100.0f - 4.125f)/Scale(), 4.125f/Scale() );
	CreateBody(&shape, &def);

	def.position.Set( (100.0f - 4.125f)/Scale(), (100.0f - 4.125f)/Scale() );
	CreateBody(&shape, &def);

	def.position.Set( 4.125f/Scale(), (100.0f - 4.125f)/Scale() );
	CreateBody(&shape, &def);

	return true;
}

b2Body* CPhysicsSystem::CreateBody(b2Shape *shape, b2BodyDef *def)
{
	b2Body *body = m_pWorld->CreateBody(def);
	body->CreateFixture(shape, 0.0f);
	return body;
}

void CPhysicsSystem::CreateWorldOutline(Sqrat::Object sq_ar, int count)
{
}

void CPhysicsSystem::Destroy()
{
	delete m_pWorld;
}

void CPhysicsSystem::Update()
{
	if ( Timer()->CurrentTime() - m_flLastPhysicsUpdate < 1.0f/70.0f )
		return;

	m_flLastPhysicsUpdate = Timer()->CurrentTime();

	int velocityIterations = 8;
	int positionIterations = 3;

#define FORCE (10.0f/PHYSICS_SCALE)

	if ( m_pBall != NULL)
	{
		if ( Input()->KeyHeld( KEY_RIGHT ) || Input()->KeyHeld( KEY_L ) )
			m_pBall->ApplyForceToCenter( b2Vec2(FORCE, 0.0f ) );

		if ( Input()->KeyHeld( KEY_LEFT ) || Input()->KeyHeld( KEY_H) )
			m_pBall->ApplyForceToCenter( b2Vec2(-FORCE, 0.0f ) );

		if ( Input()->KeyHeld( KEY_UP ) || Input()->KeyHeld( KEY_K) )
			m_pBall->ApplyForceToCenter( b2Vec2(0.0f, -FORCE) );

		if ( Input()->KeyHeld( KEY_DOWN ) || Input()->KeyHeld( KEY_J ) )
			m_pBall->ApplyForceToCenter( b2Vec2(0.0f, FORCE) );
	}

	m_pWorld->Step(1.0f/60.0f, velocityIterations, positionIterations);
}

void CPhysicsSystem::Render()
{
	// Set the flags needed for Drawing
	DebugDraw.SetFlags(flagsDraw);

	Engine()->PushContext();
	// Draw the world

	glPushAttrib( GL_CURRENT_BIT );

	if ( glIsEnabled( GL_TEXTURE_RECTANGLE_NV ) )
	{
		glDisable( GL_TEXTURE_RECTANGLE_NV );
		m_pWorld->DrawDebugData();
		glEnable( GL_TEXTURE_RECTANGLE_NV );
	}
	else if ( glIsEnabled( GL_TEXTURE_RECTANGLE_EXT ) )
	{
		glDisable( GL_TEXTURE_RECTANGLE_EXT );
		m_pWorld->DrawDebugData();
		glEnable( GL_TEXTURE_RECTANGLE_EXT );
	}
	else
	{
		glDisable( GL_TEXTURE_2D );
		m_pWorld->DrawDebugData();
		glEnable( GL_TEXTURE_2D );
	}

	glPopAttrib();
}

// -----------------------------------------------------------------------------------------------
// Pong Collision (TODO: Move)

#include "src/4pong/pong_ball.h"
#include "src/4pong/pong_player.h"
#include "src/4pong/pong_deathwall.h"

PongCollision::PongCollision()
{
}

void PongCollision::BeginContact(b2Contact* contact)
{
	/*
    CPhysicsObject* obj1 = (CPhysicsObject*)point->shape1->GetUserData();
    CPhysicsObject* obj2 = (CPhysicsObject*)point->shape2->GetUserData();
	*/

	CBaseObject *pObj1 = (CBaseObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	CBaseObject *pObj2 = (CBaseObject*)contact->GetFixtureB()->GetBody()->GetUserData();

	if ( pObj1 == NULL || pObj2 == NULL )
		return;

	CPongBall *pBall = NULL;
	CPongPlayer *pPlayer = NULL;
	CPongDeathWall *pWall = NULL;

	if ( pObj1->Name() == "pong_ball" )
	{
		pBall = (CPongBall*)pObj1;
	}	
	else if ( pObj2->Name() == "pong_ball" )
	{
		pBall = (CPongBall*)pObj2;
	}

	if ( pObj1->Name() == "pong_player" )
	{
		pPlayer = (CPongPlayer*)pObj1;
	}
	else if ( pObj2->Name() == "pong_player" )
	{
		pPlayer = (CPongPlayer*)pObj2;
	}

	if ( pObj1->Name() == "pong_deathwall" )
	{
		pWall = (CPongDeathWall*)pObj1;
	}
	else if ( pObj2->Name() == "pong_deathwall" )
	{
		pWall = (CPongDeathWall*)pObj2;
	}

	if ( pWall != NULL && pBall != NULL )
	{
		pBall->Die();
		return;
	}

	if ( pBall == NULL || pPlayer == NULL )
	{
		return;
	}
	
	b2Vec2 dir = pBall->GetBody()->GetPosition() - pPlayer->GetBody()->GetPosition();
	b2Vec2 vel = pBall->GetBody()->GetLinearVelocity();
	b2Vec2 ref;

	if ( pPlayer->IsTopdown() )
	{
		// Top/down
		ref.x = vel.x > 0.0f ? -1.0f : 1.0f;
		ref.y = vel.x > 0.0f ? 1.0f : -1.0f;
	}
	else
	{
		// Left/Right
		ref.x = vel.x > 0.0f ? 1.0f : -1.0f;
		ref.y = vel.y > 0.0f ? -1.0f : 1.0f;
	}


	ref.Normalize();
	ref *= vel.Length();

	pBall->GetBody()->SetLinearVelocity( ref );
}

void PongCollision::EndContact(b2Contact* contact)
{
	

	/*
	// Normal-based reflection calculation
	b2Vec2 vel = pBall->GetBody()->GetLinearVelocity();
	
	b2Vec2 v1 = contact->GetManifold()->localNormal;
	v1 *= 2 * (vel.x * v1.x + vel.y * v1.y);
	
	b2Vec2 ref = vel - v1;
	ref *= vel.Length();

	pBall->GetBody()->SetLinearVelocity( ref );
	*/

	/* Old reference code
    CPhysicsObject* obj1 = (CPhysicsObject*)point->shape1->GetUserData();
    CPhysicsObject* obj2 = (CPhysicsObject*)point->shape2->GetUserData();

    if (obj1 == NULL || obj2 == NULL)
    {
        return;
    }

    CPongBall *pBall = NULL;
    CPongPlayer *pPlayer = NULL;

    if (obj1->GetClassName() == "pong_ball")
    {
        pBall = (CPongBall *)obj1;
    }
    else if (obj1->GetClassName() == "pong_player")
    {
        pPlayer = (CPongPlayer *)obj1;
    }

    if (obj2->GetClassName() == "pong_ball")
    {
        pBall = (CPongBall *)obj2;
    }
    else if (obj2->GetClassName() == "pong_player")
    {
        pPlayer = (CPongPlayer *)obj2;
    }


    if (pBall && !pPlayer)
    {
        if (obj1->GetClassName() == "pong_deathwall" || obj2->GetClassName() == "pong_deathwall")
        {
            pBall->Die();
        }

        return;
    }

    if (pBall && pPlayer)
    {
        b2Vec2 newVel = pBall->m_pBody->GetLinearVelocity();

        b2Vec2 angle = pBall->m_pBody->GetPosition() - pPlayer->m_pBody->GetPosition();
        angle *= 3.0f;
        newVel += angle;

        if ( newVel.Length() > 50.0f )
        {
            newVel.Normalize();
            newVel *= 50.0f;
        }

        pBall->SetLastTouched(pPlayer);
        pBall->m_pBody->SetLinearVelocity( newVel );
    }
	*/
}

void PongCollision::Register(b2World* pWorld)
{
    pWorld->SetContactListener(this);
}

// -----------------------------------------------------------------------------------------------
// DebugDraw
void GLDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glColor4f(color.r, color.g, color.b, 1.0f);

	glBegin(GL_LINE_LOOP);

	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x * PHYSICS_SCALE, vertices[i].y * PHYSICS_SCALE);
	}

	glEnd();
}

void GLDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex3f(vertices[i].x * PHYSICS_SCALE, vertices[i].y * PHYSICS_SCALE, 1.0f);
	}
	glEnd();
	glDisable(GL_BLEND);

	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex3f(vertices[i].x * PHYSICS_SCALE, vertices[i].y * PHYSICS_SCALE, 1.0f);
	}
	glEnd();
}

void GLDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glColor3f(color.r, color.g, color.b);
	glLineWidth(1.0f);
	glBegin(GL_LINE_LOOP);

	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x * PHYSICS_SCALE, v.y * PHYSICS_SCALE);
		theta += k_increment;
	}
	glEnd();
}

void GLDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x * PHYSICS_SCALE, v.y * PHYSICS_SCALE);
		theta += k_increment;
	}
	glEnd();
	glDisable(GL_BLEND);

	theta = 0.0f;
	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x * PHYSICS_SCALE, v.y * PHYSICS_SCALE);
		theta += k_increment;
	}
	glEnd();

	b2Vec2 p = center + radius * axis;
	glBegin(GL_LINES);
	glVertex2f(center.x * PHYSICS_SCALE, center.y * PHYSICS_SCALE);
	glVertex2f(p.x * PHYSICS_SCALE, p.y * PHYSICS_SCALE);
	glEnd();
}

void GLDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(p1.x * PHYSICS_SCALE, p1.y * PHYSICS_SCALE);
	glVertex2f(p2.x * PHYSICS_SCALE, p2.y * PHYSICS_SCALE);
	glEnd();
}

void GLDebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.2f;
	glBegin(GL_LINES);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(p1.x * PHYSICS_SCALE, p1.y * PHYSICS_SCALE);
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	glVertex2f(p2.x * PHYSICS_SCALE, p2.y * PHYSICS_SCALE);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(p1.x * PHYSICS_SCALE, p1.y * PHYSICS_SCALE);
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	glVertex2f(p2.x * PHYSICS_SCALE, p2.y * PHYSICS_SCALE);

	glEnd();
}

void GLDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(p.x * PHYSICS_SCALE, p.y * PHYSICS_SCALE);
	glEnd();
	glPointSize(1.0f);
}

void GLDebugDraw::DrawString(int x, int y, const char *string, ...)
{
	/*
	char buffer[128];

	va_list arg;
	va_start(arg, string);
	vsprintf(buffer, string, arg);
	va_end(arg);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.9f, 0.6f, 0.6f);
	glRasterPos2i(x, y);
	int32 length = (int32)strlen(buffer);
	for (int32 i = 0; i < length; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	*/
}

void GLDebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(aabb->lowerBound.x * PHYSICS_SCALE, aabb->lowerBound.y * PHYSICS_SCALE);
	glVertex2f(aabb->upperBound.x * PHYSICS_SCALE, aabb->lowerBound.y * PHYSICS_SCALE);
	glVertex2f(aabb->upperBound.x * PHYSICS_SCALE, aabb->upperBound.y * PHYSICS_SCALE);
	glVertex2f(aabb->lowerBound.x * PHYSICS_SCALE, aabb->upperBound.y * PHYSICS_SCALE);
	glEnd();
}

