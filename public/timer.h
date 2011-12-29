#ifndef TIMER_H
#define TIMER_H

class CTimer
{
public:
    // TODO: What on earth was "locked" supposed to do?
    CTimer( /*bool locked = false*/ );

    // This is only a singleton for the global timer.
    // User code may create CTimer objects as they wish if neccessary
    static CTimer* GlobalTimer()
    {
        static CTimer globalTimer;
        return &globalTimer;
    }

    float CurrentTime();

    virtual void Start();
    virtual void Reset();
    virtual void Restart();

    bool IsStarted() { return m_bStarted; }

protected:

    bool m_bStarted;

private:

#ifdef _WIN32
	__int64 GetPerformanceCount();
	__int64 GetFrequency();

	__int64 m_iStart;
	__int64 m_iClockFrequency;
#endif 

	float m_flStartTime;
};

extern inline CTimer* Timer()
{
    return CTimer::GlobalTimer();
}

// TODO: Rewrite this a tiny bit to support only in or out fade
// Utility class to create in/out fades.
class CFadeTimer : public CTimer
{
public:

    CFadeTimer(float total_duration = 5.0f, float fade = 1.5f);
    
    void Start();
    float GetModifier();

    bool IsFinished() { return CurrentTime() >= m_flDuration; }

protected:
private:

    float m_flFadeStart;
    float m_flFadeEnd;
    float m_flDuration;
    float m_flFade;

    float m_flEndTime;
    bool m_bSecondStage;
};

#endif // TIMER_H
