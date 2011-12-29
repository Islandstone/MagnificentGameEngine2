#ifndef _SCRIPT_HEADER_INCLUDE
#define _SCRIPT_HEADER_INCLUDE

#include "game_systems.h"

class CScriptSystem : public CGameSystem
{
public:
	static CScriptSystem* GetInstance()
    {
        static CScriptSystem scriptsys;
        return &scriptsys;
    }

	CScriptSystem();

	bool Init();
	void Destroy();

	void Update();
	void Render();

	void PostRender();

	const String GetName() { return String(L"Script"); }

protected:
private:

};

extern inline CScriptSystem* Script()
{
	return CScriptSystem::GetInstance();
}

#endif