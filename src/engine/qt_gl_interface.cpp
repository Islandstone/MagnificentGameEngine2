#include "qt_gl_interface.h"
#include <iostream>
#include <sstream>

#include "input.h"
#include <QDebug>

#ifdef _WIN32
#include <gl\GLU.h>
#endif 

#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>
#include "engine.h"

ILuint g_iTexture1 = 0;
ILuint g_iTexture2 = 0;

QtInterface::QtInterface(QWidget *parent) 
    : QGLWidget(parent)
{
    setAutoBufferSwap(true);

    qApp->installEventFilter(this);
    m_bShouldClose = false;
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

	/*
	ilGenImages(1, &g_iTexture);
	ilBindImage(g_iTexture);

	ilLoadImage("splash.bmp");

	qDebug() >> ilutGLTexImage(0);
	*/

	// TODO: Super-cool function, but we cannot use this in the long run
	g_iTexture1 = ilutGLLoadImage( "splash.bmp" );
	g_iTexture2 = ilutGLLoadImage( "logo.bmp" );

    /*
    int width=1020, height=768;
    glViewport(0,0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    */
    glClearColor(0,0,0,0);
    setFocusPolicy(Qt::StrongFocus);
    //grabKeyboard();
}

void QtInterface::resizeGL(int width, int height)
{
    qDebug("GL Resize");

    glViewport(0,0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void QtInterface::paintGL()
{
    if ( Input()->KeyHeld( VK_W ) )
    {
        m_flRotation -= 1.0f;
    }

    if ( Input()->KeyHeld( VK_Q ) )
    {
        m_flRotation += 1.0f;
    }

    glClear(GL_COLOR_BUFFER_BIT);

    if ( m_flRotation >= 360.0f )
    {
        m_flRotation -= 360.0f;
    }
    else if ( m_flRotation < 0.0f )
    {
        m_flRotation += 360.0f;
    }

	glPushMatrix();
	glLoadIdentity();

	// (300, 300) screen pos
	glTranslatef(300, 300, 0);

	// Rotate around center, which is (256, 256)
	glTranslatef(256, 256, 0);
	glRotatef(m_flRotation, 0.0f, 0.0f, 1.0f);
	glTranslatef(-256, -256, 0);

	glColor3f(1,1,1);

	glBindTexture(GL_TEXTURE_2D, g_iTexture2);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.0, 512.0, 0.0);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(512.0, 512.0, 0.0);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(512.0, 0.0, 0.0);

		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	glPopMatrix();
	
	//RenderText(2,10, String(L"Test"));

	if ( Input()->KeyPressed( VK_f5 ) )
	{
		ilutGLScreenie();
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
