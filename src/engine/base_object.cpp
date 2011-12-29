#include "base_object.h"
#include "engine.h"

CBaseObject::CBaseObject()
{
	Engine()->AddObject(this);
}