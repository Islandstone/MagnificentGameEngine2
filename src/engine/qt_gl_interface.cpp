#include "qt_gl_interface.h"
#include "timer.h"

#include <iostream>
#include <sstream>

#include "input.h"
#include <QDebug>
#include <QGLFormat>

#ifdef _WIN32
#include <gl/GLU.h>
#else
#include <GL/glu.h>
#endif 

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include "engine.h"

/*
char* toAscii(String str)
{
	return const_cast<char*>(QString::fromStdWString(str).toStdString().c_str());
}
*/

int glh_extension_supported( const char *extension )
{
    static const GLubyte *extensions = NULL;
    const GLubyte *start;
    GLubyte *where, *terminator;
    
    // Extension names should not have spaces. 
    where = (GLubyte *) strchr( extension, ' ');
    if ( where || *extension == '\0')
      return 0;
    
    if ( !extensions )
      extensions = glGetString( GL_EXTENSIONS );

    // It takes a bit of care to be fool-proof about parsing the
    // OpenGL extensions string.  Don't be fooled by sub-strings,
    // etc.
    start = extensions;
    for (;;) 
    {
        where = (GLubyte *) strstr( (const char *) start, extension );
        if ( !where )
            break;
        terminator = where + strlen( extension );
        if ( where == start || *(where - 1) == ' ' ) 
        {
            if ( *terminator == ' ' || *terminator == '\0' ) 
            {
                return 1;
            }
        }
        start = terminator;
    }
    return 0;
}

// This has to be called before the creation of the QGLWidget
void InitFormat()
{
    /*
	QGLFormat fmt;

	fmt.setDoubleBuffer(true);
	fmt.setSwapInterval(1);
	fmt.setProfile(QGLFormat::CoreProfile);

	QGLFormat::setDefaultFormat(fmt);
    */
}

ILuint g_iTexture1 = 0;
ILuint g_iTexture2 = 0;

QtInterface::QtInterface(QWidget *parent) 
    : QGLWidget(parent)
{
    qApp->installEventFilter(this);
    m_bShouldClose = false;
	
	m_flLastTime = 0.0f;
	m_iFrameCount = 0;
	m_flFPS = 0.0f;

	if ( !isValid() )
	{
		qDebug("Not a valid OpenGL configuration");
		Engine()->Stop();
		return;
	}
	
    setAutoBufferSwap(true);
}


void QtInterface::initializeGL() 
{
    qDebug("GL Init");

	if ( ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		 iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		 ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) {
			printf("DevIL version is different...exiting!\n");
			Engine()->Stop();
	} 

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer( ILUT_OPENGL );

	// Enable alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glEnable(GL_BLEND);		
	glAlphaFunc(GL_GREATER,0.1);
	glDisable(GL_ALPHA_TEST);

	glDisable( GL_DEPTH_TEST );

    /*
	// By checking this, we can enable some more efficient ways of rendering sprites
	if ( glh_extension_supported("GL_NV_texture_rectangle") )
	{
		qDebug("NV texture rectangle supported!");
		//glEnable( GL_TEXTURE_RECTANGLE_NV );
	}
	else if ( glh_extension_supported("GL_EXT_texture_rectangle") )
	{
		qDebug("EXT texture rectangle supported!");
		//glEnable( GL_TEXTURE_RECTANGLE_EXT );
	}
	else
	{
		qDebug("No texture rectangle support");
	}
    */

	//TODO: Engine()->Precache();

    /*
    int width=1020, height=768;
    glViewport(0,0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    */
    glClearColor(0,0,0,0);
	//glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    setFocusPolicy(Qt::StrongFocus);
    m_flFPS = 0.0f;
    m_flLastTime = 0.0f;
}

void QtInterface::resizeGL(int width, int height)
{
    //qDebug("GL Resize");

    glViewport(0,0, (GLsizei)width, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	float viewwidth = 0.0f;
	float viewheight = 0.0f;

	// delta's from the center of the screen
	float dx = 0.0f;
	float dy = 0.0f;

	if ( width >= height )
	{
		viewwidth = 100 * (float)width/(float)height;
		viewheight = 100;	
		dx = 0.5f * viewwidth - 50.0f;
	}
	else
	{
		viewwidth = 100;
		viewheight = 100 * (float)height/(float)width;
		dy = 0.5f * viewheight - 50.0f;
	}
	
	gluOrtho2D(0, viewwidth, viewheight, 0);

	glTranslatef(dx, dy, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void QtInterface::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	Engine()->Render();

	if ( Input()->KeyPressed( KEY_F5 ) )
	{
#ifdef _WIN32 // TODO: Update DevIl installation on linux to match newest version
		ilutGLScreenie();
#endif
	}

	m_iFrameCount += 1;
	float curtime = Timer()->CurrentTime();
	float dt = curtime - m_flLastTime;

	if ( dt >= 1.0f )
	{
		m_flFPS = (m_iFrameCount / dt);
		m_flLastTime = curtime;
		m_iFrameCount = 0;
	}
	
	if ( m_flFPS != 0.0f ) 
	{
		QString str;
		str.sprintf("%.2f fps (%.5f ms), time: %.2f", m_flFPS, (1.0f/m_flFPS), curtime);

		renderText(1, 10, str);
	}
}

void QtInterface::keyPressEvent(QKeyEvent *event) 
{
    if ( event->isAutoRepeat() )
    {
        return;
    }

	int keycode = event->key();

    /*if ( keycode >= 255 )
    {
        qDebug() << "Not registered!:" << "Input:" << event->text() << "pressed (" << keycode << ")";
    }
    else
	*/
    {
        //qDebug() << "Input: " << event->text() << " pressed (" << keycode << ")";
        Input()->SetKeyDown( keycode );
    }
}

void QtInterface::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat() )
    {
        return;
    }

    int keycode = event->key();

    /*if ( keycode >= 255 )
    {
		qDebug() << "Not registered!:" << "Input:" << event->text() << "released (" << keycode << ")";
    }
    else*/
    {
        //qDebug() << "Input:" << event->text() << "released (" << keycode << ")";
        Input()->SetKeyUp( keycode );
    }
}

void QtInterface::RenderText(int x, int y, std::wstring str)
{
	renderText(x, y, QString::fromStdWString(str) );
}
