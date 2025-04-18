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
#include "scene.h"
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
    timer.start(1, this);
    mousePressPosition=glm::vec2(0);
    m_t1 = QTime::currentTime();
}

GLWindow::~GLWindow()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.

    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteTextures(1, &textureColorbuffer);
    delete sceneWorld;
}

void GLWindow::initAndResizeBuffer()
{
    if(framebuffer)
        glDeleteFramebuffers(1, &framebuffer);
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width()*DPIScaleFactor, height()*DPIScaleFactor, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Attach color texture as color attachment 0 (max 8 different 0-7)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);//GL_READ_FRAMEBUFFER_EXT GL_DRAW_FRAMEBUFFER_EXT


    if(rbo)
        glDeleteRenderbuffers(1, &rbo);
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    // use a single renderbuffer object for both a depth AND stencil buffer.
    glRenderbufferStorage(GL_RENDERBUFFER, /*GL_DEPTH_COMPONENT32F*/GL_DEPTH24_STENCIL8, width()*DPIScaleFactor*UPScale, height()*DPIScaleFactor*UPScale);
    // now actually attach it
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, /*GL_DEPTH_ATTACHMENT*/GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);//can be only one FBTex2D ^
    // now that we actually created the framebuffer and added all
    // attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "!!! ERROR !!! ::FRAMEBUFFER:: Framebuffer is not complete!";
        return;
    }


    // Bind default framebuffer
    //    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    DPIScaleFactor = devicePixelRatio();
    qDebug()<<"Display scale factor: " << DPIScaleFactor;

    sceneWorld = new Scene();

    camera = sceneWorld->getCamera();
    eulerYP = &sceneWorld->reg()->get<FPSEulerComponent>(camera);
    controlledEntity = &camera;

    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 widthShadow, heightShadow, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER/*_EXT*/);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER/*_EXT*/);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR/*_EXT*/, borderColor);

    //    attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // now that we actually created the framebuffer and added all
    // attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "!!! ERROR !! ::FRAMEBUFFER:: Framebuffer is not complete!";
        return;
    }

    initAndResizeBuffer();

    light = sceneWorld->CreateEntity();
    //Vuk: I do not like this have to be changed
    //Vuk: unrelated problems with -90 pitch)
    btVector3 lightEulerRotation(0, btRadians(-90.0f), 0);
    //    btVector3 lightEulerRotation(0, btRadians(-45.0f), 0);
    light.addComponent<FPSEulerComponent>(lightEulerRotation);

    Transform lightInitTransform(btVector3(0.0f, 10.0f, 0.0f));
    btQuaternion lightQuatRot(lightEulerRotation.x(), lightEulerRotation.y(), 0);
    lightInitTransform.setRotation(lightQuatRot);
    light.addComponent(TransformComp(lightInitTransform));
    //    TransformComp lightTrans(lightInitTransform);
    //    light.addTransformComponent(lightTrans);


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
    //    light.addTextureBoxComp(/*"reflectCube"*/"skyCubeTex");
    CubeMapComp cubeMap((sceneWorld->getTextureManager()->getId("reflectCube")));
    light.addComponent(cubeMap);


    //    GLint lightID = shaderProgramMain->getUniform("light");
    //    light.addComponent(LightComp(lightID));
    //    light.addFixSphereBVComp();


    // Enable depth buffer
    //    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //    glFrontFace(GL_CW);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    //collor gamma
    //    glEnable(GL_FRAMEBUFFER_SRGB_EXT);
    glEnable(GL_FRAMEBUFFER_SRGB);

    deltaTimer.start();
}

void GLWindow::resizeGL(int w, int h)
{
    //    auto camPersp = world->reg()->get<CameraComp>(camera);
    Camera &camPersp = sceneWorld->reg()->get<CameraComp>(camera);
    camPersp.aspect = GLfloat(w) / GLfloat(h ? h : 1);

    setAspectFowMult();

    projectionMat = glm::perspective(glm::radians(camPersp.fov), camPersp.aspect,
                                     camPersp.zNear, camPersp.zFar);
    sceneWorld->shaderMain->bindShader();
    glUniformMatrix4fv(sceneWorld->shaderMain->projection, 1, GL_FALSE, &projectionMat[0][0]);
    //    shaderProgramMain->setProjectionMat(&projectionMat[0][0]);

    sceneWorld->shaderSky->bindShader();
    glUniformMatrix4fv(sceneWorld->shaderSky->projection, 1, GL_FALSE, &projectionMat[0][0]);
    //    world->shaderProgramSky->setProjectionMat(&projectionMat[0][0]);

    //    sceneWorld->shaderIrradiance->bindShader();
    //    glUniformMatrix4fv(sceneWorld->shaderIrradiance->projection, 1, GL_FALSE, &projectionMat[0][0]);

    initAndResizeBuffer();

}

void GLWindow::paintGL()
{
    btScalar tm[16];

    ///////////////////////////////////////////////
    /// \brief ShadowDepthMapGenerate
    ///
    glViewport(0, 0, widthShadow, heightShadow);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);


    sceneWorld->shaderShadow->bindShader();
    orthoLightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 51.0f);
    glUniformMatrix4fv(sceneWorld->shaderShadow->projection, 1, GL_FALSE, &orthoLightProjection[0][0]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);

    sceneWorld->shaderShadow->bindShader();
    TransformComp &lightTransformComp = sceneWorld->reg()->get<TransformComp>(light);
    glm::mat4 lightViewMat = lightTransformComp.transform.getInverseTransformMatrix();
    GLint viewUniform = sceneWorld->shaderShadow->view;// getUniform("view");
    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, &lightViewMat[0][0]);

    sceneWorld->shaderShadow->setTextureUniforms();

    GLint modelSH = sceneWorld->shaderShadow->model;//getUniform("model");
    auto groupSH = sceneWorld->reg()->group<MeshComp, MaterialPBRComp, TransformComp>();//, FixSphereBVComp>();//, cMesh>();
    GLuint lastMeshVAOLight;
    groupSH.each([this, &modelSH, &tm, &lastMeshVAOLight]
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



    ///////////////////////////////////////////////////////////////////////////////////
    /// \brief shaderMain
    ///
    glViewport(0, 0, width()*DPIScaleFactor*UPScale, height()*DPIScaleFactor*UPScale);
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

    sceneWorld->shaderMain->bindShader();
    sceneWorld->shaderMain->setTextureUniforms();


    Transform &cameraTransformComp = sceneWorld->reg()->get<TransformComp>(camera);
    //    btVector3 cam = cameraTransformComp.getPosition();
    glm::mat4 viewMat = cameraTransformComp.getInverseTransformMatrix();

    //    auto *viewMatRawPointer = glm::value_ptr(invertMat); //instead &nameMAT[0][0]
    //    shaderProgramMain->setViewMat(&viewMat[0][0]);
    glUniformMatrix4fv(sceneWorld->shaderMain->view, 1, GL_FALSE, &viewMat[0][0]);

    //    Alternative with entity in ShadeProgram
    //    mvpComp UniformMPV =  world->reg()->get<mvpComp>(*world->shaderMain->uniform);
    //    glUniformMatrix4fv(UniformMPV.view, 1, GL_FALSE, &viewMat[0][0]);

    Transform &lightTransform = sceneWorld->reg()->get<TransformComp>(light);
    glUniformMatrix4fv(sceneWorld->shaderMain->light, 1, GL_FALSE, glm::value_ptr(lightTransform.getTransformMatrix()) );//&mtm[0][0]);
    //    GLuint &lightCompID = world->reg()->get<LightComp>(light);
    //    glUniformMatrix4fv(lightCompID, 1, GL_FALSE, glm::value_ptr(lightTransform.getTransformMatrix()) );//&mtm[0][0]);

    //    glUniform3fv(camPosUniform, 1, &cam[0]);

    glm::mat4 lightSpaceMat = orthoLightProjection * lightTransformComp.transform.getInverseTransformMatrix();
    glUniformMatrix4fv(sceneWorld->shaderMain->lightSpaceMat, 1, GL_FALSE, glm::value_ptr(lightSpaceMat));
    glActiveTexture(GL_TEXTURE0 + 5);//0-5???
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);

    // glm::vec3 camWorldPos = cameraTransformComp.getGLMPosition(); // or whatever returns glm::vec3
    // GLint camPosUniform = glGetUniformLocation(sceneWorld->shaderMain->programId(), "camPos");
    // glUniform3fv(camPosUniform, 1, glm::value_ptr(camWorldPos));

    GLuint lastAlbedoId;
    GLuint lastNormalId;
    GLuint lastMetalId;
    GLuint lastRoughId;
    GLuint lastAoId;
    GLuint lastMeshVAO;
    GLuint lastCubeMapId= sceneWorld->reg()->get<CubeMapComp>(light).cubeTextureId;
    glActiveTexture(GL_TEXTURE0 + 6 );//0-5???
    glBindTexture(GL_TEXTURE_CUBE_MAP, lastCubeMapId);

    //world.view<cRender>().each([this](auto &render) //as alternative
    auto group = sceneWorld->reg()->group<MeshComp, MaterialPBRComp, TransformComp>();//, FixSphereBVComp>();//, cMesh>();
    group.each([this, &tm, &cameraTransformComp,
                &lastAlbedoId, &lastNormalId, &lastMetalId, &lastRoughId, &lastAoId, &lastMeshVAO]//,&lastCubeMapId]
               (MeshComp &mesh, MaterialPBRComp &texture, TransformComp &transform)//,FixSphereBVComp)// &boundingVol)
               {
                   //        if(!isInCameraFrustumAndDistance(cameraTransformComp, transform))//Moja
                   //            return;
                   //        if(!boundingVol.isOnFrustum(cam, frustum))
                   //            return ;
                   transform.transform.getOpenGLMatrix(tm);
                   glUniformMatrix4fv(sceneWorld->shaderMain->model, 1, GL_FALSE, tm);

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

    ///////// END shaderMain //////////////


    /////////////////////////////////////////////
    glDepthFunc(GL_LEQUAL);
    sceneWorld->shaderSky->bindShader();
    GLint SkyViewUniform = sceneWorld->shaderSky->view;//getUniform("view");
    //    shaderProgramMain->setViewMat(&viewMat[0][0]);
    glUniformMatrix4fv(SkyViewUniform, 1, GL_FALSE, &viewMat[0][0]);
    auto [cubeMapId, mesh] = sceneWorld->reg()->get<CubeMapComp, MeshComp>(sceneWorld->getSkyCube()); {}//<-for formating {}

    glActiveTexture(GL_TEXTURE0);//!!!!!!!!!!
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, /*(void*)*/0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);



    ///////////////////////////////////////////////
    //    glViewport(0, 0, width()/2, height()/2);
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    //    glViewport(0, height()/2, width(), height());
    glViewport(0, 0, width()*DPIScaleFactor, height()*DPIScaleFactor);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    //    // clear all relevant buffers
    //set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    //    glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // good for debuging screen size
    glClear(GL_COLOR_BUFFER_BIT);


    ///////////////////////////////////////////////////
    sceneWorld->shaderProgramFBScr->bind();
    sceneWorld->shaderProgramFBScr->setTextureUniforms();

    glBindVertexArray(quadVAO);
    glBindVertexArray(sceneWorld->renderQuad->VAO);
    glActiveTexture(GL_TEXTURE0 + 0 );
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
    //    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, sceneWorld->renderQuad->getIndicesSize(), GL_UNSIGNED_INT,/*(void*)*/0);


    //////////////////////////////////////////////////////////////
    /// \brief shaderDebugQuad
    //    glViewport(0, 0, width()/2, height()/2);
    glViewport(0, 0, widthShadow/4, heightShadow/4);
    //    // clear all relevant buffers
    //    glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

    sceneWorld->shaderDebugQuad->bind();
    sceneWorld->shaderDebugQuad->setTextureUniforms();

    glBindVertexArray(quadVAO);
    glBindVertexArray(sceneWorld->renderQuad->VAO);
    glActiveTexture(GL_TEXTURE0 + 0 );
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);	// use the color attachment texture as the texture of the quad plane
    glDrawElements(GL_TRIANGLES, sceneWorld->renderQuad->getIndicesSize(), GL_UNSIGNED_INT,/*(void*)*/0);
    //    glDrawArrays(GL_TRIANGLES, 0, 6);

    //////// END shaderDebugQuad /////////

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
    Camera &camPersp = sceneWorld->reg()->get<CameraComp>(camera);
    if( //relativPos.length2()>200*200   ||
        relativPos.angle(-forward) > qDegreesToRadians(camPersp.fov * aspectFowMult))
        return false;
    return true;
}

void GLWindow::setAspectFowMult()
{
    Camera &camPersp = sceneWorld->reg()->get<CameraComp>(camera);
    aspectFowMult = GLfloat(width() > GLfloat(height())? camPersp.aspect: 1);
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
        controlledTransform = &sceneWorld->reg()->get<TransformComp>(*controlledEntity);
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
        eulerYP = &sceneWorld->reg()->get<FPSEulerComponent>(camera);
    }
    if(keys[Qt::Key_2])
    {
        controlledEntity = &light;
        eulerYP = &sceneWorld->reg()->get<FPSEulerComponent>(light);
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
    Camera &camPersp = sceneWorld->reg()->get<CameraComp>(camera);
    const float halfVSide = camPersp.zFar * tanf(camPersp.fov * .5f);
    const float halfHSide = halfVSide * camPersp.aspect;
    const btVector3 frontMultFar = camPersp.zFar * cam.forward();

    frustum.nearFace = { cam.getPosition() + camPersp.zNear * cam.forward(), cam.forward() };
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

//void GLWindow::initQuadForFB()
//{
//    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//    // positions   // texCoords
//    -1.0f,  1.0f,   0.0f, 1.0f,
//    -1.0f, -1.0f,   0.0f, 0.0f,
//     1.0f, -1.0f,   1.0f, 0.0f,

    //    -1.0f,  1.0f,   0.0f, 1.0f,
    //     1.0f, -1.0f,   1.0f, 0.0f,
    //     1.0f,  1.0f,   1.0f, 1.0f
    //    };

    //    glGenVertexArrays(1, &quadVAO);
    //    glGenBuffers(1, &quadVBO);
    //    glBindVertexArray(quadVAO);
    //    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    //    glEnableVertexAttribArray(0);
    //    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    //    glEnableVertexAttribArray(1);
    //    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    //}
