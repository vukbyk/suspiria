#include "glwindow.h"


#include <QImage>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QTimer>
//#include <QScreen>

#include <QMouseEvent>
#include <cmath>
#include <assimpload.h>
#include <mesh.h>
#include <texture.h>

GLWindow::GLWindow()
{
}

GLWindow::~GLWindow()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
//    makeCurrent();
    delete texture;
//    doneCurrent();
}

void GLWindow::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void GLWindow::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}

void GLWindow::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(.05, 0.2, 0.3, 1);

    initShaders();
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

QString versionedShaderCode(const QString &src)
{
    QString versionedSrc;

    if (QOpenGLContext::currentContext()->isOpenGLES())
       return versionedSrc.append("#version 300 es\n" + src);
    else
       return versionedSrc.append("#version 330\n" + src);
}

void GLWindow::initShaders()
{
    // Compile vertex shader

//    stream.setCodec("UTF-8");
    QFile filevertexShaderSource(":/shaders/vshader.glsl");
    if (!filevertexShaderSource.open(QIODevice::ReadOnly|QFile::Text))
    {
        qDebug() << "Cannot open file ";
        close();
    }
    QTextStream invs(&filevertexShaderSource);
    QString vsSource = invs.readAll();
//    /*qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << */versionedShaderCode(vsSource);

    filevertexShaderSource.close();


//    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
    if (!program.addShaderFromSourceCode(QOpenGLShader::Vertex, versionedShaderCode(vsSource)))
        close();


    QFile fragmentShaderSource(":/shaders/fshader.glsl");
    if (!fragmentShaderSource.open(QIODevice::ReadOnly|QFile::Text))
    {
        qDebug() << "Cannot open file ";
        close();
    }
    QTextStream infs(&fragmentShaderSource);
    QString fsSource = infs.readAll();
    /*qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << */versionedShaderCode(fsSource);
    fragmentShaderSource.close();

    // Compile fragment shader
//    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
    if (!program.addShaderFromSourceCode(QOpenGLShader::Fragment, versionedShaderCode(fsSource)))
        close();

//    // Bind shader pipeline for use
//    if (!program.bind())
//        close();

//    // Compile vertex shader
//    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl"))
//        close();

//    // Compile fragment shader
//    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl"))
//        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}

void GLWindow::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
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
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    mesh->render();
}

void GLWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_R)
    {
        colorBack = 0.5f;
    }
}
