#include "glwindow.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

#include <QMouseEvent>

#include <cmath>
#include <assimpload.h>
#include <mesh.h>
#include <texture.h>

#include <glm/gtc/type_ptr.hpp>

#include "shaderprogram.h"
#include "model.h"
#include "camera.h"

GLWindow::GLWindow()
{
    zNear = 0.3f;
    zFar = 100.0f;

    mousePressPosition=glm::vec2(0);
    rotationAxis = glm::vec3(1);
    rotation = glm::quat();

    m_t1 = QTime::currentTime();
    shaderProgram = new ShaderProgram();
    camera = new Camera();
//    camera->getTransform().setPosition(glm::vec3(0.0, 0.0, -5.0) );
//    camera->getTransform().setRotation(glm::vec3(.2,0,0));
    camera->getTransform().moveForward(3.0);
//    camera->getTransform().rotate(glm::vec3(-.2,0,0));
}

GLWindow::~GLWindow()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
//    makeCurrent();
    delete texture;
//    doneCurrent();
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    using namespace glm;

    glClearColor(.05, 0.2, 0.3, 1);

    shaderProgram->initShaders("shader");

    texture = new Texture();

    Model *room = new Model("cube.obj", "brickwall.jpg", "brickwall_normal.jpg");
    room->getTransform().setPosition(vec3( 0.0, 0.0, 0.0));
    room->getTransform().setScale( vec3(30.0, 30.0, 30.0));
    scene.addChild(room);

    Model *bike = new Model("vulture.obj", "brickwall.jpg", "brickwall_normal.jpg");
    bike->getTransform().setPosition(vec3( 0.0, 0.0, 0.0));
    bike->getTransform().setScale( vec3(30.0, 30.0, 30.0));
    scene.addChild(bike);

//    AssimpLoad a("vulture.obj");
//    auto mp = a.sceneMeshRendererDataCache.at("vulture.obj")[0];
//    mesh=mp.mesh;

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

void GLWindow::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    GLfloat aspect = GLfloat(w) / GLfloat(h ? h : 1);

    // Reset projection
    projection = glm::mat4(1);
    //    qprojection.setToIdentity();

    // Set perspective projection
    projection = glm::perspective(fov, aspect, zNear, zFar);
//    projection = QMatrix4x4( glm::value_ptr(gprojection) ).transposed();
}

void GLWindow::paintGL()
{
    //test
    glClearColor(colorBack, colorBack, colorBack, 1);

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind(0);

//    glActiveTexture(GL_TEXTURE0+1);
//    glBindTexture(GL_TEXTURE_2D, textureId);

    setProjectionMat();
    setViewMat();
//    GLint modelTransMat = glGetUniformLocation(shaderProgram->programId(),"model");
//    glm::mat4 modelTempMat = glm::mat4(1);
//    glUniformMatrix4fv(modelTransMat, 1, GL_FALSE, &modelTempMat[0][0]);//&mtm[0][0]);

    // Use texture unit 0 which contains cube.png
//    program.setUniformValue("texture", 0); //Qt Alternative
    GLint texLoc = shaderProgram->uniformLocation("uv");
    glUniform1i(texLoc, 0);
//    mesh->render();
//    model->renderAll();
    scene.renderAll();

//    m_t1 = QTime::currentTime();
//    int curDelta = m_t0.msecsTo(m_t1);
//    m_t0 = m_t1;

//    qDebug()<< curDelta;
}

void GLWindow::setProjectionMat()
{
    const float w = width();
    const float h = height();
    float aspect =w/h;
    glm::mat4 projection = glm::perspective(glm::degrees(fov), aspect, zNear, zFar);
    GLint projectionid = glGetUniformLocation(shaderProgram->programId(), "projection");
    glUniformMatrix4fv(projectionid, 1, GL_FALSE, &projection[0][0]);
}

void GLWindow::setViewMat()
{

    GLint camPos=glGetUniformLocation(shaderProgram->programId(),"viewPos");;
    const glm::vec3 v = camera->getTransform().getPosition();
    glUniform3fv(camPos, 1, &v[0]);

    GLint view = glGetUniformLocation(shaderProgram->programId(),"view");
//    window.input.getKeyModState();

//    if(window.getInput()->getMouseWheel().y)
//        camera->getTransform().moveForward(window.getInput()->getMouseWheel().y);

//    vec2 rotator(window.getInput()->getMouseDelta().y * glm::radians(0.5f), window.getInput()->getMouseDelta().x * glm::radians(0.5f));
//    camera->getTransform().addYawPitch(vec3(rotator,0));

    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera->getTransform().getCameraTransformMatrix()) );//&mtm[0][0]);

//    glm::mat4 vi = camera->getTransform().getCameraTransformMatrix();
//    shaderProgram->setUniformValue("view",
//                             QMatrix4x4(glm::value_ptr(vi)).transposed());
}

void GLWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_R)
    {
        colorBack = 0.5f;
    }
}

void GLWindow::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
//    qmousePressPosition = QVector2D(e->localPos());
    mousePressPosition = glm::vec2(e->localPos().x(), e->localPos().y());
}

void GLWindow::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
//    QVector2D diff = QVector2D(e->localPos()) - qmousePressPosition;
//    QVector2D diff = QVector2D(e->localPos().x()- gmousePressPosition.x, e->localPos().y()- gmousePressPosition.y) ;
    glm::vec2 gdiff = glm::vec2(e->localPos().x(), e->localPos().y())  - mousePressPosition;
//    glm::vec2 diff = glm::vec2(difff.x(), difff.y());
    // Rotation axis is perpendicular to the mouse position difference
    // vector
    glm::vec3 n = glm::normalize( glm::vec3(gdiff.y, gdiff.x, 0.0));
    // Accelerate angular speed relative to the length of the mouse sweep
//    qreal acc = diff.length() / 100.0;
    GLfloat acc = glm::length(gdiff)/100.0f;
    // Calculate new rotation axis as weighted sum
//    qrotationAxis = (qrotationAxis * qangularSpeed + n * acc).normalized();

    rotationAxis = glm::normalize(glm::vec3 (rotationAxis * angularSpeed + n * acc) );

    // Increase angular speed
//    qangularSpeed += acc;
    angularSpeed += acc;
}

void GLWindow::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
//    qangularSpeed *= 0.98;
    angularSpeed *= 0.98;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01)
    {
        angularSpeed = 0.0;
    }
    else
    {
        // Update rotation
//        qrotation = QQuaternion::fromAxisAndAngle(qrotationAxis, gangularSpeed) * qrotation;
        rotation = glm::rotate(rotation, angularSpeed, rotationAxis) * rotation;
//        grotation = glm::quat(-qrotation.z(), qrotation.y(),-qrotation.x(),qrotation.scalar());
        // Request an update
        update();
    }
}
