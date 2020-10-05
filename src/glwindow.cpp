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

GLWindow::GLWindow()
{

    mousePressPosition=glm::vec2(0);
    rotationAxis = glm::vec3(1);
    rotation = glm::quat();
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

    glClearColor(.05, 0.2, 0.3, 1);

    program.initShaders();

    texture = new Texture();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    AssimpLoad a("vulture.obj");
    auto mp = a.sceneMeshRendererDataCache.at("vulture.obj")[0];
    mesh=mp.mesh;

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

void GLWindow::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.3f, zFar = 7.0f, fov = 45.0f;

    // Reset projection
//    qprojection.setToIdentity();
    projection = glm::mat4(1);

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


    // Calculate model view transformation
    glm::mat4 gmatrix = glm::mat4(1);
    gmatrix = glm::translate(gmatrix, glm::vec3(0.0, 0.0, -5.0) );
//    grotation = glm::quat(-rotation.z(), rotation.y(),-rotation.x(),rotation.scalar());
    gmatrix = gmatrix * glm::toMat4(rotation);//glm::rotate(gmatrix, glm::mat4_cast(grotation));
    glm::mat4 tmat = projection * gmatrix;
    // Set modelview-projection matrix
//    glm::mat4 glmunif = gprojection * gmatrix;
//    QMatrix4x4 unif = QMatrix4x4( (float *) glm::value_ptr(glmunif) );
//    program.setUniformValue("mvp_matrix", unif.transposed() );


    // Calculate model view transformation
//    QMatrix4x4 matrix;
//    matrix.translate(0.0, 0.0, -5.0);
//    matrix.rotate(qrotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix",  QMatrix4x4( glm::value_ptr(tmat) ).transposed());
//    program.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
//    program.setUniformValue("texture", 0);
    GLint texLoc = program.uniformLocation("texture");
    glUniform1i(texLoc, 0);
    mesh->render();
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

    rotationAxis = glm::normalize(glm::vec3 (rotationAxis * gangularSpeed + n * acc) );

    // Increase angular speed
//    qangularSpeed += acc;
    gangularSpeed += acc;
}

void GLWindow::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
//    qangularSpeed *= 0.98;
    gangularSpeed *= 0.98;

    // Stop rotation when speed goes below threshold
    if (gangularSpeed < 0.01)
    {
        gangularSpeed = 0.0;
    }
    else
    {
        // Update rotation
//        qrotation = QQuaternion::fromAxisAndAngle(qrotationAxis, gangularSpeed) * qrotation;
        rotation = glm::rotate(rotation, gangularSpeed, rotationAxis) * rotation;
//        grotation = glm::quat(-qrotation.z(), qrotation.y(),-qrotation.x(),qrotation.scalar());
        // Request an update
        update();
    }
}
