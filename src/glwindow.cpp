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
    light = world->CreateEntity();
    light.addTransformComponent(0.0f, 3.0f, -6.0f);
    camera = world->CreateEntity();
    camera.addTransformComponent(0.0f, 2.0f, 0.0f);
//    auto transformCamera = &world->reg()->get<TransformComponent>(camera);
//    transformCamera->transform.setRotation(btVector3(1,0,0), -.5);
    controlledEntity = &camera;

//    entt::entity entity;
//    auto &m = world->reg()->get<TransformComponent>(entity);
//    glUniformMatrix4fv(lightID, 1, GL_FALSE, glm::value_ptr(m.transform.getTransformMatrix()) );//&mtm[0][0]);
//    scene->addChild(lightByScene);

//    scene.world.emplace<TransformComponent>(entity, getTransform());
//    //ruzno za ent
//    Scene *s = &scene;
//    parentSpacial = s;

//    meshMng = new MeshManager;

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
//    delete skyScene;
//    delete scene;
    delete world;
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    skyShaderProgram->initShaders("sky");
    shaderProgram->initShaders("tutorial");

    world->getMeshManager()->loadAssimp("vulture.obj");
    world->getMeshManager()->loadAssimp("cube.obj");
    world->getMeshManager()->loadAssimp("cubeinvert.obj");
    world->getMeshManager()->loadAssimp("cubeinvertmini.obj");
    world->getMeshManager()->loadAssimp("sphare.obj");

    world->getTextureManager()->load("defaultComplex.png", true);

    world->getTextureManager()->load("white.png",     false);
    world->getTextureManager()->load("exoalbedo.jpg", false);
    world->getTextureManager()->load("brickwall.jpg", false);

    world->getTextureManager()->load("defaultNormal.png", false);
    world->getTextureManager()->load("brickwall_normal.png", false);
    world->getTextureManager()->load("brickwall_normal.jpg", false);
    world->getTextureManager()->load("exoskelet_Exoskelet_Normal.png", false);

    light.addSimpleRenderComponent("cubeinvert.obj", "white.png", "defaultNormal.png");

//    Model *skyCube = new Model("invertCube.obj", "brickwall.jpg");//, "Vulture_Diffuse.alpha_normal.jpg");

//    Model *skyCube = new Model(world->getMeshManager()->get("invertCube.obj"), world->getTextureManager()->get("brickwall.jpg"), world->getTextureManager()->get("defaultNormal.png"));


//    skyCube->getTransform().setOrigin(btVector3(0,0,0));
//    skyCube->getTransform().setPosition( glm::vec3(0.0f, 0.0f, 0.0f) );
//    skyCube->getTransform().setScale( glm::vec3(50.0f, 50.0f, 50.0f) );
//    skyScene->addChild(skyCube);


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
//            e.addSimpleRenderComponent("cube.obj", "white.png", "brickwall_normal.jpg");
            e.addSimpleRenderComponent("cube.obj", "defaultComplex.png", "defaultNormal.png");
//            if(j%2)
//                e.addSimpleRenderComponent("cube.obj", "brickwall.jpg", "brickwall_normal.jpg");
//            else
//                e.addSimpleRenderComponent("cube.obj", "white.png", "defaultNormal.png");

            e.addTransformComponent( -50.0f+i*1, 0.0f, -50.0f+j*1);
        }
    }


    {
//    Model *exo = new Model("vulture.obj", "vulture.jpg", "Vulture_Diffuse.alpha_normal.png");
////    Model *room = new Model("cube.obj", "white.png");
//    exo->getTransform().setPosition( glm::vec3(0.0f, 0.0f, -2.0f) );
////    exo->getTransform().setScale( glm::vec3(2.0f, 2.0f, 2.0f) );
//    scene->addChild(exo);

//    modelLight = new Model("sphare.obj", "white.png");
//    modelLight->getTransform().setPosition(btVector3(2.0, 5.0, .0));
//    scene->addChild(modelLight);

//    Model *sphare = new Model("sphare.obj", "white.png");//, "brickwall_normal.jpg");
////    sphare->getTransform().setPosition( glm::vec3(0.0f, 0.0f, -20.0f) );
////    sphare->getTransform().setScale( glm::vec3(12.0f, 12.0f, 12.0f) );
////    sphare->getTransform().setPosition(btVector3(2.0, 5.0, .0));
//    scene->addChild(sphare);

////    Model *bike = new Model("vulture.obj", "vulture.png", "Vulture_Diffuse.alpha_normal.jpg");

////    Material *matBike = new Material("vulture.png", "Vulture_Diffuse.alpha_normal.jpg" );
////    Mesh *modBike= new Mesh("vulture.obj") ;

//    for(int i=0; i<200; i++) //100000 rel = 53-55 fps, dbg = 28fps
//    {
//        for(int j=0; j<250; j++)
//        {
//            Model *bike = new Model(matHelper, modHelper);
////            bike->getTransform().setOrigin(btVector3(-100.0f+i*3, 0.0f, -100.0f+j*6));
//            bike->getTransform().setPosition( glm::vec3(-50.0f+i*3, 0.0f, -50.0f+j*6) );
////            bike->getTransform().setScale( glm::vec3(1.0f, 1.0f, 1.0f) );
////            //bike->getTransform().rotate( glm::vec3(glm::radians(45.0f),
////            //                                           glm::radians(45.0f),
////            //                                           glm::radians(45.0f)));
//            scene->addChild(bike);
//        }
//    }
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
    // Calculate aspect ratio
    GLfloat aspect = GLfloat(w) / GLfloat(h ? h : 1);
    projectionMat = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    shaderProgram->bindShader();
    shaderProgram->setProjectionMat(&projectionMat[0][0]);
    // Reset projection
//    projection = glm::mat4(1.0f);

    // Set perspective projection
//    projection = glm::perspective(fov, aspect, zNear, zFar);
}

void GLWindow::paintGL()
{


//    float aspect = (float)width()/height();
//    projectionMat = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
//    auto *pPointerPM = glm::value_ptr(projectionMat);//same as &projection[0][0])
    auto &cameraHandleTrans = world->reg()->get<TransformComponent>(camera);
//    auto *viewMat = glm::value_ptr(camera->getTransform().getCameraTransformMatrix());
    glm::mat4 invMat = cameraHandleTrans.transform.getCameraTransformMatrix();
    auto *viewMat = glm::value_ptr(invMat);
    // Clear color and depth buffer
//    glClear(/*GL_COLOR_BUFFER_BIT | */GL_DEPTH_BUFFER_BIT);

//    skyShaderProgram->bindSetPVMat(pPointerPM, cameraMat);
//    skyScene->renderAll();

//    glClear(/*GL_COLOR_BUFFER_BIT | */GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    lightByScene->renderAll();
//    scene->renderAll();

    //Separated to change of size for Perspective when window is resized,
    //but also should have for zoom in future
//    shaderProgram->bindSetPVMat(pPointerPM, cameraMat);

    shaderProgram->bindShader();
    shaderProgram->setViewMat(viewMat);

    GLint lightID = shaderProgram->getUniform( "light");

    auto &lightTransform = world->reg()->get<TransformComponent>(light);
    glUniformMatrix4fv(lightID, 1, GL_FALSE, glm::value_ptr(lightTransform.transform.getTransformMatrix()) );//&mtm[0][0]);


//    btVector3 cam = camera->getPosition() * (-1.0);
    btVector3 cam = cameraHandleTrans.transform.getPosition() * (-1.0);
    GLint viewPosCam = glGetUniformLocation(shaderProgram->programId(), "viewPosCam");
////    glm::vec3 v(0,0,1.0f);
    glUniform3fv(viewPosCam, 1, &cam[0]);
    GLint model = shaderProgram->getUniform("model");
    btScalar tm[16];
    auto group = world->reg()->group<SimpleRenderComponent, TransformComponent>();//, cMesh>();
    group.each([this, &model, &tm](auto &render, auto &transform)//, auto &mesh)
//    world.view<cRender>().each([this](auto &render)
    {
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


//    count ++;
//    nanoSec += deltaTimer.nsecsElapsed();
//    if (count  >= 100)
//    {
//        qDebug()<< "timing:" << ( (double)nanoSec) / (count*1000000000) << "ns (ms*100)";
//        qDebug()<< "timing:" << ((double)count*1000000000) /nanoSec  << "FPS";
//        count=0;
//        nanoSec=0;
//    }
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
        controlledTransform->transform.addYawPitch(glm::vec3(rotator,0));
    }
    if(keys[Qt::Key_Down])
    {
        glm::vec2 rotator( -3.0f * glm::radians(0.1f), 0.0f);
        controlledTransform->transform.addYawPitch(glm::vec3(rotator,0));
    }
    if(keys[Qt::Key_Left])
    {
        glm::vec2 rotator( 0.0f, 3.0f * glm::radians(0.1f));
        controlledTransform->transform.addYawPitch(glm::vec3(rotator,0));
    }
    if(keys[Qt::Key_Right])
    {
        glm::vec2 rotator( 0.0f, -3.0f * glm::radians(0.1f));
        controlledTransform->transform.addYawPitch(glm::vec3(rotator,0));
    }
    update();
}

void GLWindow::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    if( lastMousePosition == glm::ivec2(-1,-1) )
        lastMousePosition =  glm::ivec2(mouseEvent->position().x(), mouseEvent->position().y());

    mouseDelta = glm::ivec2(mouseEvent->position().x(), mouseEvent->position().y())  - lastMousePosition;
    glm::vec2 rotator(mouseDelta.y * glm::radians(0.2f), -mouseDelta.x * glm::radians(0.2f));
//    glm::vec2 rotator(mouseDelta.y * 0.001f, -mouseDelta.x * 0.001f);

    if(controlledEntity!=nullptr)
    {
        controlledTransform = &world->reg()->get<TransformComponent>(*controlledEntity);
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



