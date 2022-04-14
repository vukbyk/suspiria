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
#include "glwindow.h"
#include "glwindow.h"

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
    setAspectFowMultiplayer();//for Frustum culling multyply ony if W<H


    timer.start(1, this);

    m_t1 = QTime::currentTime();

//    skyScene = new Scene();
//    scene = new Scene();
//    camera = new Camera();
//    camera->getTransform().moveForward(10);
//    lightByScene = new Light(shaderProgram);
//    lightByScene->getTransform().setPosition(btVector3(1.0, 2.0, 3.0));

    world = new World();
    camera = world->CreateEntity();
    camera.addTransformComponent(0.0f, 2.0f, 0.0f);//For FPS benchmark 0,2,0
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
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    const auto textureList = std::vector<std::string>({"reflectionBox", "albedoTexture", "normalTexture"});

    shaderProgramPBR = new ShaderProgram("PBRForward");
    shaderProgramPBR->setUniformNamesAndIds(textureList);
    shaderProgramSky = new ShaderProgram("sky", {"albedoTexture"});

    std::vector<std::string> faces
    {
//        "sky/right.jpg",
//        "sky/left.jpg",
//        "sky/top.jpg",
//        "sky/bottom.jpg",
//        "sky/front.jpg",
//        "sky/back.jpg"

//        "sky/stormydays_ft.tga",
//        "sky/stormydays_bk.tga",
//        "sky/stormydays_up.tga",
////        "sky/stormydays_dn.tga",
//        "brickwall.jpg",
//        "sky/stormydays_rt.tga",
//        "sky/stormydays_lf.tga"

        "sky/arrakisday_ft.tga",
        "sky/arrakisday_bk.tga",
        "sky/arrakisday_up.tga",
        "sky/arrakisday_dn.tga",
        "sky/arrakisday_rt.tga",
        "sky/arrakisday_lf.tga"

    };
    world->getTextureManager()->loadBoxTexture("skyCube", faces);
    world->getTextureManager()->loadBoxTexture("reflectCube", faces, false);


    world->getTextureManager()->load("defaultComplex.png", true);
    world->getTextureManager()->load("white.png",     false);
    world->getTextureManager()->load("exoalbedo.jpg", true);
    world->getTextureManager()->load("brickwall.jpg", false);
//    world->getTextureManager()->load("cube.png", false);
    world->getTextureManager()->load("defaultNormal.png", false);
    world->getTextureManager()->load("brickwall_normal.png", false);
    world->getTextureManager()->load("brickwall_normal.jpg", false);
    world->getTextureManager()->load("exoskelet_Exoskelet_BaseColor.png", false);
    world->getTextureManager()->load("exoskelet_Exoskelet_Normal.png", false,false);
    world->getTextureManager()->load("cyborg_normal.png", true, true);
    world->getTextureManager()->load("cyborg_diffuse.png", true, true);
    world->getTextureManager()->load("vulture.png",true, true);
    world->getTextureManager()->load("Vulture_Diffuse.alpha_normal.jpg",true, true);
    world->getTextureManager()->load("Vulture_Diffuse.alpha.png",false, true);


    world->getMeshManager()->loadAssimp("vulture.obj");
    world->getMeshManager()->loadAssimp("cube.obj");
    world->getMeshManager()->loadAssimp("cubeinvert.obj");
    world->getMeshManager()->loadAssimp("cubeinvertmini.obj");
    world->getMeshManager()->loadAssimp("sphare.obj");
    world->getMeshManager()->loadAssimp("sky/skycube.obj");
    world->getMeshManager()->loadAssimp("sky/skycubeinv.obj");
    world->getMeshManager()->loadAssimp("cyborg.obj");

    skyCube = world->CreateEntity();
    skyCube.addComponent(TransformComp(Transform()));
    skyCube.addMeshComponent("sky/skycubeinv.obj");
    skyCube.addTextureBoxComp("skyCube");

    reflectiveAsset=world->CreateEntity();
    reflectiveAsset.addTexturePBRComp("cyborg_diffuse.png", "cyborg_normal.png");//"brickwall_normal.jpg");
    reflectiveAsset.addTextureBoxComp("reflectCube");
    reflectiveAsset.addMeshComponent("cyborg.obj");
    reflectiveAsset.addTransformComponent( 0, 2.0f, -7.0f);
//    reflectiveAsset.addFixSphereBVComp();

    light = world->CreateEntity();
    light.addTransformComponent(0.0f, 3.0f, -6.0f);
//    light.addSimpleRenderComp("cubeinvertmini.obj", "white.png", "defaultNormal.png");
    light.addMeshComponent("cubeinvertmini.obj");
    light.addTexturePBRComp("white.png", "defaultNormal.png");
    GLint lightID = shaderProgramPBR->getUniform("light");
    light.addComponent(LightComp(lightID));
//    light.addFixSphereBVComp();

    Entity e;
    e=world->CreateEntity();
    e.addTexturePBRComp("white.png", "defaultNormal.png");//"brickwall_normal.jpg");
    e.addTextureBoxComp("skyCube");
    e.addMeshComponent("sphare.obj");
    e.addTransformComponent( -3.0, 2.0f, -7.0f);
//    e.addFixSphereBVComp();

    e=world->CreateEntity();
    e.addTexturePBRComp("white.png", "defaultNormal.png");//"brickwall_normal.jpg");
    e.addTextureBoxComp("skyCube");
    e.addMeshComponent("cube.obj");
    e.addTransformComponent( 3.0, 2.0f, -7.0f);
//    e.addFixSphereBVComp();

//    e = world->CreateEntity();
//    e.addTexturePBRComp("white.png", "brickwall_normal.jpg");
//    e.addMeshComponent("cube.obj");
//    e.addTransformComponent( 0, 2.0f, -5.0f);
    //100000 = 28fpsGentoo/Suse 24dbg-53dbg (56.5 after  remove rot and pos separate
    for(int i=0; i<400; i++)
    {
        for(int j=0; j<250; j++)
        {
            e=world->CreateEntity();
            e.addTexturePBRComp("brickwall.jpg", "brickwall_normal.jpg");
            e.addTextureBoxComp("reflectCube");
            e.addTransformComponent( -50.0f+i*1, 0.0f, -50.0f+j*1);
            e.addMeshComponent("cube.obj");
//            e.addFixSphereBVComp();
//            e.addComponent(MeshComp(world->getMeshManager()->get("cube.obj")->getVAO(),
//                                    world->getMeshManager()->get("cube.obj")->getIndicesSize()));
//            e.addSimpleRenderComp("cube.obj", "brickwall.jpg", "brickwall_normal.jpg");
//            e.addSimpleRenderComponent("cube.obj", "defaultComplex.png", "defaultNormal.png");
////            if(j%2)
////                e.addSimpleRenderComponent("cube.obj", "brickwall.jpg", "brickwall_normal.jpg");
////            else
////                e.addSimpleRenderComponent("cube.obj", "white.png", "defaultNormal.png");
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
    aspect = GLfloat(w) / GLfloat(h ? h : 1);
    setAspectFowMultiplayer();

    projectionMat = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    shaderProgramPBR->bindShader();
    shaderProgramPBR->setProjectionMat(&projectionMat[0][0]);

    shaderProgramSky->bindShader();
    shaderProgramSky->setProjectionMat(&projectionMat[0][0]);

}

void GLWindow::paintGL()
{
    TransformComp &cameraTransformComp = world->reg()->get<TransformComp>(camera);
    glm::mat4 invertMat = cameraTransformComp.transform.getCameraTransformMatrix();
    auto *viewMat = glm::value_ptr(invertMat);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //Separated to change of size for Perspective when window is resized,
    //but also should have for zoom in future or jsut for evry case
//    shaderProgram->bindSetPVMat(pPointerPM, cameraMat);

    shaderProgramPBR->bindShader();
    shaderProgramPBR->setViewMat(viewMat);
    shaderProgramPBR->setTextureUniforms();

    auto &lightTransform = world->reg()->get<TransformComp>(light);
    auto &lightID = world->reg()->get<LightComp>(light);
    glUniformMatrix4fv(lightID, 1, GL_FALSE, glm::value_ptr(lightTransform.transform.getTransformMatrix()) );//&mtm[0][0]);

    btVector3 cam = cameraTransformComp.transform.getPosition();
    GLint viewPosCam = glGetUniformLocation(shaderProgramPBR->programId(), "viewPosCam");

    glUniform3fv(viewPosCam, 1, &cam[0]);
    GLint model = shaderProgramPBR->getUniform("model");

    btScalar tm[16];

    auto group = world->reg()->group<MeshComp, MaterialPBRComp, TransformComp>();//, FixSphereBVComp>();//, cMesh>();
    //world.view<cRender>().each([this](auto &render) //as alternative


//    GLuint lastAlbedoId = group.raw<TexturesPBRComp>()->albedoId;
//    glActiveTexture(GL_TEXTURE0 + 0 );
//    glBindTexture(GL_TEXTURE_2D, lastAlbedoId);

//    GLuint lastNormalId = group.raw<TexturesPBRComp>()->normalId;
//    glActiveTexture(GL_TEXTURE0 + 1 );
//    glBindTexture(GL_TEXTURE_2D, lastNormalId);

//    GLuint textureBox = world->reg()->get<TexturesBoxComp>(reflectiveAsset).albedoId;
//    glActiveTexture(GL_TEXTURE0 + 2 );
//    glBindTexture(GL_TEXTURE_CUBE_MAP, textureBox);

//    GLuint lastMeshVAO = group.raw<MeshComp>()->VAO;
//    glBindVertexArray(lastMeshVAO);
//    glDrawElements(GL_TRIANGLES, group.raw<MeshComp>()->indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);


    GLuint lastAlbedoId;
    GLuint lastNormalId;
    GLuint lastMeshVAO;

    GLuint lastCubeMapId= world->reg()->get<TexturesBoxComp>(reflectiveAsset).albedoId;
    glActiveTexture(GL_REFLECTION_MAP + 0 );
    glBindTexture(GL_REFLECTION_MAP, lastCubeMapId);
    Frustum frustum =Frustum(createFrustumFromCamera(cameraTransformComp));
    group.each([this, &model, &tm, &cameraTransformComp, &lastAlbedoId, &lastNormalId, &lastMeshVAO]//, &frustum]
               (MeshComp &mesh, MaterialPBRComp &texture, TransformComp &transform)//,FixSphereBVComp)// &boundingVol)
    {
        if(!isInCameraFrustumAndDistance(cameraTransformComp, transform))
            return;
//        if(!boundingVol.isOnFrustum(cam, frustum))
//            return ;
        transform.transform.getOpenGLMatrix(tm);
        glUniformMatrix4fv(model, 1, GL_FALSE, tm);

//ToDo: Material and Material instance based on textures and other param

        if(lastMeshVAO != mesh.VAO)
        {
            lastMeshVAO = mesh.VAO;
            glBindVertexArray(mesh.VAO);
        }
        if(lastAlbedoId != texture.albedoId)
        {
            lastAlbedoId = texture.albedoId;
            glActiveTexture(GL_TEXTURE0 + 1 );
            glBindTexture(GL_TEXTURE_2D, lastAlbedoId);
        }
        if(lastNormalId != texture.normalId)
        {
            lastNormalId = texture.normalId;
            glActiveTexture(GL_TEXTURE0 + 2);
            glBindTexture(GL_TEXTURE_2D, lastNormalId);
        }
        glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);

    });

//    qDebug("xyz %f %f %f\n", norm.x(), norm.y(), norm.z());


////    glUniformMatrix4fv(lightID, 1, GL_FALSE, glm::value_ptr(lightTransform.transform.getTransformMatrix()) );//&mtm[0][0]);

//    auto mirrorCam = cameraTransformComp.transform.getPosition();
//    auto viewPosCamMirror = glGetUniformLocation(mirrorShaderProgram->programId(), "viewPosCam");

//    glUniform3fv(viewPosCamMirror, 1, &mirrorCam[0]);
//    auto mirrormodel = mirrorShaderProgram->getUniform("model");

//    auto transform = world->reg()->get<TransformComponent>(reflectiveAsset);
//    transform.transform.getOpenGLMatrix(tm);
//    glUniformMatrix4fv(mirrormodel, 1, GL_FALSE, tm);

//    auto texture = world->reg()->get<TexturesBoxComp>(reflectiveAsset);


//    glActiveTexture(GL_TEXTURE0 + 0 );
//    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.albedoId);

//    auto meshMirror = world->reg()->get<MeshComp>(reflectiveAsset);
//    glBindVertexArray(meshMirror.VAO);
//    glDrawElements(GL_TRIANGLES, meshMirror.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);



    glDepthFunc(GL_LEQUAL);
    shaderProgramSky->bindShader();
    shaderProgramSky->setViewMat(viewMat);
    auto &textureId = world->reg()->get<TexturesBoxComp>(skyCube).albedoId;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    auto &mesh = world->reg()->get<MeshComp>(skyCube);
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);



    count ++;
    nanoSec += deltaTimer.nsecsElapsed();
    if (count >= 100)
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
//TODO: Camera to first rate citizen (add as class member pointer)
bool GLWindow::isInCameraFrustumAndDistance(TransformComp &cameraTransformComp, TransformComp &actor)
{
    btVector3 forward = cameraTransformComp.transform.forward();
    btVector3 offsetPos = cameraTransformComp.transform.getPosition() + (-forward * -5.0f);
    btVector3 relativPos=actor.transform.getPosition() - offsetPos;

//    btScalar nearPlaneDist=forward.dot( relativPos );
//    if(nearPlaneDist > 5 )//|| relativPos.length2()>200*200 )
//        return false;

    if( //relativPos.length2()>200*200   ||
        relativPos.angle(-forward) > qDegreesToRadians(fov * aspectFowMultiplayer))
        return false;

    return true;

    Frustum frustum= createFrustumFromCamera(cameraTransformComp);
}

void GLWindow::setAspectFowMultiplayer()
{
    aspectFowMultiplayer = GLfloat(width() > GLfloat(height())? aspect: 1);
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
        controlledTransform = &world->reg()->get<TransformComp>(*controlledEntity);
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


Frustum GLWindow::createFrustumFromCamera(const Transform& cam, float nearOffset, float farOffset)
{
    Frustum frustum;
    const float halfVSide = zFar * tanf(fov * .5f);
    const float halfHSide = halfVSide * aspect;
    const btVector3 frontMultFar = zFar * cam.forward();

    frustum.nearFace = { cam.getPosition() + zNear * cam.forward(), cam.forward() };
    frustum.farFace = { cam.getPosition() + frontMultFar, -cam.forward() };
    frustum.rightFace = { cam.getPosition(),
                            btCross(cam.up(),frontMultFar + cam.right() * halfHSide) };
    frustum.leftFace = { cam.getPosition(),
                            btCross(frontMultFar - cam.right() * halfHSide, cam.up()) };
    frustum.topFace = { cam.getPosition(),
                            btCross(cam.right(), frontMultFar - cam.up() * halfVSide) };
    frustum.bottomFace =
        { cam.getPosition(), btCross(frontMultFar + cam.up() * halfVSide, cam.right()) };

    return frustum;
}



