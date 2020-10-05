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
#include <QBasicTimer>

#include <shaderprogram.h>

//#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

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

//    void initShaders();

    void keyPressEvent(QKeyEvent *event) override;

private:

    class Mesh *mesh;

    QBasicTimer timer;

    ShaderProgram program;
    GLuint textureId;
//    QOpenGLTexture *texture = nullptr;
    class Texture *texture = nullptr;

    glm::mat4x4 projection;
    glm::vec2 mousePressPosition;
    glm::vec3 rotationAxis;
    glm::quat rotation;

    GLfloat gangularSpeed = 0;

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
