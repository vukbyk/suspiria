#pragma once

#include <QtOpenGL/QOpenGLWindow>
#include <QtOpenGLWidgets/QOpenGLWidget>
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

    class World *world;
    class ShaderProgram *shaderProgramMain;
    class ShaderProgram *shaderProgramPBR;
    class ShaderProgram *shaderProgramSky;
    class ShaderProgram *shaderProgramFBScr;

    GLuint framebuffer;
    GLuint textureColorbuffer;
    GLuint rbo;

//    class Camera *camera;
    Entity light;
    Entity camera;
    Entity skyCube;
    Entity *controlledEntity=nullptr;
    TransformComp *controlledTransform=nullptr;

    QBasicTimer timer;

    glm::vec2 mousePressPosition;
    btVector3 camerEulerYP;

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    GLfloat zNear = 0.3f;
    GLfloat zFar = 100.0f;
    GLfloat fov = 45.0f;
    GLfloat aspectFowMultiplayer = 1;
    GLfloat aspect;

    const float moveSpeed = 2.0;
    const float rotSpeed = .2;

    QTime m_t0;
    QTime m_t1;

    QElapsedTimer deltaTimer;
    float deltaTime;
    qint64 nanoSec=0;
    long count=0;

    std::map<GLuint, GLboolean> keys;//or maybe GLint
    glm::ivec2 mouseDelta = glm::ivec2(-1,-1);
    glm::ivec2 lastMousePosition = glm::ivec2(-1,-1);
    glm::ivec2 mouseWheel = glm::ivec2(-1,-1);

    glm::mat4 projectionMat;

protected:
    void timerEvent(QTimerEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    bool isInCameraFrustumAndDistance(TransformComp &cameraTransformComp, TransformComp &actor);

    bool event(QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *mouseEvent) override;
    Frustum createFrustumFromCamera(const Transform& cam, GLfloat nearOffset=0, GLfloat farOffset=0);

public:
    using QOpenGLWindow::QOpenGLWindow;
    GLWindow();
    ~GLWindow();

    void setAspectFowMultiplayer();
};

