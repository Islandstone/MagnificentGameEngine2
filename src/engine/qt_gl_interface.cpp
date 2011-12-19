#include "qt_gl_interface.h"
#include <iostream>
#include <sstream>

#include "input.h"
#include <QDebug>

QtInterface::QtInterface(QWidget *parent) 
    : QGLWidget(parent)
{
//    setAutoBufferSwap(true);

    qApp->installEventFilter(this);
    m_bShouldClose = false;
}


void QtInterface::initializeGL() 
{
    qDebug("GL Init");

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
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void QtInterface::paintGL()
{
    //qDebug("GL Paint");

    // Q
    if ( Input()->KeyHeld( 24 ) )
    {
        m_flRotation -= 1.0f;
    }

    // W
    if ( Input()->KeyHeld( 25 ) )
    {
        m_flRotation += 1.0f;
    }

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1,0,0);

    if ( m_flRotation >= 360.0f )
    {
        m_flRotation -= 360.0f;
    }
    else if ( m_flRotation < 0.0f )
    {
        m_flRotation += 360.0f;
    }

    glLoadIdentity();
    glRotatef(m_flRotation, 0.0f, 0.0f, 1.0f);

    glBegin(GL_POLYGON);

    glVertex2f(0,0);
    glVertex2f(100,500);
    glVertex2f(500, 100);

    glEnd();
}

void QtInterface::keyPressEvent(QKeyEvent *event) 
{
    if ( event->isAutoRepeat() )
    {
        event->accept();
        return;
    }

    int keycode = event->nativeScanCode();

    if ( keycode >= 255 )
    {
        /*
        qDebug() << "The following key was not catched: \n";
        qDebug() << event->text();
        qDebug() << keycode;
        */
        return;
    }
    else
    {
        if ( !event->isAutoRepeat() ) 
        {
            qDebug() << "Key pressed: ";
            qDebug() << event->text();
            qDebug() << keycode;
        }

        Input()->SetKeyDown( keycode );
    }
}

void QtInterface::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat() )
    {
        return;
    }

    int keycode = event->nativeScanCode();

    if ( keycode >= 255 )
    {
        /*
        qDebug() << "The following key was not catched: \n";
        qDebug() << event->text();
        qDebug() << keycode;
        */
        return;
    }
    else
    {
        qDebug() << "Key released: ";
        qDebug() << event->text();
        qDebug() << keycode;

        Input()->SetKeyUp( keycode );
    }
}

void QtInterface::RenderText(int x, int y, std::wstring &str)
{
}
