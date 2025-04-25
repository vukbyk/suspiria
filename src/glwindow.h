#pragma once

#include <QtOpenGL/QOpenGLWindow>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QBasicTimer>
#include <QElapsedTimer>
#include <QTime>

#include <future>
//#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "entity.h"

class GLWindow : public QOpenGLWindow, protected QOpenGLExtraFunctions
{
    Q_OBJECT

    class Scene *sceneWorld;

    std::vector<entt::entity> visibleEntities;
    std::vector<entt::entity> nextVisibleEntities;
    std::future<void> visibleEntitiesFuture;
    bool visibleEntitiesReady = true;

    GLuint framebuffer;
    GLuint textureColorbuffer;
    GLuint rbo;

    GLfloat DPIScaleFactor=1.0f;
    GLfloat const UPScale=1.0f;

    GLuint captureFBO;
    GLuint captureRBO;

    GLuint irradianceMap;

    const GLuint widthShadow=1024*2, heightShadow=1024*2;

    GLuint depthMapFBO;
    GLuint depthMapTexture;

    glm::mat4 orthoLightProjection;

    GLuint quadVAO, quadVBO;
    Entity camera;
    Entity light;
    Entity *controlledEntity=nullptr;

    TransformComp *controlledTransform=nullptr;

    glm::mat4 projectionMat;


    glm::vec2 mousePressPosition;

    class FPSEulerComponent *eulerYP;

    GLfloat aspectFowMult = 1;

    const float moveSpeed = 2.0;
    const float rotSpeed = .2;

    QBasicTimer timer;
    QTime m_t0;
    QTime m_t1;
    QElapsedTimer deltaTimer;
    float deltaTime;
    qint64 nanoSec=0;
    long count=0;
    std::map<GLuint, GLboolean> keys;
    glm::ivec2 mouseDelta = glm::ivec2(-1,-1);
    glm::ivec2 lastMousePosition = glm::ivec2(-1,-1);
    glm::ivec2 mouseWheel = glm::ivec2(-1,-1);

    void initAndResizeBuffer();
    void updateVisibleEntities(const glm::mat4& viewProj);

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

public:
    using QOpenGLWindow::QOpenGLWindow;
    GLWindow();
    ~GLWindow();
    void setAspectFowMult();
    bool isInFrustumClipSpace(const btVector3& worldPos, float radius, const glm::mat4& projection);
};
