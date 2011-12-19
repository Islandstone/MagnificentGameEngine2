#include "base.h"
#include "input.h"

void ZeroFill( char *array, int count )
{
    for (int i = 0; i < count; i++)
    {
        array[i] = 0;
    }
}

CInput::CInput()
{
    m_pCurrentState = m_szKeyBuffer1;
    m_pPreviousState = m_szKeyBuffer2;

    ZeroFill( m_pCurrentState, sizeof(m_szKeyBuffer1) );
    ZeroFill( m_pPreviousState, sizeof(m_szKeyBuffer2) );
}

void CInput::PostRender()
{
    for (int i = 0; i < 255; i++)
    {
        m_pPreviousState[i] = m_pCurrentState[i];
    }
}

void CInput::SetKeyUp(int keycode)
{
    m_pCurrentState[keycode] = 0;
}

void CInput::SetKeyDown(int keycode)
{
    m_pCurrentState[keycode] = 1;
}

bool CInput::KeyUp(int keycode)
{
    return (m_pCurrentState[keycode] == 0) /*&& (m_pPreviousState[keycode] == 0)*/;
}

bool CInput::KeyPressed(int keycode)
{
    return (m_pCurrentState[keycode] == 1) && (m_pPreviousState[keycode] == 0);
}

bool CInput::KeyHeld(int keycode)
{
    return (m_pCurrentState[keycode] == 1) /*&& (m_pPreviousState[keycode] == 0)*/;
}

bool CInput::KeyReleased(int keycode)
{
    return (m_pCurrentState[keycode] == 0) && (m_pPreviousState[keycode] == 1);
}
