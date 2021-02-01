#pragma once

#include <QOpenGLWindow>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QBasicTimer>
#include <QTime>

//#include <shaderprogram.h>

//#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "spacial.h"

class GLWindow : public QOpenGLWindow, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    using QOpenGLWindow::QOpenGLWindow;
    GLWindow();
    ~GLWindow();

//    float col=0.0f;

protected:
    void timerEvent(QTimerEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    bool event(QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *mouseEvent) override;

    void setViewMat();
    void setProjectionMat();

private:

    class Scene *scene;
    class Scene *skyScene;
    class Camera *camera;
    class Light *light;
    class Mesh *mesh;
    class Model *model;
    class ShaderProgram *shaderProgram;
    class ShaderProgram *skyShaderProgram;

    GLuint textureId;

    QBasicTimer timer;

    glm::mat4x4 projection;
    glm::vec2 mousePressPosition;
    glm::vec3 rotationAxis;
    glm::quat rotation;

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    GLfloat zNear = 0.3f;
    GLfloat zFar = 100.0f;
    GLfloat fov = 45.0f;

    QTime m_t0;
    QTime m_t1;

    QMap<GLint, GLboolean> keys;
    glm::ivec2 mouseDelta = glm::ivec2(-1,-1);
    glm::ivec2 lastMousePosition = glm::ivec2(-1,-1);
    glm::ivec2 mouseWheel = glm::ivec2(-1,-1);
};

