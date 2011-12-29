#include <iostream>
#include <sstream>

#include "engine.h"
#include "qt_gl_interface.h"

#ifndef _WIN32
#include "unistd.h"
#endif

#include "time.h"

#include "input.h"
#include "sound.h"
#include "base_object.h"

#include "timer.h"

#include "squirrel.h"
#include "sqstdio.h"
#include "sqrat.h"
#include "sqratimport.h"

#include "sprite.h"

HSQUIRRELVM vm;

void msleep( float msec )
{
#ifdef _WIN32
	Sleep(msec);
#else
    usleep( msec * 1000 );
#endif
}

std::string wtoa(const std::wstring& wstr)
{
   return (std::string(wstr.begin(), wstr.end()));
}

CEngine::CEngine()
{
}

bool CEngine::Init() 
{
    Message(L"Engine Init\n");

    CreateSystems();

    if ( !InitSystems() ) 
    {
        return false;
    }
	
	Timer()->Start();
    
    Message(L"Engine Init Successful\n");
    return true;
}

void CEngine::CreateSystems()
{
    AddSystem( Input() );
    AddSystem( Sound() );
}

void CEngine::AddSystem( IGameSystem *pSystem )
{
    m_vecGameSystems.push_back(pSystem);
}

bool CEngine::InitSystems()
{
    for (int i = 0; i < m_vecGameSystems.size(); i++)
    {
        if ( m_vecGameSystems[i]->Init() == false)
        {
            return false;
        }
    }

    return true;
}

void CEngine::Destroy()
{
    Message(L"Engine Destroy\n");
}

void CEngine::Start()
{   
    Message(L"Engine Start\n");

    Main();
}

void printfunc(HSQUIRRELVM v, const SQChar *s, ...) 
{ 
	static wchar_t temp[2048];

    va_list vl;
	va_start(vl, s);
	vswprintf( temp, s, vl );
	va_end(vl);

	OutputDebugStringW(temp);
	//OutputDebugStringW(L"\n");
} 

void debughook(HSQUIRRELVM vm, SQInteger type, const SQChar *sourcename, SQInteger line, const SQChar *funcname)
{

}

void CEngine::Main()
{
	InitFormat();

    QtInterface window;
    window.resize(1024, 578);
    window.show();
	window.raise(); // Bring to front

	vm = sq_open(1024);
	Sqrat::DefaultVM::Set(vm);
	sq_setprintfunc(vm, printfunc, printfunc);

	//sq_setnativedebughook(vm, debughook);

	sqstd_seterrorhandlers(vm);
	sqstd_printcallstack(vm);

	sqstd_register_iolib(vm);

	Sqrat::Class<CSprite> spriteDef;

	spriteDef.Func(L"LoadTexture", &CSprite::LoadTexture).Func(L"Think", &CSprite::Think);
	Sqrat::RootTable().Bind(L"Sprite", spriteDef);
	
	Sqrat::Script script;

	try {
		script.CompileFile(L"myscript.nut");
		script.Run();
	} catch(std::exception e) {
		Debug(L"Script error\n");
	}

    while ( !window.shouldClose() )
    {
        m_pApp->processEvents();
		FrameAdvance();

		Sqrat::Function f = Sqrat::RootTable().GetFunction(L"OnFrame");
		f.Execute();

        if ( Input()->KeyPressed( VK_Esc ) )
        {
            break;
        }
		else if (Input()->KeyHeld( VK_Alt ) && ( Input()->KeyReleased( VK_Enter ) || Input()->KeyReleased(VK_Return) ) )
		{
			if ( window.isFullScreen() ) 
			{
				window.showNormal();
			}
			else
			{
				window.showFullScreen();
			}
		}
            
        for (int i = 0; i < m_vecGameSystems.size(); i++)
        {
            m_vecGameSystems[i]->Render();
        }

        window.Render();

        for (int i = 0; i < m_vecGameSystems.size(); i++)
        {
            m_vecGameSystems[i]->PostRender();
        }

        // We don't really need more than 100 fps, so sleep for 0.01 sec
        msleep(0);
    }

	/*
	HSQUIRRELVM v;
	v = sq_open(1024);

	sq_pushroottable(v);
	sq_setprintfunc(v, printfunc, printfunc);

	sqstd_dofile(v, L"myscript.nut", 0, true);

	sq_pushstring(v, L"foo", -1);
	sq_get(v, -2);
	sq_pushroottable(v);
	
	sq_call(v, 1, false, false);
	sq_pop(v, 1);

	sq_pushstring(v, L"bar", -1);
	sq_get(v, -2);
	sq_pushroottable(v);

	sq_call(v, 1, false, false);
	sq_pop(v, 1);

	sq_close(v);
	*/

	sq_close(vm);

    Message(L"Engine Main End\n");
}

void CEngine::FrameAdvance()
{
    Update();
}

void CEngine::Update()
{
    for (int i = 0; i < m_vecGameSystems.size(); i++)
    {
        m_vecGameSystems[i]->Update();
    }

	for (int i = 0; i < m_vecObjectList.size(); i++)
    {
        m_vecObjectList[i]->Think();
    }
}

void CEngine::Render()
{
	for (int i = 0; i < m_vecObjectList.size(); i++)
    {
        m_vecObjectList[i]->Render();
    }
}

void CEngine::Stop()
{
    Message(L"Engine Stop\n");

    Destroy();
}

void CEngine::Message( String msg )
{
#ifdef _WIN32
	OutputDebugStringW( msg.c_str() );
#else
    std::wcerr << msg << std::endl;
#endif
}

void CEngine::Debug( String msg )
{
#ifdef _WIN32
	OutputDebugStringW( msg.c_str() );
#else
    std::wcerr << msg << std::endl;
#endif
}

void CEngine::Error( String msg )
{
    std::wcerr << msg << std::endl;
}


void CEngine::SetFullscreen(bool fullscreen)
{
    
}
