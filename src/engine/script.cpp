#include "script.h"
#include "engine.h"

#include <sstream>

#include "squirrel.h"
#include "sqstdio.h"
#include "sqstdblob.h"
#include "sqrat.h"
#include "sqratimport.h"

void printfunc(HSQUIRRELVM v, const SQChar *s, ...) 
{
	static char temp[2048];

    va_list vl;
	va_start(vl, s);
	vsprintf( temp, s, vl );
	va_end(vl);

	//OutputDebugStringW(temp);
	std::stringstream st; st << temp;
	Engine()->Debug( st.str() );
	//OutputDebugStringW("\n");
} 

/*
void debughook(HSQUIRRELVM vm, SQInteger type, const SQChar *sourcename, SQInteger line, const SQChar *funcname)
{
	std::wstringstream st;
	st << "gvim " << sourcename << L" +" << line;

	std::string s = st.str();

	system(s.c_str());
}
*/

CScriptSystem::CScriptSystem()
{
	
}

void CScriptSystem::AddBinder(ClassBinder *pBinder)
{
	if (pBinder != NULL)
	{
		m_vBinders.push_back(pBinder);
	}
}

bool CScriptSystem::Init()
{
	// Init SQ
	HSQUIRRELVM vm;

	vm = sq_open(1024);
	
	sq_pushroottable(vm);

	sqstd_register_bloblib(vm);
	sqstd_register_iolib(vm);
	//sqrat_register_importlib(vm);

	sq_setprintfunc(vm, printfunc, printfunc);
	//sq_setnativedebughook(vm, debughook);
	sqstd_seterrorhandlers(vm);
	sqstd_printcallstack(vm);

	Sqrat::DefaultVM::Set(vm);

	// Bind classes
	for (int i = 0; i < m_vBinders.size(); i++)
	{
		if (m_vBinders[i] == NULL) 
		{ 
			Engine()->Debug("NULL BINDER!!!\n"); continue; 
		}

		m_vBinders[i]->Bind();
	}

	Sqrat::Script script;

	try {
		script.CompileFile("main.nut");
		script.Run();
	} catch (Sqrat::Exception e) {
		Engine()->Debug("Script error\n"); 
		Engine()->Debug( e.Message().c_str() );
		Engine()->Debug( "\n" );
		return false;
	} catch(std::exception e) {
		Engine()->Debug("Script error\n");
		return false;
	}

	// Compile & exec scripts
	return true;
}

void CScriptSystem::Destroy()
{
	sq_close( Sqrat::DefaultVM().Get() );
}

void CScriptSystem::PostInit()
{
	Sqrat::Function f = Sqrat::RootTable().GetFunction("OnPostInit");
	f.Execute();
}

void CScriptSystem::Update()
{
	Sqrat::Function f = Sqrat::RootTable().GetFunction("OnUpdate");
	f.Execute();
}

void CScriptSystem::Render()
{
	/*
	Sqrat::Function f = Sqrat::RootTable().GetFunction("OnRender");
	f.Execute();
	*/
}

void CScriptSystem::PostRender()
{
}



ClassBinder::ClassBinder(ClassBindingFunc fn)
{
	m_bIsBound = false;
	m_pBinderFunc = fn;

	Script()->AddBinder(this);
}

void ClassBinder::Bind()
{
	if (!m_bIsBound)
	{
		(*m_pBinderFunc)();
		m_bIsBound = true;
	}
}
