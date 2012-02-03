#include "base.h"
#include "engine.h"
#include "qt_gl_interface.h"

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
