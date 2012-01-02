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

#include "sprite.h"
#include "script.h"

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
	m_pWindow = NULL;
}

bool CEngine::Init() 
{
    Message(L"Engine Init\n");

	Timer()->Start();

    CreateSystems();

    if ( !InitSystems() ) 
    {
        return false;
    }
    
    Message(L"Engine Init Successful\n");
    return true;
}

void CEngine::CreateSystems()
{
    AddSystem( Input() );
    AddSystem( Sound() );
	AddSystem( Script() );
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

	for (int i = 0; i < m_vecGameSystems.size(); i++)
    {
        m_vecGameSystems[i]->Destroy();
    }
}

void CEngine::Start()
{   
    Message(L"Engine Start\n");

    Main();
}

void CEngine::Main()
{
	InitFormat();

    QtInterface window;
	m_pWindow = &window;
    window.resize(1024, 578);
    window.show();
	window.raise(); // Bring to front

	Script()->PostInit();

    while ( !window.shouldClose() )
    {
        m_pApp->processEvents();
		FrameAdvance();

        if ( Input()->KeyPressed( KEY_ESC ) )
        {
            break;
        }
		else if (Input()->KeyHeld( KEY_ALT ) && ( Input()->KeyReleased( KEY_ENTER ) || Input()->KeyReleased( KEY_RETURN ) ) )
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

void CEngine::PushContext()
{
	if (m_pWindow != NULL)
	{
		if ( m_pWindow->context()->isValid() )
			m_pWindow->makeCurrent();
	}
}

void CEngine::PopContext()
{
	/*
	if (m_pWindow != NULL)
		m_pWindow->doneCurrent();
		*/
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
