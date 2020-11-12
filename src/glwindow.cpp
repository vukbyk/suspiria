#include "glwindow.h"

//#include <QOpenGLShaderProgram>
//#include <QOpenGLBuffer>
//#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QMouseEvent>

#include <qlogging.h>
#include <qtimer.h>

#include <cmath>
#include <assimpload.h>
#include <mesh.h>
#include <texture.h>

#include <glm/gtc/type_ptr.hpp>


#include "shaderprogram.h"
#include "scene.h"
#include "model.h"
#include "camera.h"
#include "light.h"


GLWindow::GLWindow()
{
//    QTimer *refereshTimer = new QTimer(this);
//    connect(refereshTimer, SIGNAL(timeout()), this, SLOT(update()));
//    refereshTimer->start(1);

    // Use QBasicTimer because its faster than QTimer
    timer.start(1, this);

    zNear = 0.3f;
    zFar = 100.0f;

    mousePressPosition=glm::vec2(0);
    rotationAxis = glm::vec3(1);
    rotation = glm::quat();

    m_t1 = QTime::currentTime();
    shaderProgram = new ShaderProgram();
    scene = new Scene(shaderProgram);
    camera = new Camera();
    light = new Light(shaderProgram);
    light->getTransform().setPosition(glm::vec3(.0, .0, .0));
    scene->addChild(light);
//    camera->getTransform().setPosition(glm::vec3(0.0, 0.0, -5.0) );
//    camera->getTransform().setRotation(glm::vec3(.2, 0, 0));
//    qDebug( "cam position %f", camera->getTransform().getPosition().z);
//    camera->getTransform().moveForward(3.0);
}

GLWindow::~GLWindow()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
//    makeCurrent();
//    doneCurrent();
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shaderProgram->initShaders("tutorial");
    scene->setModel( glGetUniformLocation(shaderProgram->programId(),"model") );

    Model *room = new Model("cube.obj", "brickwall.jpg", "brickwall_normal.jpg");
//    Model *room = new Model("cube.obj", "white.png");
    room->getTransform().setPosition( glm::vec3(0.0f, 0.0f, -20.0f) );
    room->getTransform().setScale( glm::vec3(12.0f, 12.0f, 12.0f) );
    scene->addChild(room);

    Model *exo = new Model("cube.obj", "exoalbedo.jpg", "exoskelet_Exoskelet_Normal.png");
//    Model *room = new Model("cube.obj", "white.png");
    exo->getTransform().setPosition( glm::vec3(0.0f, 0.0f, -2.0f) );
    exo->getTransform().setScale( glm::vec3(2.0f, 2.0f, 2.0f) );
    scene->addChild(exo);


//    Model *sphare = new Model("sphare.obj", "white.png");//, "brickwall_normal.jpg");
//    sphare->getTransform().setPosition( glm::vec3(0.0f, 0.0f, -20.0f) );
//    sphare->getTransform().setScale( glm::vec3(12.0f, 12.0f, 12.0f) );
//    scene->addChild(sphare);

    Model *bike = new Model("vulture.obj", "vulture.png", "Vulture_Diffuse.alpha_normal.jpg");
    bike->getTransform().setPosition( glm::vec3(5.0f, 0.0f, 0.0f) );
    bike->getTransform().setScale( glm::vec3(1.0f, 1.0f, 1.0f) );
//    bike->getTransform().rotate( glm::vec3(glm::radians(45.0f),
//                                           glm::radians(45.0f),
//                                           glm::radians(45.0f)));
    scene->addChild(bike);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    // Use texture unit 0 which contains cube.png
//    program.setUniformValue("texture", 0); //Qt Alternative
//    GLint texLoc = shaderProgram->uniformLocation("uv");
}

void GLWindow::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    GLfloat aspect = GLfloat(w) / GLfloat(h ? h : 1);

    // Reset projection
    projection = glm::mat4(1.0f);

    // Set perspective projection
    projection = glm::perspective(fov, aspect, zNear, zFar);
}

void GLWindow::paintGL()
{


    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    // create floating point color buffer
//    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
//    glGenTextures(1, &colorBuffer);
//    glBindTexture(GL_TEXTURE_2D, colorBuffer);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width(), height(), 0, GL_RGBA, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // create depth buffer (renderbuffer)
//    glGenRenderbuffers(1, &rboDepth);
//    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width(), height() );
//    // attach buffers
//    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//        qDebug() << "Framebuffer not complete!";
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);

//    texture->bind(0);
//    glActiveTexture(GL_TEXTURE0+1);
//    glBindTexture(GL_TEXTURE_2D, textureId);

    setProjectionMat();
    setViewMat();
    scene->renderAll();

//    m_t1 = QTime::currentTime();
//    int curDelta = m_t0.msecsTo(m_t1);
//    m_t0 = m_t1;
//    qDebug()<< curDelta;
}

void GLWindow::timerEvent(QTimerEvent *)
{
    if(keys[Qt::Key_W])
        camera->getTransform().moveForward(-0.05f);
    if(keys[Qt::Key_S])
        camera->getTransform().moveForward(0.05f);
    if(keys[Qt::Key_D])
        camera->getTransform().moveRight(-0.05f);
    if(keys[Qt::Key_A])
        camera->getTransform().moveRight(0.05f);
    if(keys[Qt::Key_E])
        camera->getTransform().moveUp(-0.05);
    if(keys[Qt::Key_Q])
        camera->getTransform().moveUp(0.05);

    if(keys[Qt::Key_Right])
        camera->getTransform().rotate(glm::vec3(0,.2f,0));
    if(keys[Qt::Key_Left])
        camera->getTransform().rotate(glm::vec3(0,-.2f,0));
    if(keys[Qt::Key_Up])
        camera->getTransform().addYawPitch(glm::vec3(.2f,0,0));
    if(keys[Qt::Key_Down])
        camera->getTransform().addYawPitch(glm::vec3(-.2f,0,0));

    if(keys[Qt::Key_T])
        light->getTransform().moveForward(0.05f);
    if(keys[Qt::Key_G])
        light->getTransform().moveForward(-0.05f);
    if(keys[Qt::Key_F])
        light->getTransform().moveRight(-0.05f);
    if(keys[Qt::Key_H])
        light->getTransform().moveRight(0.05f);
    if(keys[Qt::Key_Y])
        light->getTransform().moveUp(0.05);
    if(keys[Qt::Key_R])
        light->getTransform().moveUp(-0.05);


    update();
}

void GLWindow::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    if( lastMousePosition == glm::ivec2(-1,-1) )
        lastMousePosition =  glm::ivec2(mouseEvent->localPos().x(), mouseEvent->localPos().y());

    mouseDelta = glm::ivec2(mouseEvent->localPos().x(), mouseEvent->localPos().y())  - lastMousePosition;
    glm::vec2 rotator(mouseDelta.y * glm::radians(0.1f), mouseDelta.x * glm::radians(0.1f));
    camera->getTransform().addYawPitch(glm::vec3(rotator,0));
    lastMousePosition = glm::ivec2(mouseEvent->localPos().x(), mouseEvent->localPos().y());

//    qDebug() << "x: " << mouseDelta.x << " y: " << mouseDelta.y;
//    qDebug() << "x: " << mouseEvent->globalPos().x()  << "y: " << mouseEvent->globalPos().y();
}

bool GLWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Enter:
        lastMousePosition = glm::ivec2(-1,-1);
        return true; break;
    default:
        return QOpenGLWindow::event(event);
    }
}

void GLWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        this->close();

    keys[event->key()] = true;

    if (event->isAutoRepeat())
        return;
}

void GLWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    keys[event->key()] = false;
}

void GLWindow::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = glm::vec2(e->localPos().x(), e->localPos().y());
}

void GLWindow::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
////    QVector2D diff = QVector2D(e->localPos()) - qmousePressPosition;
////    QVector2D diff = QVector2D(e->localPos().x()- gmousePressPosition.x, e->localPos().y()- gmousePressPosition.y) ;
//    glm::vec2 gdiff = glm::vec2(e->localPos().x(), e->localPos().y())  - mousePressPosition;
////    glm::vec2 diff = glm::vec2(difff.x(), difff.y());
//    // Rotation axis is perpendicular to the mouse position difference
//    // vector
//    glm::vec3 n = glm::normalize( glm::vec3(gdiff.y, gdiff.x, 0.0));
//    // Accelerate angular speed relative to the length of the mouse sweep
////    qreal acc = diff.length() / 100.0;
//    GLfloat acc = glm::length(gdiff)/100.0f;
//    // Calculate new rotation axis as weighted sum
////    qrotationAxis = (qrotationAxis * qangularSpeed + n * acc).normalized();

////    rotationAxis = glm::normalize(glm::vec3 (rotationAxis * angularSpeed + n * acc) );

//    // Increase angular speed
////    qangularSpeed += acc;
////    angularSpeed += acc;
}

void GLWindow::setViewMat()
{
    GLint view = glGetUniformLocation(shaderProgram->programId(),"view");
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera->getTransform().getCameraTransformMatrix()) );//&mtm[0][0]);
//    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera->getTransform().getTransformMatrix()) );//&mtm[0][0]);

//    GLint camPos=glGetUniformLocation(shaderProgram->programId(),"viewPos");;
//    const glm::vec3 v = camera->getTransform().getPosition();
//    glUniform3fv(camPos, 1, &v[0]);

/*
    window.input.getKeyModState();
//    if(gwindow.getInput()->getMouseWheel().y)
//        camera->getTransform().moveForward(window.getInput()->getMouseWheel().y);

//    vec2 rotator(window.getInput()->getMouseDelta().y * glm::radians(0.5f), window.getInput()->getMouseDelta().x * glm::radians(0.5f));
//    camera->getTransform().addYawPitch(vec3(rotator,0));

//    glm::mat4 vi = camera->getTransform().getCameraTransformMatrix();
//    shaderProgram->setUniformValue("view", QMatrix4x4(glm::value_ptr(vi)).transposed());
*/
}

void GLWindow::setProjectionMat()
{
    const float w = width();
    const float h = height();
    float aspect = w/h;
    glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    GLint projectionid = glGetUniformLocation(shaderProgram->programId(), "projection");
    glUniformMatrix4fv(projectionid, 1, GL_FALSE, &projection[0][0]);
}



