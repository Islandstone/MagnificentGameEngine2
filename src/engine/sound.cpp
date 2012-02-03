#include "base.h"
#include "sound.h"
#include "engine.h"
#include "input.h"

#ifdef ENABLE_FMOD

#include "fmod.hpp"

String FMOD_ErrorString(FMOD_RESULT errcode);

#ifdef DEBUG

#define ERRCHECK(x) if (x != FMOD_OK) { Engine()->Error( FMOD_ErrorString(x) ); return false; }
#else
#define ERRCHECK(x) if (x != FMOD_OK) { Engine()->Error( FMOD_ErrorString(x) ); return false; }
#endif

#endif

#include "script.h"

void BindSound()
{
	Sqrat::Class<CSoundSystem, Sqrat::NoConstructor> def;

	def.Func("Play", &CSoundSystem::Play_);
	def.StaticFunc("Get", &CSoundSystem::GetInstance );
	Sqrat::RootTable().Bind("SoundSystem", def);

	// Push the singleton to squirrel
	sq_pushroottable( Sqrat::DefaultVM::Get() );
	sq_pushstring( Sqrat::DefaultVM::Get(), "SoundSystem", -1 );
	sq_get(  Sqrat::DefaultVM::Get(), -2 );
	sq_pushstring(  Sqrat::DefaultVM::Get(), "Sound", -1 );
	sq_createinstance(  Sqrat::DefaultVM::Get(), -2 );
	sq_setinstanceup(  Sqrat::DefaultVM::Get(), -1, (SQUserPointer)Sound() );
	sq_newslot(  Sqrat::DefaultVM::Get(), -4, SQFalse );
	sq_pop(  Sqrat::DefaultVM::Get(), 2 );
}

ClassBinder sampleBinder(&BindSound);

CSoundSystem::CSoundSystem()
{
#ifdef ENABLE_FMOD
    m_pSystem = NULL;
#endif
    m_bHasPlayedTest = false;
}

bool CSoundSystem::Init()
{
/*
* Large portions of this code is based on the initialization code
* provided with the FMOD installation
*/

#ifdef ENABLE_FMOD

    FMOD_RESULT result;
    unsigned int version;
    int numdrivers;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS caps;
    char name[256];

    /*
Create a System object and initialize.
*/
    result = System_Create(&m_pSystem);
    ERRCHECK(result);
    
    result = m_pSystem->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        //printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n", version, FMOD_VERSION);
        
        /*
        wchar_t szStr[255];
        wchar_t *pszStr = &szStr[0];
        _snwprintf_s(pszStr, ARRAYSIZE(szStr), sizeof(szStr), "You are using an old version of FMOD, %08x. This program requires %08x", version, FMOD_VERSION );
        */

        Engine()->Error("Old version of FMOD in use!");
        return false;
    }
    
    result = m_pSystem->getNumDrivers(&numdrivers);
    ERRCHECK(result);

    if (numdrivers == 0)
    {
        result = m_pSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        ERRCHECK(result);
    }
    else
    {
        result = m_pSystem->getDriverCaps(0, &caps, 0, &speakermode);
        ERRCHECK(result);

        result = m_pSystem->setSpeakerMode(speakermode); /* Set the user selected speaker mode. */
        ERRCHECK(result);

        if (caps & FMOD_CAPS_HARDWARE_EMULATED) /* The user has the 'Acceleration' slider set to off! This is really bad for latency!. */
        { /* You might want to warn the user about this. */

            Engine()->Error("Hardware accelerated sound has been disabled.\nTo ensure optimum sound quality, go to Control Panel -> Sound -> Volume(tab) -> Advanced(button) -> Performance(tab)\nand partially or fully enable hardware acceleration");

            result = m_pSystem->setDSPBufferSize(1024, 10);
            ERRCHECK(result);
        }

        result = m_pSystem->getDriverInfo(0, name, 256, 0);
        ERRCHECK(result);

        if (strstr(name, "SigmaTel")) /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit. PCM floating point output seems to solve it. */
        {
            result = m_pSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
            ERRCHECK(result);
        }
    }

	int flags =
#ifdef _DEBUG
			FMOD_INIT_NORMAL | FMOD_INIT_ENABLE_PROFILE;
#else
			FMOD_INIT_NORMAL;
#endif

    result = m_pSystem->init(100, flags, 0);
    
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER) /* Ok, the speaker mode selected isn't supported by this soundcard. Switch it back to stereo... */
    {
        result = m_pSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        ERRCHECK(result);
            
        result = m_pSystem->init(100, flags, 0);/* ... and re-init. */
        ERRCHECK(result);
    }

#endif

    return true;
}

void CSoundSystem::Play(const String filename)
{
#ifdef ENABLE_FMOD
    CSample* pSample = CreateSample(filename);

    if (pSample != NULL)
    {
        pSample->Play();
    }
#endif
}

void CSoundSystem::Play_(char* file)
{
#ifdef ENABLE_FMOD
	std::stringstream st; st << file;

	CSample* pSample = CreateSample(st.str());

    if (pSample != NULL)
    {
        pSample->Play();
    }
#endif
}

CSample* CSoundSystem::GetSampleByName(const String filename)
{
#ifdef ENABLE_FMOD
    for (unsigned int i = 0; i < m_vSamples.size(); i++)
    {
        if ( m_vSamples[i] != NULL && m_vSamples[i]->GetFilename() == filename )
        {
            return m_vSamples[i];
        }
    }
#endif
    return NULL;
}

CSample* CSoundSystem::CreateSample(const String filename)
{
    CSample* pSample = GetSampleByName(filename);
#ifdef ENABLE_FMOD
    if (pSample != NULL)
    {
        return pSample;
    }

    pSample = new CSample(filename);

    m_vSamples.push_back(pSample);
#endif
    return pSample;
}

void CSoundSystem::Update()
{
#ifdef ENABLE_FMOD
    m_pSystem->update();

    if ( Input()->KeyReleased( KEY_0 ) && !m_bHasPlayedTest)
    {
        Play("startup.mp3");
        m_bHasPlayedTest = true;
    }
    
    if ( Input()->KeyHeld( KEY_MINUS )) // -
    {
        FMOD_RESULT res;
        //float curtime = Timer()->CurrentTime();
        //Engine()->Debug("Decreasing volume\n");
        
        FMOD::ChannelGroup *pChannelGroup;
        res = m_pSystem->getMasterChannelGroup(&pChannelGroup);

        if (res != FMOD_OK)
        {
            return;
        }

        float current_volume = 1.0f;
        res = pChannelGroup->getVolume(&current_volume);

        if (res != FMOD_OK)
        {
            return;
        }

        res = pChannelGroup->setVolume( max( current_volume - (0.4f /* * (curtime-m_flLastUpdateTime) */), 0.0f) );

        if (res != FMOD_OK)
        {
            return;
        }
    }
    
    if ( Input()->KeyHeld( KEY_PLUS )) // +
    {
        FMOD_RESULT res;
        //float curtime = Timer()->CurrentTime();

        FMOD::ChannelGroup *pChannelGroup;
        res = m_pSystem->getMasterChannelGroup(&pChannelGroup);

        if (res != FMOD_OK)
        {
            return;
        }

        float current_volume = 1.0f;
        res = pChannelGroup->getVolume(&current_volume);

        if (res != FMOD_OK)
        {
            return;
        }

        res = pChannelGroup->setVolume( min(current_volume + (0.2f /* * (curtime-m_flLastUpdateTime) */), 1.0f) );

        if (res != FMOD_OK)
        {
            return;
        }
    }

    //m_flLastUpdateTime = Timer()->CurrentTime();
#endif
}

void CSoundSystem::Destroy()
{
#ifdef ENABLE_FMOD

    for (unsigned int i = 0; i < m_vSamples.size(); i++)
    {
        //Zap( m_vSamples[i] );
        if ( m_vSamples[i] != NULL )
        {
            delete m_vSamples[i];
            m_vSamples[i] = NULL;
        }
    }

    m_vSamples.clear();

    m_pSystem->release();

#endif
}

CSample::CSample(String filename)
{
    m_sFilename = filename;
#ifdef ENABLE_FMOD
    m_pSound = NULL;
    m_pChannel = NULL;
#endif

    LoadFile();
}

CSample::CSample()
{
#ifdef ENABLE_FMOD
    m_pSound = NULL;
    m_pChannel = NULL;
#endif
}

void CSample::Load(char *file)
{
	std::stringstream st; st << file;
	m_sFilename = st.str();

	LoadFile();
}

CSample::~CSample()
{
#ifdef ENABLE_FMOD
    if (m_pSound != NULL)
    {
        m_pSound->release();
        m_pSound = NULL;
    }
#endif
    m_sFilename.clear();
}

void CSample::LoadFile()
{
#ifdef ENABLE_FMOD
    FMOD_RESULT result = FMOD_OK;
    result = Sound()->FMODSystem()->createStream( m_sFilename.c_str(), FMOD_DEFAULT | FMOD_CREATESTREAM, NULL, &m_pSound);

    if (result != FMOD_OK)
    {
#ifdef DEBUG
        String str = "Failed to load sound file " + m_sFilename + L", Error code was: " + FMOD_ErrorString(result);
        Engine()->Debug(str);
#endif
    }
#endif
}

void CSample::Play(bool paused)
{
#ifdef ENABLE_FMOD
    FMOD_RESULT result;

    if (m_pSound == NULL)
    {
        return;
    }

    result = Sound()->FMODSystem()->playSound(FMOD_CHANNEL_FREE, m_pSound, paused, &m_pChannel);

    if (result != FMOD_OK)
    {
        String str("Failed to load sound file " + m_sFilename);
        Engine()->Error(str);

#ifdef DEBUG
        str.append(", Error code was: " + FMOD_ErrorString(result) );
        Engine()->Debug(String("Failed to load sound file ") );
#endif
    }
#endif
}

void CSample::Stop()
{
#ifdef ENABLE_FMOD
    if (m_pChannel != NULL)
    {
        m_pChannel->stop();
    }
#endif
}

void CSample::Test() 
{
	Engine()->Debug("TEST()");
}
