#include "base.h"
#include "engine.h"
#include "qt_gl_interface.h"

#if 0
int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int argc = 0;
	wchar_t** argv = CommandLineToArgvW( GetCommandLineW(), &argc );
	QApplication app(argc, (char**)argv);

    CEngine *pEngine = CEngine::GetInstance();

    pEngine->SetQtApp(&app);

    if ( pEngine->Init() ) 
    {
        pEngine->Start();
    }

    pEngine->Stop();

    return 0;
}

#else

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    CEngine *pEngine = CEngine::GetInstance();

    pEngine->SetQtApp(&app);

    if ( pEngine->Init() ) 
    {
        pEngine->Start();
    }

    pEngine->Stop();

    return 0;
}

#endif