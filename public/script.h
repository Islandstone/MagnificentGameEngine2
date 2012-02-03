#ifndef _SCRIPT_HEADER_INCLUDE
#define _SCRIPT_HEADER_INCLUDE

#include "game_systems.h"
#include "squirrel.h"
#include "sqrat.h"
#include <vector>

class ClassBinder;

class CScriptSystem : public CGameSystem
{
public:
	static CScriptSystem* GetInstance()
    {
        static CScriptSystem scriptsys;
        return &scriptsys;
    }

	CScriptSystem();

	// If this proves too shady just move it into a separate class
	void AddBinder(ClassBinder *pBinder);

	bool Init();
	void PostInit();
	void Destroy();

	void Update();

	// TODO: Pre-frame callback?
	void Render();
	void PostRender();

	const String GetName() { return String("Script"); }

protected:
private:

	std::vector<ClassBinder*> m_vBinders;
};

extern inline CScriptSystem* Script()
{
	return CScriptSystem::GetInstance();
}


typedef void (*ClassBindingFunc)(void);

class ClassBinder
{
public:
	ClassBinder(ClassBindingFunc fn);

	void Bind();
	
private:
	bool m_bIsBound;
	ClassBindingFunc m_pBinderFunc;
};

/*
Macro Madness
*/

#define BEGIN_SCRIPTBIND( class_name, script_name ) \
	void BindClass##class_name(); \
	ClassBinder class_name##Binder(&BindClass##class_name); \
	void BindClass##class_name(){ \
	const SQChar *scriptname = L#script_name; \
	Sqrat::Class<##class_name> def; 

#define BIND_FUNC( script_name, class_func ) def.Func("##script_name", &##class_func );

#define END_SCRIPTBIND() Sqrat::RootTable().Bind(scriptname, def); } 

#endif
