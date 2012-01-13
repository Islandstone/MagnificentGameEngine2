#ifndef QT_INTERFACE_HEADER

#define QT_INTERFACE_HEADER

#include <QGLWidget>
#include <QApplication>
#include <QKeyEvent>

#include "sprite.h"

#define GL_TEXTURE_RECTANGLE_NV 0x84F5
#define GL_TEXTURE_RECTANGLE_EXT GL_TEXTURE_RECTANGLE_NV

extern void InitFormat();

class QtInterface : public QGLWidget
{
        Q_OBJECT

public:
        QtInterface( QWidget *parent = NULL ); 

        void Render() { updateGL(); }
        bool shouldClose() { return m_bShouldClose; }
		void RenderText(int x, int y, std::wstring str);

protected:

        void closeEvent(QCloseEvent *event) { m_bShouldClose = true; event->accept();/* QGLWidget::close(); */ }
        void initializeGL();
        void resizeGL(int width, int height);
        void paintGL();

        bool m_bShouldClose;

        void keyPressEvent(QKeyEvent *event); 
        void keyReleaseEvent(QKeyEvent *event); 

		int m_iFrameCount;
		float m_flLastTime;
		float m_flFPS;
};
#endif /* end of include guard: QT_INTERFACE_HEADER */

