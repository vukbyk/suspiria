#include "glwindow.h"

//#include <QOpenGLShaderProgram>
//#include <QOpenGLBuffer>
//#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QMouseEvent>

#include <qlogging.h>
#include <qtimer.h>

#include <glm/gtc/type_ptr.hpp>

#include "scene.h"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "material.h"

#include "world.h"
#include "components.h"
//#include "entity.h"

#include "texture.h"
#include "mesh.h"

#ifdef GL_ES_VERSION_2_0
    #include <qopengles2ext.h>
#endif


GLWindow::GLWindow()
{
//    QTimer *refereshTimer = new QTimer(this);
//    connect(refereshTimer, SIGNAL(timeout()), this, SLOT(update()));
//    refereshTimer->start(1);

    // Use QBasicTimer because its faster than QTimer

    zNear = 0.3f;
    zFar = 1500.0f;
    camerEulerYP = btVector3(0,0,0);

    mousePressPosition=glm::vec2(0);


    timer.start(1, this);

    m_t1 = QTime::currentTime();

    skyShaderProgram = new ShaderProgram();
//    skyScene = new Scene();

    shaderProgram = new ShaderProgram();
//    scene = new Scene();
//    camera = new Camera();
//    camera->getTransform().moveForward(10);
//    lightByScene = new Light(shaderProgram);
//    lightByScene->getTransform().setPosition(btVector3(1.0, 2.0, 3.0));

    world = new World();
    camera = world->CreateEntity();
    camera.addTransformComponent(0.0f, 2.0f, 0.0f);
//    camera.addComponent(TransformComponent(Transform()));
//    auto transformCamera = &world->reg()->get<TransformComponent>(camera);
//    transformCamera->transform.setRotation(btVector3(1,0,0), -.5);
    controlledEntity = &camera;

//    entt::entity entity;
//    auto &m = world->reg()->get<TransformComponent>(entity);
//    glUniformMatrix4fv(lightID, 1, GL_FALSE, glm::value_ptr(m.transform.getTransformMatrix()) );//&mtm[0][0]);
//    scene->addChild(lightByScene);

}

GLWindow::~GLWindow()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
//    makeCurrent();
//    doneCurrent();
//    delete skyScene;
//    delete scene;
    delete world;
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    skyShaderProgram->initShaders("sky");

    std::vector<std::string> faces
    {
//        "sky/right.jpg",
//        "sky/left.jpg",
//        "sky/top.jpg",
//        "sky/bottom.jpg",
//        "sky/front.jpg",
//        "sky/back.jpg"
        "sky/stormydays_ft.tga",
        "sky/stormydays_bk.tga",
        "sky/stormydays_up.tga",
        "sky/stormydays_dn.tga",
        "sky/stormydays_rt.tga",
        "sky/stormydays_lf.tga"

    };
    world->getTextureManager()->loadBoxTexture("skyCube", faces);

    shaderProgram->initShaders("tutorial");

//    world->getMeshManager()->loadAssimp("vulture.obj");
    world->getMeshManager()->loadAssimp("cube.obj");
    world->getMeshManager()->loadAssimp("cubeinvert.obj");
    world->getMeshManager()->loadAssimp("cubeinvertmini.obj");
    world->getMeshManager()->loadAssimp("sphare.obj");
    world->getMeshManager()->loadAssimp("sky/skycube.obj");
    world->getMeshManager()->loadAssimp("sky/skycubeinv.obj");


    world->getTextureManager()->load("defaultComplex.png", true);
    world->getTextureManager()->load("white.png",     false);
    world->getTextureManager()->load("exoalbedo.jpg", false);
    world->getTextureManager()->load("brickwall.jpg", false);
    world->getTextureManager()->load("cube.png", false);
    world->getTextureManager()->load("defaultNormal.png", false);
    world->getTextureManager()->load("brickwall_normal.png", false);
    world->getTextureManager()->load("brickwall_normal.jpg", false);
//    world->getTextureManager()->load("exoskelet_Exoskelet_Normal.png", false);

    skyCube = world->CreateEntity();
    skyCube.addComponent(TransformComponent(Transform()));
    skyCube.addSimpleRenderComponent("sky/skycubeinv.obj", "skyCube", "defaultNormal.png");

    light = world->CreateEntity();
    light.addTransformComponent(0.0f, 3.0f, -6.0f);
    light.addSimpleRenderComponent("sky/skycubeinv.obj", "white.png", "defaultNormal.png");
    GLint lightID = shaderProgram->getUniform( "light");
    light.addComponent(LightComponent(lightID));

    Entity e;

//    e=world->CreateEntity();
//    e.addSimpleRenderComponent("cube.obj", "white.png", "defaultNormal.png");
//    e.addTransformComponent(0.0, -5.0, 0.0f);

    //100000 = 28fpsGentoo/Suse 24dbg-53dbg (56.5 after  remove rot and pos separate
    for(int i=0; i<400; i++)
    {
        for(int j=0; j<250; j++)
        {
            e=world->CreateEntity();
            e.addSimpleRenderComponent("cube.obj", "brickwall.jpg", "brickwall_normal.jpg");
//            e.addSimpleRenderComponent("sphare.obj", "defaultComplex.png", "defaultNormal.png");
//            if(j%2)
//                e.addSimpleRenderComponent("cube.obj", "brickwall.jpg", "brickwall_normal.jpg");
//            else
//                e.addSimpleRenderComponent("cube.obj", "white.png", "defaultNormal.png");

            e.addTransformComponent( -50.0f+i*1, 0.0f, -50.0f+j*1);
        }
    }


    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    //collor gamma
    glEnable(GL_FRAMEBUFFER_SRGB_EXT);

    // Use texture unit 0 which contains cube.png
//    program.setUniformValue("texture", 0); //Qt Alternative
//    GLint texLoc = shaderProgram->uniformLocation("uv");

    deltaTimer.start();
}

void GLWindow::resizeGL(int w, int h)
{
    GLfloat aspect = GLfloat(w) / GLfloat(h ? h : 1);
    projectionMat = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    shaderProgram->bindShader();
    shaderProgram->setProjectionMat(&projectionMat[0][0]);

    skyShaderProgram->bindShader();
    skyShaderProgram->setProjectionMat(&projectionMat[0][0]);
}

void GLWindow::paintGL()
{
    auto &cameraHandleTrans = world->reg()->get<TransformComponent>(camera);
    glm::mat4 invertMat = cameraHandleTrans.transform.getCameraTransformMatrix();
    auto *viewMat = glm::value_ptr(invertMat);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //Separated to change of size for Perspective when window is resized,
    //but also should have for zoom in future or jsut for evry case
//    shaderProgram->bindSetPVMat(pPointerPM, cameraMat);

    shaderProgram->bindShader();
    shaderProgram->setViewMat(viewMat);

    auto &lightTransform = world->reg()->get<TransformComponent>(light);
    auto &lightID = world->reg()->get<LightComponent>(light);
    glUniformMatrix4fv(lightID, 1, GL_FALSE, glm::value_ptr(lightTransform.transform.getTransformMatrix()) );//&mtm[0][0]);

    btVector3 cam = cameraHandleTrans.transform.getPosition();
    GLint viewPosCam = glGetUniformLocation(shaderProgram->programId(), "viewPosCam");

    glUniform3fv(viewPosCam, 1, &cam[0]);
    GLint model = shaderProgram->getUniform("model");

    btScalar tm[16];
    auto group = world->reg()->group<SimpleRenderComponent, TransformComponent>();//, cMesh>();
    //world.view<cRender>().each([this](auto &render) //as alternative
    group.each([this, &model, &tm, cameraHandleTrans](auto &render, TransformComponent &transform)//, auto &mesh)
    {
        btVector3 relativPos=transform.transform.getPosition() -
                             cameraHandleTrans.transform.getPosition();
        btVector3 norm = cameraHandleTrans.transform.forward();
//        qDebug("xyz %f %f %f\n", norm.x(), norm.y(), norm.z());
        btScalar nearPlaneDist=btDot(norm, relativPos);
        if(nearPlaneDist > 0 || relativPos.length2()>100*100 )
            return;

        transform.transform.getOpenGLMatrix(tm);

        glUniformMatrix4fv(model, 1, GL_FALSE, tm);
//      if(render.albedoId != save){...
        glActiveTexture(GL_TEXTURE0 + 0 );
        glBindTexture(GL_TEXTURE_2D, render.albedoId);

        glActiveTexture(GL_TEXTURE0 + 1 );
        glBindTexture(GL_TEXTURE_2D, render.normalId);

        glBindVertexArray(render.VAO);
        glDrawElements(GL_TRIANGLES, render.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);

    });

    //with this only unset pixels are drawne
    glDepthFunc(GL_LEQUAL);
    skyShaderProgram->bindShader();
    skyShaderProgram->setViewMat(viewMat);
    auto &renderSky = world->reg()->get<SimpleRenderComponent>(skyCube);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, world->getTextureManager()->getId("skyCube"));
    glBindVertexArray(renderSky.VAO);
    glDrawElements(GL_TRIANGLES, renderSky.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);



    count ++;
    nanoSec += deltaTimer.nsecsElapsed();
    if (count  >= 100)
    {
        qDebug()<< "timing:" << ( (double)nanoSec) / (count*1000000000) << "ns (ms*100)";
        qDebug()<< "timing:" << ((double)count*1000000000) /nanoSec  << "FPS";
        count=0;
        nanoSec=0;
    }
//    //qDebug()<< "timing:" << ((double)nanoSec / count) << "ns/call";

    deltaTime = (double)deltaTimer.nsecsElapsed()/1000000000;

//    m_t1 = QTime::currentTime();
//    float curDelta = m_t0.msecsTo(m_t1);
//    m_t0 = m_t1;
//    qDebug()<< "Old delta: "<<curDelta <<  "dt: " << (float)deltaTime/1000000000;//deltaTimer.nsecsElapsed();

    deltaTimer.restart();
}

void GLWindow::timerEvent(QTimerEvent *)
{
    if(!controlledEntity)
        return;
//    auto &m = world->reg()->get<TransformComponent>(light);
//    if(keys[Qt::Key_T])
//    {
//        m.transform.moveForwardGLM(moveSpeed * deltaTime);
//    }
//    if(keys[Qt::Key_G])
//    {
//        m.transform.moveForwardGLM(-moveSpeed * deltaTime);
//    }
//    if(keys[Qt::Key_F])
//    {
//        m.transform.moveRightGLM(-moveSpeed * deltaTime);
//    }
//    if(keys[Qt::Key_H])
//    {
//        m.transform.moveRightGLM(moveSpeed * deltaTime);
//    }
//    if(keys[Qt::Key_Y])
//    {
//        m.transform.moveUpGLM(moveSpeed * deltaTime);
//    }
//    if(keys[Qt::Key_R])
//    {
//        m.transform.moveUpGLM(-moveSpeed * deltaTime);
//    }

    if(keys[Qt::Key_1])
    {
        controlledEntity= &camera;
    }
    if(keys[Qt::Key_2])
    {
        controlledEntity = &light;
    }

    if(controlledEntity == nullptr)
    {
        update();
        return;
    }
    else
    {
        controlledTransform = &world->reg()->get<TransformComponent>(*controlledEntity);
    }

    if(keys[Qt::Key_W])
    {
        controlledTransform->transform.moveForwardGLM(-moveSpeed * deltaTime);
    }
    if(keys[Qt::Key_S])
    {
        controlledTransform->transform.moveForwardGLM(moveSpeed * deltaTime);
    }
    if(keys[Qt::Key_D])
    {
        controlledTransform->transform.moveRightGLM(moveSpeed * deltaTime);
    }
    if(keys[Qt::Key_A])
    {
        controlledTransform->transform.moveRightGLM(-moveSpeed * deltaTime);
    }
    if(keys[Qt::Key_E])
    {
        controlledTransform->transform.moveUpGLM(moveSpeed * deltaTime);
    }
    if(keys[Qt::Key_Q])
    {
        controlledTransform->transform.moveUpGLM(-moveSpeed * deltaTime);
    }

//    if(keys[Qt::Key_Right])
//        camera->getTransform().rotate     (glm::vec3(0,glm::radians(rotSpeed),0));
//    if(keys[Qt::Key_Left])
//        camera->getTransform().rotate     (glm::vec3(0,glm::radians(-rotSpeed),0));
//    if(keys[Qt::Key_Up])
//        camera->getTransform().addYawPitch(glm::vec3(glm::radians(.2f),0,0));
//    if(keys[Qt::Key_Down])
//        camera->getTransform().addYawPitch(glm::vec3(glm::radians(-.2f),0,0));

    if(keys[Qt::Key_Up])
    {
        glm::vec2 rotator( 3.0f * glm::radians(0.1f), 0.0f);
        if(controlledEntity!=nullptr)
        {
            if(controlledEntity==&camera)
            camerEulerYP += btVector3(rotator.y,rotator.x,0);
            btQuaternion q(camerEulerYP.x(), camerEulerYP.y(), 0);
            controlledTransform->transform.setRotation(q);
        }
        else
            controlledTransform->transform.addYawPitch(glm::vec3(rotator,0));
    }
    if(keys[Qt::Key_Down])
    {
        glm::vec2 rotator( -3.0f * glm::radians(0.1f), 0.0f);
        if(controlledEntity!=nullptr)
        {
            if(controlledEntity==&camera)
            camerEulerYP += btVector3(rotator.y,rotator.x,0);
            btQuaternion q(camerEulerYP.x(), camerEulerYP.y(), 0);
            controlledTransform->transform.setRotation(q);
        }
        else
            controlledTransform->transform.addYawPitch(glm::vec3(rotator,0));
    }
    if(keys[Qt::Key_Left])
    {
        glm::vec2 rotator( 0.0f, 3.0f * glm::radians(0.1f));
        if(controlledEntity!=nullptr)
        {
            if(controlledEntity==&camera)
            camerEulerYP += btVector3(rotator.y,rotator.x,0);
            btQuaternion q(camerEulerYP.x(), camerEulerYP.y(), 0);
            controlledTransform->transform.setRotation(q);
        }
        else
            controlledTransform->transform.addYawPitch(glm::vec3(rotator,0));
    }
    if(keys[Qt::Key_Right])
    {
        glm::vec2 rotator( 0.0f, -3.0f * glm::radians(0.1f));
        if(controlledEntity!=nullptr)
        {
            if(controlledEntity==&camera)
            camerEulerYP += btVector3(rotator.y,rotator.x,0);
            btQuaternion q(camerEulerYP.x(), camerEulerYP.y(), 0);
            controlledTransform->transform.setRotation(q);
        }
        else
            controlledTransform->transform.addYawPitch(glm::vec3(rotator,0));
    }
    update();
}

void GLWindow::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    if( lastMousePosition == glm::ivec2(-1,-1) )
    {
        lastMousePosition =  glm::ivec2(mouseEvent->position().x(), mouseEvent->position().y());
        return;//????
    }

    mouseDelta = glm::ivec2(mouseEvent->position().x(), mouseEvent->position().y())  - lastMousePosition;
    glm::vec2 rotator(mouseDelta.y * glm::radians(0.2f), -mouseDelta.x * glm::radians(0.2f));
    if(controlledEntity!=nullptr)
    {
        if(controlledEntity==&camera)
        camerEulerYP += btVector3(rotator.y,rotator.x,0);
        btQuaternion q(camerEulerYP.x(), camerEulerYP.y(), 0);
        controlledTransform->transform.setRotation(q);
    }
    else
    {
//        controlledTransform = &world->reg()->get<TransformComponent>(*controlledEntity);
        controlledTransform->transform.addYawPitch(glm::vec3(rotator,0));
    }

    lastMousePosition = glm::ivec2(mouseEvent->position().x(), mouseEvent->position().y());

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
    mousePressPosition = glm::vec2(e->position().x(), e->position().y());
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




