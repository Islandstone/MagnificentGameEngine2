#include "base.h"
#include "input.h"
#include "engine.h"

#include <sstream>

#include "script.h"

#define MAKE_KEY( name ) Sqrat::ConstTable().Const(#name, name);

void BindInput()
{
	Sqrat::Class<CInput, Sqrat::NoConstructor> def;

	def.Func("KeyUp", &CInput::KeyUp);
	def.Func("KeyPressed", &CInput::KeyPressed);
	def.Func("KeyHeld", &CInput::KeyHeld);
	def.Func("KeyReleased", &CInput::KeyReleased);
		
	Sqrat::RootTable().Bind("InputSystem", def);
	
	//Sqrat::ConstTable().Const("KEY_LEFT", KEY_LEFT);
	MAKE_KEY( KEY_LEFT );
	MAKE_KEY( KEY_RIGHT );
	MAKE_KEY( KEY_UP );
	MAKE_KEY( KEY_DOWN );
	MAKE_KEY( KEY_Q );
	MAKE_KEY( KEY_W );
	MAKE_KEY( KEY_PAGEUP );
	MAKE_KEY( KEY_PAGEDOWN );
	MAKE_KEY( KEY_HOME );

	// Push the singleton to squirrel
	sq_pushroottable( Sqrat::DefaultVM::Get() );
	sq_pushstring( Sqrat::DefaultVM::Get(), "InputSystem", -1 );
	sq_get(  Sqrat::DefaultVM::Get(), -2 );
	sq_pushstring(  Sqrat::DefaultVM::Get(), "Input", -1 );
	sq_createinstance(  Sqrat::DefaultVM::Get(), -2 );
	sq_setinstanceup(  Sqrat::DefaultVM::Get(), -1, (SQUserPointer)Input() );
	sq_newslot(  Sqrat::DefaultVM::Get(), -4, SQFalse );
	sq_pop(  Sqrat::DefaultVM::Get(), 2 );
}

ClassBinder inputBinder(&BindInput);

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
