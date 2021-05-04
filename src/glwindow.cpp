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
#include "meshmanager.h"
#include "texturemanager.h"

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
    skyScene = new Scene(skyShaderProgram);

    shaderProgram = new ShaderProgram();
    scene = new Scene(shaderProgram);
    camera = new Camera();
    light = new Light(shaderProgram);
    light->getTransform().setPosition(btVector3(1.0, 2.0, 3.0));
    scene->addChild(light);

    world = new World();

    meshMenager = new MeshManager();
    textureMenager = new TextureManager();

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
    delete skyScene;
    delete scene;
    delete world;
    delete meshMenager;
    delete textureMenager;
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shaderProgram->initShaders("tutorial");
    scene->setModel( glGetUniformLocation(shaderProgram->programId(),"model") );

    skyShaderProgram->initShaders("sky");
    skyScene->setModel( glGetUniformLocation(skyShaderProgram->programId(),"model") );

    meshMenager->loadAssimp("vulture.obj");
    meshMenager->loadAssimp("cube.obj");
    meshMenager->loadAssimp("invertCube.obj");

    textureMenager->load("brickwall.jpg", true);
    textureMenager->load("exoalbedo.jpg", true);
    textureMenager->load("exoskelet_Exoskelet_Normal.png", false);
    textureMenager->load("defaultNormal.png", false);

//    Material *m=new Material(textureMenager->get("brickwall.jpg"));

//    Model *skyCube = new Model("invertCube.obj", "brickwall.jpg");//, "Vulture_Diffuse.alpha_normal.jpg");
    Model *skyCube = new Model(meshMenager->get("invertCube.obj"), textureMenager->get("brickwall.jpg"), textureMenager->get("defaultNormal.png"));
//    skyCube->getTransform().setOrigin(btVector3(0,0,0));
//    skyCube->getTransform().setPosition( glm::vec3(0.0f, 0.0f, 0.0f) );
//    skyCube->getTransform().setScale( glm::vec3(50.0f, 50.0f, 50.0f) );
    skyScene->addChild(skyCube);

    matHelper = new Material(textureMenager->get("exoalbedo.jpg"), textureMenager->get("exoskelet_Exoskelet_Normal.png") );
//    meshHelper = new Mesh("cube.obj") ;

//    Model *cube = new Model("cube.obj", "brickwall.jpg", "brickwall_normal.jpg");
////    Model *room = new Model("cube.obj", "white.png");
//    cube->getTransform().setPosition( glm::vec3(0.0f, 0.0f, -20.0f) );
//    cube->getTransform().setScale( glm::vec3(12.0f, 12.0f, 12.0f) );
//    scene->addChild(cube);

    for(int i=0; i<200; i++) //100000 = 28fps
    {
        for(int j=0; j<250; j++)
        {

//            cr.albedoId = textureMenager->getId("exoalbedo.jpg");
//            cr.normalId = textureMenager->getId("exoskelet_Exoskelet_Normal.png");
            cr.albedoId = matHelper->getAlbedo()->getTextureId();
            cr.normalId = matHelper->getNormal()->getTextureId();
            cr.VAO = meshMenager->get("cube.obj")->getVAO();
            cr.indicesSize = meshMenager->get("cube.obj")->getIndicesSize();


            e=world->CreateEntity();
            e.addComponent(cr);
            e.addComponent(transformComponent(btVector3(-50.0f+i*3, 0.0f, -50.0f+j*6)));
//            e.addComponent(cMesh(modHelper->VAO, modHelper->indicesSize));

        }
    }
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

//    for(int i=0; i<200; i++) //100000 = 28fps
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

//    delete matBike;
//    delete modBike;

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

    // Reset projection
    projection = glm::mat4(1.0f);

    // Set perspective projection
    projection = glm::perspective(fov, aspect, zNear, zFar);
}

void GLWindow::paintGL()
{

    // Clear color and depth buffer
//    glClear(/*GL_COLOR_BUFFER_BIT | */GL_DEPTH_BUFFER_BIT);

    skyShaderProgram->bindShader();
    setProjectionMat();
    setViewMat();
    skyScene->renderAll();

    glClear(/*GL_COLOR_BUFFER_BIT | */GL_DEPTH_BUFFER_BIT);

    shaderProgram->bindShader();
//    setProjectionMat();
//    setViewMat();

    const float w = width();
    const float h = height();
    float aspect = w/h;
    glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    GLint projectionid = glGetUniformLocation(shaderProgram->programId(), "projection");
    glUniformMatrix4fv(projectionid, 1, GL_FALSE, &projection[0][0]);

    GLint view = glGetUniformLocation(shaderProgram->programId(),"view");
    glUniformMatrix4fv(view, 1, GL_FALSE,
                       glm::value_ptr(camera->getTransform().getCameraTransformMatrix()) );//&mtm[0][0]);

    light->renderAll();
//    scene->renderAll();

//    btScalar tm[16];
//    GLint model=glGetUniformLocation(shaderProgram->programId(),"model");
//    cr.transform.getOpenGLMatrix(tm);
//    glUniformMatrix4fv(model, 1, GL_FALSE, tm);
////        glUniformMatrix4fv(model, 1, GL_FALSE, render.transform);

////        if(render.albedoId != save)
//    {
//        glActiveTexture(GL_TEXTURE0 + 0 );
//        glBindTexture(GL_TEXTURE_2D, cr.albedoId);

//        glActiveTexture(GL_TEXTURE0 + 1 );
//        glBindTexture(GL_TEXTURE_2D, cr.normalId);
////            save = render.albedoId;

//        glBindVertexArray(cr.VAO);
//        glDrawElements(GL_TRIANGLES, cr.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);
//    }


    GLint model=glGetUniformLocation(shaderProgram->programId(),"model");
//    glGetUniformLocation(&model, "model");
    btScalar tm[16];
    auto group = world->registry.group<SimpleRenderComponent, transformComponent>();//, cMesh>();
    group.each([this, &model, &tm](auto &render, auto &transform)//, auto &mesh)
//    world.view<cRender>().each([this](auto &render)
    {
//        render.transform.getOpenGLMatrix(tm);
        transform.transform.getOpenGLMatrix(tm);
        glUniformMatrix4fv(model, 1, GL_FALSE, tm);
//        glUniformMatrix4fv(model, 1, GL_FALSE, render.transform);

//        if(render.albedoId != save)
        {


            glActiveTexture(GL_TEXTURE0 + 0 );
            glBindTexture(GL_TEXTURE_2D, render.albedoId);

            glActiveTexture(GL_TEXTURE0 + 1 );
            glBindTexture(GL_TEXTURE_2D, render.normalId);
//            save = render.albedoId;

            glBindVertexArray(render.VAO);
            glDrawElements(GL_TRIANGLES, render.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);
//            glBindVertexArray(mesh.VAO);
//            glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);
        }
    });


//    count ++;
//    nanoSec += deltaTimer.nsecsElapsed();
//    if (count  >= 100)
//    {

//        qDebug()<< "timing:" << ( (double)nanoSec) / (count*1000000000) << "ms";
//        qDebug()<< "timing:" << ((double)count*1000000000) /nanoSec  << "FPS";
//        count=0;
//        nanoSec=0;
//    }

//    qDebug()<< "timing:" << ((double)nanoSec / count) << "ns/call";

    deltaTime = (double)deltaTimer.nsecsElapsed()/1000000000;


//    m_t1 = QTime::currentTime();
//    float curDelta = m_t0.msecsTo(m_t1);
//    m_t0 = m_t1;
//    qDebug()<< "Old delta: "<<curDelta <<  "dt: " << (float)deltaTime/1000000000;//deltaTimer.nsecsElapsed();

    deltaTimer.restart();
}

void GLWindow::timerEvent(QTimerEvent *)
{
    if(keys[Qt::Key_W])
    {
//        camera->getBTransform().getOrigin().setY(btScalar( 1000.0f));
        camera->getTransform().moveForwardGLM(-moveSpeed * deltaTime);
    }
    if(keys[Qt::Key_S])
        camera->getTransform().moveForwardGLM(moveSpeed * deltaTime);
    if(keys[Qt::Key_D])
        camera->getTransform().moveRightGLM(-moveSpeed * deltaTime);
    if(keys[Qt::Key_A])
        camera->getTransform().moveRightGLM(moveSpeed * deltaTime);
    if(keys[Qt::Key_E])
        camera->getTransform().moveUpGLM(-moveSpeed * deltaTime);
    if(keys[Qt::Key_Q])
        camera->getTransform().moveUpGLM(moveSpeed * deltaTime);

//    if(keys[Qt::Key_Right])
//        camera->getTransform().rotate     (glm::vec3(0,glm::radians(rotSpeed),0));
//    if(keys[Qt::Key_Left])
//        camera->getTransform().rotate     (glm::vec3(0,glm::radians(-rotSpeed),0));
//    if(keys[Qt::Key_Up])
//        camera->getTransform().addYawPitch(glm::vec3(glm::radians(.2f),0,0));
//    if(keys[Qt::Key_Down])
//        camera->getTransform().addYawPitch(glm::vec3(glm::radians(-.2f),0,0));

    if(keys[Qt::Key_Up])
        camera->getTransform().addYawPitch(btVector3(rotSpeed,0,0));
    if(keys[Qt::Key_Down])
        camera->getTransform().addYawPitch(btVector3(-rotSpeed,0,0));

    if(keys[Qt::Key_T])
    {
        light->getTransform().moveForwardGLM(moveSpeed * deltaTime);
        modelLight->getTransform().moveForwardGLM(moveSpeed * deltaTime);
        auto &m = scene->world.get<transformComponent>(light->entity);
        m.transform.moveForwardGLM(moveSpeed * deltaTime);
//        btVector3 pos( light->getTransform().getGLMPosition().x,
//                       light->getTransform().getGLMPosition().y,
//                       light->getTransform().getGLMPosition().z);
//        m.transform.setOrigin(pos);

    }
    if(keys[Qt::Key_G])
    {
        light->getTransform().moveForwardGLM(-moveSpeed * deltaTime);
        modelLight->getTransform().moveForwardGLM(-moveSpeed * deltaTime);
        auto &m = scene->world.get<transformComponent>(light->entity);
        m.transform.moveForwardGLM(-moveSpeed * deltaTime);
    }
    if(keys[Qt::Key_F])
    {
        light->getTransform().moveRightGLM(-moveSpeed * deltaTime);
        modelLight->getTransform().moveRightGLM(-moveSpeed * deltaTime);
        auto &m = scene->world.get<transformComponent>(light->entity);
        m.transform.moveRightGLM(-moveSpeed * deltaTime);
    }
    if(keys[Qt::Key_H])
    {
        light->getTransform().moveRightGLM(moveSpeed * deltaTime);
        modelLight->getTransform().moveRightGLM(moveSpeed * deltaTime);
        auto &m = scene->world.get<transformComponent>(light->entity);
        m.transform.moveRightGLM(moveSpeed * deltaTime);
    }
    if(keys[Qt::Key_Y])
    {
        light->getTransform().moveUpGLM(moveSpeed * deltaTime);
        modelLight->getTransform().moveUpGLM(moveSpeed * deltaTime);
        auto &m = scene->world.get<transformComponent>(light->entity);
        m.transform.moveUpGLM(moveSpeed * deltaTime);
    }
    if(keys[Qt::Key_R])
    {
        light->getTransform().moveUpGLM(-moveSpeed * deltaTime);
        modelLight->getTransform().moveUpGLM(-moveSpeed * deltaTime);
        auto &m = scene->world.get<transformComponent>(light->entity);
        m.transform.moveUpGLM(-moveSpeed * deltaTime);
    }

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
//    GLint view = glGetUniformLocation(shaderProgram->programId(),"view");
//    glUniformMatrix4fv(view, 1, GL_FALSE, camera->getTransformMatrix() );//&mtm[0][0]);

//    view = glGetUniformLocation(skyShaderProgram->programId(),"view");
//    glUniformMatrix4fv(view, 1, GL_FALSE, camera->getTransformMatrix()  );//&mtm[0][0]);

//    GLint view = glGetUniformLocation(shaderProgram->programId(),"view");
//    glUniformMatrix4fv(view, 1, GL_FALSE, camera->getTransformMatrix() );//&mtm[0][0]);

//    view = glGetUniformLocation(skyShaderProgram->programId(),"view");
//    glUniformMatrix4fv(view, 1, GL_FALSE, camera->getTransformMatrix()  );//&mtm[0][0]);

    GLint view = glGetUniformLocation(shaderProgram->programId(),"view");
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera->getTransform().getCameraTransformMatrix()) );//&mtm[0][0]);

    view = glGetUniformLocation(skyShaderProgram->programId(),"view");
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera->getTransform().getCameraTransformMatrix()) );//&mtm[0][0]);

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

    projectionid = glGetUniformLocation(skyShaderProgram->programId(), "projection");
    glUniformMatrix4fv(projectionid, 1, GL_FALSE, &projection[0][0]);
}



