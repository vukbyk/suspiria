#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWindow>
#include <QMatrix4x4>
#include <QVector3D>

#include <QOpenGLFunctions>

QT_BEGIN_NAMESPACE

class QOpenGLTexture;
class QOpenGLShaderProgram;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;

QT_END_NAMESPACE


#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>


class GLWindow : public QOpenGLWindow, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    using QOpenGLWindow::QOpenGLWindow;
    GLWindow();
    ~GLWindow();

//    float col=0.0f;

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();

    void keyPressEvent(QKeyEvent *event) override;

private:

    class Mesh *mesh;

    QBasicTimer timer;

    QOpenGLShaderProgram program;
    GLuint textureId;
//    QOpenGLTexture *texture = nullptr;
    class Texture *texture = nullptr;


    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;

    GLfloat colorBack = 0.0f;

};


#endif


////Minimal
//#include <QOpenGLWindow>
//#include <QOpenGLExtraFunctions>

//class GeometryEngine;

//class GLWindow : public QOpenGLWindow, protected QOpenGLExtraFunctions
//{
//    Q_OBJECT

//public:
//    using QOpenGLWindow::QOpenGLWindow;
//    ~GLWindow();

//protected:

//    void initializeGL() override;
//    void paintGL() override;

//private:
//};

//#endif // MAINWINDOW_H
