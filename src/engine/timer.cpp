#include "base.h"

#include "timer.h"
#include "script.h"

#ifdef _WIN32

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

#else

#error NOT UPDATED FOR UNIX!

#endif

void BindTimer()
{
	Sqrat::Class<CTimer, Sqrat::NoConstructor> def;

	//def.Func(L"Start", &CTimer::Start
	def.Func(L"CurrentTime", &CTimer::CurrentTime);

	Sqrat::RootTable().Bind(L"TimerSystem", def);

	// Push the singleton to squirrel
	sq_pushroottable( Sqrat::DefaultVM::Get() );
	sq_pushstring( Sqrat::DefaultVM::Get(), L"TimerSystem", -1 );
	sq_get(  Sqrat::DefaultVM::Get(), -2 );
	sq_pushstring(  Sqrat::DefaultVM::Get(), L"Timer", -1 );
	sq_createinstance(  Sqrat::DefaultVM::Get(), -2 );
	sq_setinstanceup(  Sqrat::DefaultVM::Get(), -1, (SQUserPointer)Timer() );
	sq_newslot(  Sqrat::DefaultVM::Get(), -4, SQFalse );
	sq_pop(  Sqrat::DefaultVM::Get(), 2 );
}

ClassBinder timerBinder(&BindTimer);

CTimer::CTimer()
{	
	m_iStart = 0;
    m_flStartTime = 0;
    m_iClockFrequency = -1;
}

#ifdef _WIN32
__int64 CTimer::GetPerformanceCount()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li.QuadPart;
}

__int64 CTimer::GetFrequency()
{
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    return li.QuadPart;
}
#endif

void CTimer::Start()
{
#ifdef _WIN32
    m_iStart = CTimer::GetPerformanceCount();
    m_iClockFrequency = CTimer::GetFrequency();
#else

#endif

	m_bStarted = true;
	m_flStartTime = CurrentTime();
}

float CTimer::CurrentTime()
{
#ifdef _WIN32
    if (m_iClockFrequency == -1)
    {
        return 0.0f;
    }

    return ((float)(CTimer::GetPerformanceCount() - m_iStart))/(float)m_iClockFrequency;
#else
	return 0.0f;
#endif
}

void CTimer::Reset()
{
	m_bStarted = false;
    m_iStart = 0;
    m_iClockFrequency = -1;
}

void CTimer::Restart()
{
    Reset();
	Start();
}

CFadeTimer::CFadeTimer(float total_duration /* = 5.0f */, float fade /* = 1.5f */) : CTimer()
{
    m_flDuration = total_duration;

    if ( 2.0f*fade > m_flDuration )
    {
        m_flFade = 0.5f * m_flDuration;
    }
    else
    {
        m_flFade = fade;
    }

    m_flEndTime = 0.0f;
}

void CFadeTimer::Start()
{
    CTimer::Start();

    m_flFadeStart = CurrentTime();
    m_flFadeEnd = m_flFadeStart + m_flFade;

    m_bSecondStage = false;
}

float CFadeTimer::GetModifier()
{
    float current_time = CurrentTime();

    if (m_bSecondStage)
    {
        // Still at the peak
        if (current_time <= m_flFadeStart)
        {
            return 1.0f;
        }

        // Fade out
        float retval = 1.0f-(current_time - m_flFadeStart)/m_flFade;

        if (retval > 1.0f)
        {
            return 1.0f;
        }
        else if (retval < 0.0f)
        {
            return 0.0f;
        }

        return retval;
    }
    else /*if (current_time <= m_flFadeEnd)*/
    {
        // Fade in
        float retval = (current_time - m_flFadeStart)/m_flFade;

        if (retval >= 1.0f)
        {
            // Transition from fading in to fade out
            m_bSecondStage = true;
            m_flFadeStart = m_flDuration - m_flFade;

            return 1.0f;
        }

        return retval;
    }

    return 1.0f;
}