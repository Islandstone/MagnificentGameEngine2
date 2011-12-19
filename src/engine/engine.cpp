#include <iostream>
#include <sstream>

#include "engine.h"
#include "qt_gl_interface.h"

#include "unistd.h"
#include "time.h"

#include "input.h"
#include "sound.h"

void msleep( float msec )
{
    usleep( msec * 1000 );
}

CEngine::CEngine()
{
}

bool CEngine::Init() 
{
    Message(L"Engine Init");

    CreateSystems();

    if ( !InitSystems() ) 
    {
        return false;
    }
    
    Message(L"Engine Init Successful");
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
    Message(L"Engine Destroy");
}

void CEngine::Start()
{   
    Message(L"Engine Start");

    Main();
}

void CEngine::Main()
{
    QtInterface window;
    window.resize(800, 600);
    window.show();

    while ( !window.shouldClose() )
    {
        m_pApp->processEvents();
        FrameAdvance();

        // ESC
        if ( Input()->KeyPressed( 9 ) )
        {
            break;
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
        usleep(1000);
    }

    Message(L"Engine Main End");
}

void CEngine::FrameAdvance()
{
    Update();
    //Render();
}

void CEngine::Update()
{
    for (int i = 0; i < m_vecGameSystems.size(); i++)
    {
        m_vecGameSystems[i]->Update();
    }
}

void CEngine::Stop()
{
    Message(L"Engine Stop");

    Destroy();
}

void CEngine::Message( String msg )
{
    std::wcout << msg << std::endl;
}

void CEngine::Debug( String msg )
{
    std::wcout << msg << std::endl;
}

void CEngine::Error( String msg )
{
    std::wcout << msg << std::endl;
}


void CEngine::SetFullscreen(bool fullscreen)
{
    
}
