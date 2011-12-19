#ifndef QT_INTERFACE_HEADER

#define QT_INTERFACE_HEADER

#include <QGLWidget>
#include <QApplication>
#include <QKeyEvent>

class QtInterface : public QGLWidget
{
        Q_OBJECT

public:
        QtInterface( QWidget *parent = NULL ); 

        void Render() { updateGL(); }
        bool shouldClose() { return m_bShouldClose; }
protected:

        void closeEvent(QCloseEvent *event) { m_bShouldClose = true; event->accept();/* QGLWidget::close(); */ }
        void initializeGL();
        void resizeGL(int width, int height);
        void paintGL();
        void RenderText(int x, int y, std::wstring &str);
    
        bool m_bShouldClose;

        void keyPressEvent(QKeyEvent *event); 
        void keyReleaseEvent(QKeyEvent *event); 

        float m_flRotation;
};
#endif /* end of include guard: QT_INTERFACE_HEADER */

