#ifndef _PHYSICS_INCLUDE
#define _PHYSICS_INCLUDE

#include "game_systems.h"
#include "Box2D\Box2D.h"
#include "script.h"

#define PHYSICS_SCALE (30.0f)

class CPhysicsSystem : public CGameSystem
{
public:
	CPhysicsSystem();

	static CPhysicsSystem* GetInstance()
	{
		static CPhysicsSystem physics;
		return &physics;
	}

	// Game System
	bool Init();
	void Update();
	void Render();
	void Destroy();

	b2World* World() { return m_pWorld; }

	const String GetName() { return String(L"Physics"); }

	// Physics stuff
	inline float Scale() { return PHYSICS_SCALE; }
	b2Body* CreateBody(b2Shape *shape, b2BodyDef *def);
	void CreateWorldOutline(Sqrat::Object sq_array, int count);

protected:


private:
	float m_flLastPhysicsUpdate;
	b2World *m_pWorld;
	b2Body *m_pBall;
};

extern inline CPhysicsSystem* Physics()
{
	return CPhysicsSystem::GetInstance();
}

class PongCollision : public b2ContactListener
{
public:
    PongCollision();

    // Called when two fixtures begin to touch.
	void BeginContact(b2Contact* contact);

	// Called when two fixtures cease to touch.
	void EndContact(b2Contact* contact);

    void Register(b2World* pWorld);
};

extern PongCollision pong_collision;

// This class implements debug drawing callbacks that are invoked
// inside b2World::Step.
class GLDebugDraw : public b2Draw
{
public:
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	void DrawTransform(const b2Transform& xf);

    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

    void DrawString(int x, int y, const char* string, ...); 

    void DrawAABB(b2AABB* aabb, const b2Color& color);
};

#endif