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
    camera.addComponent<FPSEulerComponent>(btVector3(0,0,0));
    eulerYP = &world->reg()->get<FPSEulerComponent>(camera);
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
    glDeleteFramebuffers(1, &framebuffer);
    delete world;
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    importTextures();
    importMeshes();
    initQuadForFB();


    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 widthShadow, heightShadow, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//    attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Shadow shader configuration
    shaderShadow = new ShaderProgram("shadowmap");

    shaderDebugQuad = new ShaderProgram("debugquad");
    shaderDebugQuad->setUniformNamesAndIds({"depthMap"});
    shaderDebugQuad->bind();
    shaderDebugQuad->setTextureUniforms();


    std::vector<std::string> faces
    {
        "sky/right.jpg",
        "sky/left.jpg",
        "sky/top.jpg",
        "sky/bottom.jpg",
        "sky/front.jpg",
        "sky/back.jpg"

//        "sky/stormydays_ft.tga",
//        "sky/stormydays_bk.tga",
//        "sky/stormydays_up.tga",
////        "sky/stormydays_dn.tga",
//        "brickwall.jpg",
//        "sky/stormydays_rt.tga",
//        "sky/stormydays_lf.tga"

//        "sky/arrakisday_ft.tga",
//        "sky/arrakisday_bk.tga",
//        "sky/arrakisday_up.tga",
//        "sky/arrakisday_dn.tga",
//        "sky/arrakisday_rt.tga",
//        "sky/arrakisday_lf.tga"
    };
    world->getTextureManager()->loadBoxTexture("skyCubeTex", faces);

    std::vector<std::string> reflectCube
    {
        "sky/arrakisday_ft.tga",
        "sky/arrakisday_bk.tga",
        "sky/arrakisday_up.tga",
        "sky/arrakisday_dn.tga",
        "sky/arrakisday_rt.tga",
        "sky/arrakisday_lf.tga"
    };

    world->getTextureManager()->loadBoxTexture("reflectCube", reflectCube, false);


    // framebuffer configuration
    // -------------------------
    shaderProgramFBScr = new ShaderProgram("fbscreen");
    shaderProgramFBScr->setUniformNamesAndIds({"screenTexture"});
    shaderProgramFBScr->bind();
    shaderProgramFBScr->setTextureUniforms();



    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture

    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Attach color texture as color attachment 0 (max 8 different 0-7)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);//GL_READ_FRAMEBUFFER_EXT GL_DRAW_FRAMEBUFFER_EXT
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    // use a single renderbuffer object for both a depth AND stencil buffer.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width(), height());
    // now actually attach it
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);//can be only one FBTex2D ^
    // now that we actually created the framebuffer and added all
    // attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "!!! ERROR !! ::FRAMEBUFFER:: Framebuffer is not complete!";
        return;
    }
    // Bind default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
//    //unsigned int irradianceMap;
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);


    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------

    shaderIrradiance = new ShaderProgram("cubemap.vs", "irradiance_convolution.fs");
//    shaderIrradiance->setUniformNamesAndIds({"skyCube"});
    shaderIrradiance->setUniformNamesAndIds({"screenTexture"});
    shaderIrradiance->bind();
    shaderIrradiance->setTextureUniforms();


//    shaderBrdf = new ShaderProgram("brdf");

    //Todo: SkyMap connect to shader also
    const auto textureList = std::vector<std::string>(
                {"albedoMap", "normalMap", "metallicMap", "roughnessMap",
                 "aoMap", "shadowMap","skyCube"});
    shaderProgramMain = new ShaderProgram("pbr");
    shaderProgramMain->setUniformNamesAndIds(textureList);


    shaderProgramMain->bind();
    shaderProgramMain->setTextureUniforms();


    shaderProgramSky = new ShaderProgram("sky.vs", "sky.fs");
//    shaderProgramSky->setUniformNamesAndIds({"reflectCube"});
    shaderProgramSky->setUniformNamesAndIds({"skyCubeTex"});

    skyCube = world->CreateEntity();
    skyCube.addComponent(TransformComp(Transform()));
    skyCube.addMeshComponent("sky/skycubeinv.obj");
    skyCube.addTextureBoxComp("reflectCube");
//    skyCube.addTextureBoxComp("skyCubeTex");

    light = world->CreateEntity();
    //Vuk: I do not like this have to be changed
    //Vuk: unrelated problems with -90 pitch)
    btVector3 lightEulerRotation(0, btRadians(-90.0f), 0);
//    btVector3 lightEulerRotation(0, btRadians(-45.0f), 0);
    light.addComponent<FPSEulerComponent>(lightEulerRotation);



    Transform lightInitTransform(btVector3(0.0f, 10.0f, 0.0f));
    btQuaternion lightQuatRot(lightEulerRotation.x(), lightEulerRotation.y(), 0);
    lightInitTransform.setRotation(lightQuatRot);
    TransformComp lightTrans(lightInitTransform);
    light.addTransformComponent(lightTrans);

//    controlledEntity = &light;
//    eulerYP = &world->reg()->get<FPSEulerComponent>(light);
//    //  eulerYP->euler += btVector3(rotator.y,rotator.x,0);
//    eulerYP->euler = lightEulerRotation;//btVector3(rotator.y,rotator.x,0);
//    btQuaternion q(eulerYP->euler .x(), eulerYP->euler.y(), 0);
//    controlledTransform = &world->reg()->get<TransformComp>(*controlledEntity);
//    controlledTransform->transform.setRotation(q);

//    light.addSimpleRenderComp("cubeinvertmini.obj", "white.png", "normal1x1.png");
    light.addMeshComponent("cubeinvertmini.obj");
    light.addTexturePBRComp("white.png", "normal1x1.png", "white.png", "white.png", "white.png");
    light.addTextureBoxComp(/*"reflectCube"*/"skyCubeTex");


    GLint lightID = shaderProgramMain->getUniform("light");
    light.addComponent(LightComp(lightID));
//    light.addFixSphereBVComp();

    prepareAssets();

    // Enable depth buffer
//    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
////    glCullFace(GL_BACK);
////    glFrontFace(GL_CW);
//    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    //collor gamma
    glEnable(GL_FRAMEBUFFER_SRGB_EXT);

    deltaTimer.start();
}

void GLWindow::resizeGL(int w, int h)
{
    aspect = GLfloat(w) / GLfloat(h ? h : 1);
    setAspectFowMultiplayer();

    projectionMat = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    shaderProgramMain->bindShader();
    shaderProgramMain->setProjectionMat(&projectionMat[0][0]);

    shaderProgramSky->bindShader();
    shaderProgramSky->setProjectionMat(&projectionMat[0][0]);

    shaderIrradiance->bindShader();
    shaderIrradiance->setProjectionMat(&projectionMat[0][0]);

    shaderShadow->bindShader();
//    shaderShadow->setProjectionMat(&projectionMat[0][0]);
    orthoLightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 50.0f);
    shaderShadow->setProjectionMat(&orthoLightProjection[0][0]);

    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width(), height()); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
        return;
    }

}

void GLWindow::paintGL()
{
    TransformComp &cameraTransformComp = world->reg()->get<TransformComp>(camera);
    glm::mat4 invertMat = cameraTransformComp.transform.getInverseTransformMatrix();
    auto *viewMat = glm::value_ptr(invertMat);
    btScalar tm[16];

    /////////////////////////
    /// \brief ShadowDepthMapGenerate
    ///
    glViewport(0, 0, widthShadow, heightShadow);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);

    shaderShadow->bindShader();
    TransformComp &lightTransformComp = world->reg()->get<TransformComp>(light);

    auto *lightViewMat = glm::value_ptr(lightTransformComp.transform.getInverseTransformMatrix());

    shaderShadow->setViewMat(lightViewMat);//set lightMap in future

    shaderShadow->setTextureUniforms();

    GLint modelSH = shaderShadow->getUniform("model");
    auto groupSH = world->reg()->group<MeshComp, MaterialPBRComp, TransformComp>();//, FixSphereBVComp>();//, cMesh>();
    GLuint lastMeshVAOLight;
    groupSH.each([this, &modelSH, &tm, &cameraTransformComp, &lastMeshVAOLight]
               (MeshComp &mesh, MaterialPBRComp &texture, TransformComp &transform)//,FixSphereBVComp)// &boundingVol)
    {
        transform.transform.getOpenGLMatrix(tm);
        glUniformMatrix4fv(modelSH, 1, GL_FALSE, tm);
        if(lastMeshVAOLight != mesh.VAO)
        {
            lastMeshVAOLight = mesh.VAO;
            glBindVertexArray(mesh.VAO);
        }
        glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);
    });

    ////END ShadowDepthMapGenerate

    glViewport(0, 0, width(), height());
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CCW);

    //Separated to change of size for Perspective when window is resized,
    //but also should have for zoom in future or jsut for evry case
//  //shaderProgram->bindSetPVMat(pPointerPM, cameraMat);


    shaderProgramMain->bindShader();
    shaderProgramMain->setViewMat(viewMat);
    shaderProgramMain->setTextureUniforms();

    auto &lightTransform = world->reg()->get<TransformComp>(light);
    auto &lightID = world->reg()->get<LightComp>(light);
    glUniformMatrix4fv(lightID, 1, GL_FALSE, glm::value_ptr(lightTransform.transform.getTransformMatrix()) );//&mtm[0][0]);

    btVector3 cam = cameraTransformComp.transform.getPosition();
    GLint camPos = glGetUniformLocation(shaderProgramMain->programId(), "camPos");
    glUniform3fv(camPos, 1, &cam[0]);
    GLint model = shaderProgramMain->getUniform("model");
    auto group = world->reg()->group<MeshComp, MaterialPBRComp, TransformComp>();//, FixSphereBVComp>();//, cMesh>();
    //world.view<cRender>().each([this](auto &render) //as alternative

    GLint lightSpaceMatrixShP = shaderProgramMain->getUniform("lightSpaceMatrix");
    glm::mat4 lightSpaceMatrix = orthoLightProjection * lightTransformComp.transform.getInverseTransformMatrix();
    glUniformMatrix4fv(lightSpaceMatrixShP, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    glActiveTexture(GL_TEXTURE0 + 5);//0-5???
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);

    GLuint lastAlbedoId;
    GLuint lastNormalId;
    GLuint lastMetalId;
    GLuint lastRoughId;
    GLuint lastAoId;
    GLuint lastMeshVAO;
    GLuint lastCubeMapId= world->reg()->get<cubeMapComp>(light).cubeTextureId;
    glActiveTexture(GL_TEXTURE0 + 6 );//0-5???
    glBindTexture(GL_TEXTURE_CUBE_MAP, lastCubeMapId);

    group.each([this, &model, &tm, &cameraTransformComp,
               &lastAlbedoId, &lastNormalId, &lastMetalId, &lastRoughId, &lastAoId, &lastMeshVAO]//,&lastCubeMapId]
               (MeshComp &mesh, MaterialPBRComp &texture, TransformComp &transform)//,FixSphereBVComp)// &boundingVol)
    {
//        if(!isInCameraFrustumAndDistance(cameraTransformComp, transform))
//            return;
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
            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, texture.albedoId);
        }
        if(lastNormalId != texture.normalId)
        {
            lastNormalId = texture.normalId;
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, texture.normalId);
        }
        if(lastMetalId != texture.metallicId)
        {
            lastMetalId = texture.metallicId;
            glActiveTexture(GL_TEXTURE0 + 2);
            glBindTexture(GL_TEXTURE_2D, texture.metallicId);
        }
        if(lastRoughId != texture.roughnessId)
        {
            lastRoughId = texture.roughnessId;
            glActiveTexture(GL_TEXTURE0 + 3);
            glBindTexture(GL_TEXTURE_2D, texture.roughnessId);
        }
        if(lastAoId != texture.aoId)
        {
            lastAoId = texture.aoId;
            glActiveTexture(GL_TEXTURE0 + 4);
            glBindTexture(GL_TEXTURE_2D, texture.aoId);
        }
        glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);
//        qDebug("xyz %i \n", mesh.VAO);
    });
//    qDebug("xyz %f %f %f\n", norm.x(), norm.y(), norm.z());

    glDepthFunc(GL_LEQUAL);
    shaderProgramSky->bindShader();
    shaderProgramSky->setViewMat(viewMat);
    GLuint textureId;
    textureId = world->reg()->get<cubeMapComp>(skyCube).cubeTextureId;
    glActiveTexture(GL_TEXTURE0);//!!!!!!!!!!
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    auto &mesh = world->reg()->get<MeshComp>(skyCube);
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

//    glViewport(0, 0, width()/2, height()/2);
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
//    glViewport(0, height()/2, width(), height());
    glViewport(0, 0, width(), height());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
//    // clear all relevant buffers
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgramFBScr->bind();
    glBindVertexArray(quadVAO);
    shaderProgramFBScr->setTextureUniforms();
    glActiveTexture(GL_TEXTURE0 + 0 );
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane

    glDrawArrays(GL_TRIANGLES, 0, 6);

//    glViewport(0, 0, width()/2, height()/2);
    glViewport(0, 0, widthShadow/4, heightShadow/4);
//    glViewport(width()/2, height()/2, width(), height());//1024,1024);
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
//    // clear all relevant buffers
//    glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    shaderProgramFBScr->bind();
    shaderDebugQuad->bind();
    glBindVertexArray(quadVAO);
//    shaderProgramFBScr->setTextureUniforms();
    shaderDebugQuad->setTextureUniforms();
    glActiveTexture(GL_TEXTURE0 + 0 );
    glBindTexture(GL_TEXTURE_2D, /*textureColorbuffer*/ depthMapTexture);	// use the color attachment texture as the texture of the quad plane

    glDrawArrays(GL_TRIANGLES, 0, 6);


    count ++;
    nanoSec += deltaTimer.nsecsElapsed();
    if (count >= 100)
    {
        qDebug()<< "timing:" << ( (double)nanoSec) / (count*1000000000) << "ns (ms*100)";
        qDebug()<< "timing:" << ( (double)count*1000000000) /nanoSec  << "FPS";
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
    btVector3 offsetPos = cameraTransformComp.transform.getPosition() + (-forward * -1.0f);
    btVector3 relativPos=actor.transform.getPosition() - offsetPos;

//    btScalar nearPlaneDist=forward.dot( relativPos );
//    if(nearPlaneDist > 0 )//|| relativPos.length2()>200*200 )
//        return false;

    if( //relativPos.length2()>200*200   ||
        relativPos.angle(-forward) > qDegreesToRadians(fov * aspectFowMultiplayer))
        return false;
    return true;
}

void GLWindow::setAspectFowMultiplayer()
{
    aspectFowMultiplayer = GLfloat(width() > GLfloat(height())? aspect: 1);
}

void GLWindow::timerEvent(QTimerEvent *)
{
    if(!controlledEntity)
        return;
    if(controlledEntity == nullptr)
    {
        update();
        return;
    }
    else
    {
        controlledTransform = &world->reg()->get<TransformComp>(*controlledEntity);
    }
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
        eulerYP = &world->reg()->get<FPSEulerComponent>(camera);
    }
    if(keys[Qt::Key_2])
    {
        controlledEntity = &light;
        eulerYP = &world->reg()->get<FPSEulerComponent>(light);
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
            eulerYP->euler += btVector3(rotator.y,rotator.x,0);
            btQuaternion q(eulerYP->euler .x(), eulerYP->euler.y(), 0);
            controlledTransform->transform.setRotation(q);
        }
    }
    if(keys[Qt::Key_Down])
    {
        glm::vec2 rotator( -3.0f * glm::radians(0.1f), 0.0f);
        if(controlledEntity!=nullptr)
        {
            eulerYP->euler  += btVector3(rotator.y,rotator.x,0);
            btQuaternion q(eulerYP->euler.x(), eulerYP->euler.y(), 0);
            controlledTransform->transform.setRotation(q);
        }
    }
    if(keys[Qt::Key_Left])
    {
        glm::vec2 rotator( 0.0f, 3.0f * glm::radians(0.1f));
        if(controlledEntity!=nullptr)
        {
            eulerYP->euler += btVector3(rotator.y,rotator.x,0);
            btQuaternion q(eulerYP->euler.x(), eulerYP->euler.y(), 0);
            controlledTransform->transform.setRotation(q);
        }
    }
    if(keys[Qt::Key_Right])
    {
        glm::vec2 rotator( 0.0f, -3.0f * glm::radians(0.1f));
        if(controlledEntity!=nullptr)
        {
            eulerYP->euler += btVector3(rotator.y,rotator.x,0);
            btQuaternion q(eulerYP->euler.x(), eulerYP->euler.y(), 0);
            controlledTransform->transform.setRotation(q);
        }
    }
    update();
}

void GLWindow::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    if( lastMousePosition == glm::ivec2(-1,-1) )
    {
        lastMousePosition =  glm::ivec2(mouseEvent->position().x(), mouseEvent->position().y());
        return;//???
    }

    mouseDelta = glm::ivec2(mouseEvent->position().x(), mouseEvent->position().y())  - lastMousePosition;
    glm::vec2 rotator(mouseDelta.y * glm::radians(0.2f), -mouseDelta.x * glm::radians(0.2f));
    if(controlledEntity!=nullptr)
    {
        eulerYP->euler += btVector3(rotator.y,rotator.x,0);
        btQuaternion q(eulerYP->euler.x(), eulerYP->euler.y(), 0);
        controlledTransform->transform.setRotation(q);
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
//    // Accelerate angular speed relative ttextureUniformNameo the length of the mouse sweep
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


void GLWindow::importTextures()
{

    world->getTextureManager()->load("defaultComplex.png", true);
    world->getTextureManager()->load("normal1x1.png", false);
    world->getTextureManager()->load("white.png",     false);

    world->getTextureManager()->load("cyborg_normal.png", true, true);
    world->getTextureManager()->load("cyborg_diffuse.png", true, true);
    world->getTextureManager()->load("bricks2_normal.jpg", false, true );
    world->getTextureManager()->load("brickwall_normal.jpg", false, true );
    world->getTextureManager()->load("brickwall.jpg", false, true);
//    world->getTextureManager()->load("exoskelet_Exoskelet_BaseColor.png", false);
//    world->getTextureManager()->load("exoskelet_Exoskelet_Normal.png", false,false);
//    world->getTextureManager()->load("exoalbedo.jpg", true);
    world->getTextureManager()->load("vulture.png",false, true);
    world->getTextureManager()->load("Vulture_Diffuse.alpha_normal.jpg",false , true);
//    world->getTextureManager()->load("Vulture_Diffuse.alpha.png",false, true);


    world->getTextureManager()->load("rustediron/albedo.png",false, true);
    world->getTextureManager()->load("rustediron/normal.png", false , true);
    world->getTextureManager()->load("rustediron/roughness.png");
    world->getTextureManager()->load("rustediron/metallic.png");
    world->getTextureManager()->load("rustediron/ao.png");
}



void GLWindow::importMeshes()
{
    world->getMeshManager()->loadAssimp("vulture.obj");
    world->getMeshManager()->loadAssimp("cubemaya.obj");
//    world->getMeshManager()->loadAssimp("cubeinvert.obj");
    world->getMeshManager()->loadAssimp("cubeinvertmini.obj");
    world->getMeshManager()->loadAssimp("sphare.obj");
//    world->getMeshManager()->loadAssimp("sky/skycube.obj");
    world->getMeshManager()->loadAssimp("sky/skycubeinv.obj");
    world->getMeshManager()->loadAssimp("cyborg.obj");
}

void GLWindow::initQuadForFB()
{
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void GLWindow::prepareAssets()
{
    Entity e;

    e=world->CreateEntity();
    e.addTextureAlbedoNormalComp("cyborg_diffuse.png", "cyborg_normal.png");//"brickwall_normal.jpg");
    e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addTextureBoxComp("reflectCube");
    e.addMeshComponent("cyborg.obj");
    e.addTransformComponent( 0.0f, 2.0f, -7.0f);
//    reflectiveAsset.addFixSphereBVComp();
    //100000 = 28fpsGentoo/Suse 24dbg-53dbg (56.5 after  remove rot and pos separate
    for(int i=0; i<400; i++)
    {
        for(int j=0; j<250; j++)
        {
            e=world->CreateEntity();
            e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
            e.addTextureBoxComp("reflectCube");
            e.addTransformComponent( -50.0f+i*1, 0.0f, -50.0f+j*1);
            e.addMeshComponent("cubemaya.obj");
//            e.addFixSphereBVComp();
//            e.addComponent(MeshComp(world->getMeshManager()->get("cubemaya.obj")->getVAO(),
//                                    world->getMeshManager()->get("cubemaya.obj")->getIndicesSize()));
//            e.addSimpleRenderComp("cubemaya.obj", "brickwall.jpg", "brickwall_normal.jpg");
//            e.addSimpleRenderComponent("cubemaya.obj", "defaultComplex.png", "normal1x1.png");
////            if(j%2)
////                e.addSimpleRenderComponent("cubemaya.obj", "brickwall.jpg", "brickwall_normal.jpg");
////            else
////                e.addSimpleRenderComponent("cubemaya.obj", "white.png", "normal1x1.png");
        }
    }
        e=world->CreateEntity();
        e.addTextureAlbedoNormalComp("white.png", "normal1x1.png");//"brickwall_normal.jpg");
        e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
        e.addTextureBoxComp("reflectCube");
        e.addMeshComponent("cubemaya.obj");
        e.addTransformComponent( 3.0, 4.0f, -7.0f);

        e=world->CreateEntity();
        e.addTextureAlbedoNormalComp("vulture.png", "Vulture_Diffuse.alpha_normal.jpg");
        e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
        e.addTextureBoxComp("reflectCube");
        e.addMeshComponent("vulture.obj");
        e.addTransformComponent( 0.0, 1.0f, -7.0f);

        e=world->CreateEntity();
        e.addTextureAlbedoNormalComp("white.png", "normal1x1.png");//"brickwall_normal.jpg");
        e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
        e.addTextureBoxComp("reflectCube");
        e.addMeshComponent("sphare.obj");
        e.addTransformComponent( -3.0, 2.0f, -7.0f);

        e=world->CreateEntity();
        e.addTextureAlbedoNormalComp("white.png", "normal1x1.png");//"brickwall_normal.jpg");
        e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
        e.addTextureBoxComp("reflectCube");
        e.addMeshComponent("cubemaya.obj");
        e.addTransformComponent( 3.0, 1.0f, -7.0f);


}

