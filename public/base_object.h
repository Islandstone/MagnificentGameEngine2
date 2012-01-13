#ifndef _BASE_OBJECT_HEADER
#define _BASE_OBJECT_HEADER

#include "base.h"

class IGameObject
{
public:
	virtual void Spawn() = 0;
	virtual void Activate() = 0;

	virtual bool ShouldThink() = 0;
	virtual bool ShouldRender() = 0;

	virtual void Think() = 0;
	virtual void Render() = 0;
};

/*
Base class for game objects
*/

class CBaseObject : public IGameObject
{
public:
	CBaseObject();

	virtual void Spawn() {}
	virtual void Activate() {}

	virtual bool ShouldThink() { return false; }
	virtual bool ShouldRender() { return false; }

	virtual void Think() {}
	virtual void Render() {}

	virtual String Name() { return String(L"base"); }

protected:
private:

	String m_sName;
};

#endif