#pragma once

#include <QOpenGLWindow>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QBasicTimer>
#include <QElapsedTimer>
#include <QTime>

//#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "entity.h"

class GLWindow : public QOpenGLWindow, protected QOpenGLExtraFunctions
{
    Q_OBJECT

//    class Scene *scene;
//    entt::entity light;
//    class Scene *skyScene;
//    class Light *lightByScene;
//    glm::mat4x4 projection;

    class World *world;
    Entity light;

    class Camera *camera;
    class Model *modelLight;
    class ShaderProgram *shaderProgram;
    class ShaderProgram *skyShaderProgram;

    GLuint textureId;

    QBasicTimer timer;

    glm::vec2 mousePressPosition;

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    GLfloat zNear = 0.3f;
    GLfloat zFar = 100.0f;
    GLfloat fov = 45.0f;

    const float moveSpeed = 5.0;
    const float rotSpeed = .5;

    QTime m_t0;
    QTime m_t1;

    QElapsedTimer deltaTimer;
    float deltaTime;
    qint64 nanoSec=0;
    long count=0;

    QMap<GLint, GLboolean> keys;
    glm::ivec2 mouseDelta = glm::ivec2(-1,-1);
    glm::ivec2 lastMousePosition = glm::ivec2(-1,-1);
    glm::ivec2 mouseWheel = glm::ivec2(-1,-1);

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

public:
    using QOpenGLWindow::QOpenGLWindow;
    GLWindow();
    ~GLWindow();
};

