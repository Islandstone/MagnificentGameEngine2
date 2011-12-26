#include "base.h"
#include "input.h"
#include "engine.h"

#include <sstream>

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
	int key = keycode;
	if (key >= 255) 
	{
		key -= 0x01000000;
		if (key >= 255) { return; }
	}

	/*
	std::wstringstream st;
	st << "Key " << key << " released" << std::endl;
	Engine()->Debug( st.str() );
	*/

    m_pCurrentState[key] = 0;
}

void CInput::SetKeyDown(int keycode)
{
	int key = keycode;
	if (key >= 255) 
	{
		key -= 0x01000000;
		if (key >= 255) { return; }
	}

	/*
	std::wstringstream st;
	st << "Key " << key << " pressed" << std::endl;
	Engine()->Debug( st.str() );
	*/
	
    m_pCurrentState[key] = 1;
}

bool CInput::KeyUp(int keycode)
{
	int key = keycode;
	if (key >= 255) 
	{
		key -= 0x01000000;
		if (key >= 255) { return false; }
	}

    return (m_pCurrentState[key] == 0) /*&& (m_pPreviousState[keycode] == 0)*/;
}

bool CInput::KeyPressed(int keycode)
{
	int key = keycode;
	if (key >= 255) 
	{
		key -= 0x01000000;
		if (key >= 255) { return false; }
	}

    return (m_pCurrentState[key] == 1) && (m_pPreviousState[key] == 0);
}

bool CInput::KeyHeld(int keycode)
{
	int key = keycode;
	if (key >= 255) 
	{
		key -= 0x01000000;
		if (key >= 255) { return false; }
	}

    return (m_pCurrentState[key] == 1) && (m_pPreviousState[key] == 1);
}

bool CInput::KeyReleased(int keycode)
{
	int key = keycode;
	if (key >= 255) 
	{
		key -= 0x01000000;
		if (key >= 255) { return false; }
	}

    return (m_pCurrentState[key] == 0) && (m_pPreviousState[key] == 1);
}
