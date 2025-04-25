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
#include "camera.h"

#include "world.h"
#include "scene.h"
#include "components.h"

//#include "entity.h"

//#include "mesh.h"

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

    visibleEntities.reserve(10000); //Reserve only once here
    nextVisibleEntities.reserve(10000);
    // shadowEntities.reserve(10000);
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

    // light.addComponent<FPSEulerComponent>(lightEulerRotation);

    Transform lightInitTransform(btVector3(.0f, .0f, 0.0f));
    btQuaternion lightQuatRot(btRadians(110.0f), btRadians(-45.0f), btRadians(0.0f));
    lightInitTransform.setRotation(lightQuatRot);
    // lightInitTransform.setRotation(btQuaternion(btVector3(0.5f, -1.0f, 0.0f), -45.0f));// * btQuaternion(btVector3(1.0f, 0.0f, 0.0f), -45.0f) );

    light.addComponent(TransformComp(lightInitTransform));

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
    static std::future<std::vector<entt::entity>> futureVisibleEntities;

    btScalar tm[16];
    glm::mat4 lightViewMat;
    glm::mat4 lightViewProj;

    {
        glViewport(0, 0, widthShadow, heightShadow);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.0f, 4.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_FRONT);

        sceneWorld->shaderShadow->bindShader();
        orthoLightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, 2.0f, 100.0f);
        glUniformMatrix4fv(sceneWorld->shaderShadow->projection, 1, GL_FALSE, &orthoLightProjection[0][0]);

        TransformComp &lightTransformComp = sceneWorld->reg()->get<TransformComp>(light);
        lightViewMat = lightTransformComp.transform.getInverseTransformMatrix();
        glUniformMatrix4fv(sceneWorld->shaderShadow->view, 1, GL_FALSE, &lightViewMat[0][0]);

        sceneWorld->shaderShadow->setTextureUniforms();

        auto shadowGroup = sceneWorld->reg()->group<MeshComp, MaterialPBRComp, TransformComp>();
        GLuint lastVAO = 0;
        for (auto [entity, mesh, material, transform] : shadowGroup.each())
        {
            transform.transform.getOpenGLMatrix(tm);
            glUniformMatrix4fv(sceneWorld->shaderShadow->model, 1, GL_FALSE, tm);
            if (mesh.VAO != lastVAO) {
                lastVAO = mesh.VAO;
                glBindVertexArray(mesh.VAO);
            }
            glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, 0);
        }
    }

    glm::mat4 viewMat;
    {
        glViewport(0, 0, width() * DPIScaleFactor * UPScale, height() * DPIScaleFactor * UPScale);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);

        sceneWorld->shaderMain->bindShader();
        sceneWorld->shaderMain->setTextureUniforms();

        Transform* cameraTransform = &sceneWorld->reg()->get<TransformComp>(camera).transform;
        if (sceneWorld->reg()->all_of<ParentComponent>(camera)) {
            auto& parent = sceneWorld->reg()->get<ParentComponent>(camera);
            if (parent.parentTransform)
                cameraTransform = parent.parentTransform;
        }

        viewMat = cameraTransform->getInverseTransformMatrix();
        glUniformMatrix4fv(sceneWorld->shaderMain->view, 1, GL_FALSE, &viewMat[0][0]);

        Transform& lightTransform = sceneWorld->reg()->get<TransformComp>(light);
        glm::mat4 lightSpaceMat = orthoLightProjection * lightViewMat;
        glUniformMatrix4fv(sceneWorld->shaderMain->light, 1, GL_FALSE, glm::value_ptr(lightTransform.getTransformMatrix()));
        glUniformMatrix4fv(sceneWorld->shaderMain->lightSpaceMat, 1, GL_FALSE, glm::value_ptr(lightSpaceMat));

        glActiveTexture(GL_TEXTURE0 + 5);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        GLuint cubeMapId = sceneWorld->reg()->get<CubeMapComp>(light).cubeTextureId;
        glActiveTexture(GL_TEXTURE0 + 6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

        GLuint lastVAO = 0, lastAlbedo = 0, lastNormal = 0, lastMetal = 0, lastRough = 0, lastAO = 0;

        auto mainGroup = sceneWorld->reg()->group<MeshComp, MaterialPBRComp, TransformComp>();
        glm::mat4 viewProj = projectionMat * viewMat;

        if (futureVisibleEntities.valid() && futureVisibleEntities.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            visibleEntities = futureVisibleEntities.get();
        }

        if (!futureVisibleEntities.valid())
        {
            futureVisibleEntities = std::async(std::launch::async, [this, viewProj]() {
                std::vector<entt::entity> result;
                for (auto [entity, mesh, material, transform] : sceneWorld->reg()->group<MeshComp, MaterialPBRComp, TransformComp>().each())
                {
                    const btVector3 worldPos = transform.transform.getPosition();
                    if (isInFrustumClipSpace(worldPos, 5.0f, viewProj))
                        result.push_back(entity);
                }
                return result;
            });
        }

        for (entt::entity entity : visibleEntities)
        {
            auto& mesh = sceneWorld->reg()->get<MeshComp>(entity);
            auto& material = sceneWorld->reg()->get<MaterialPBRComp>(entity);
            auto& transform = sceneWorld->reg()->get<TransformComp>(entity);

            transform.transform.getOpenGLMatrix(tm);
            glUniformMatrix4fv(sceneWorld->shaderMain->model, 1, GL_FALSE, tm);

            if (mesh.VAO != lastVAO) {
                lastVAO = mesh.VAO;
                glBindVertexArray(mesh.VAO);
            }

            auto bindTex = [this](GLuint& last, GLuint current, int unit) {
                if (last != current) {
                    last = current;
                    this->glActiveTexture(GL_TEXTURE0 + unit);
                    this->glBindTexture(GL_TEXTURE_2D, current);
                }
            };

            bindTex(lastAlbedo, material.albedoId, 0);
            bindTex(lastNormal, material.normalId, 1);
            bindTex(lastMetal,  material.metallicId, 2);
            bindTex(lastRough,  material.roughnessId, 3);
            bindTex(lastAO,     material.aoId, 4);

            glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, 0);
        }
    }

    // === Pass 3: Skybox ===
    // Draw the environment cube map as the sky background.
    {
        // Allow skybox to pass depth test even if behind everything else
        glDepthFunc(GL_LEQUAL);

        // Use skybox shader
        sceneWorld->shaderSky->bindShader();

        // Send view matrix (without translation usually in actual cubemap setups)
        glUniformMatrix4fv(sceneWorld->shaderSky->view, 1, GL_FALSE, &viewMat[0][0]);

        // Get cubemap texture and mesh for the skybox
        auto [cubeMapId, mesh] = sceneWorld->reg()->get<CubeMapComp, MeshComp>(sceneWorld->getSkyCube());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

        // Render the cube
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Restore normal depth test function
        glDepthFunc(GL_LESS);
    }

    // === Pass 4: Framebuffer Quad Post-Processing ===
    // Draw full-screen quad with the rendered scene (tone mapping, gamma, etc.)
    {
        // Set viewport back to window size
        glViewport(0, 0, width() * DPIScaleFactor, height() * DPIScaleFactor);

        // Render to default screen framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // We're rendering a 2D screen quad
        glClear(GL_COLOR_BUFFER_BIT);

        sceneWorld->shaderProgramFBScr->bind();
        sceneWorld->shaderProgramFBScr->setTextureUniforms();

        glBindVertexArray(sceneWorld->renderQuad->VAO);

        // Bind the color texture from the framebuffer
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

        // Draw the full-screen quad
        glDrawElements(GL_TRIANGLES, sceneWorld->renderQuad->getIndicesSize(), GL_UNSIGNED_INT, 0);
    }

    // === Pass 5: Shadow Map Debug Quad ===
    // Show the depth texture used for shadow mapping in a small overlay.
    {
        // Show in corner of screen
        glViewport(0, 0, widthShadow / 8, heightShadow / 8);

        // Use default framebuffer again
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        sceneWorld->shaderDebugQuad->bind();
        sceneWorld->shaderDebugQuad->setTextureUniforms();

        glBindVertexArray(sceneWorld->renderQuad->VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);

        // Draw the shadow map texture on screen
        glDrawElements(GL_TRIANGLES, sceneWorld->renderQuad->getIndicesSize(), GL_UNSIGNED_INT, 0);
    }

    // After paintGL draw is finished
    if (visibleEntitiesReady)
    {
        visibleEntitiesReady = false;
        visibleEntitiesFuture = std::async(std::launch::async, [this]() {
            auto mainGroup = sceneWorld->reg()->group<MeshComp, MaterialPBRComp, TransformComp>();

            std::vector<entt::entity> tempList;
            glm::mat4 viewMat = sceneWorld->reg()->get<TransformComp>(camera).transform.getInverseTransformMatrix();
            glm::mat4 viewProj = projectionMat * viewMat;

            for (auto [entity, mesh, material, transform] : mainGroup.each())
            {
                const btVector3 worldPos = transform.transform.getPosition();
                if (isInFrustumClipSpace(worldPos, 5.0f, viewProj)) {
                    tempList.push_back(entity);
                }
            }
            nextVisibleEntities = std::move(tempList);
            visibleEntitiesReady = true;
        });
    }
    // === Performance Metrics ===
    // Track frame render time and display FPS every 100 frames
    {
        count++;
        nanoSec += deltaTimer.nsecsElapsed();

        if (count >= 100) {
            qDebug() << "Avg Frame Time:" << ((double)nanoSec) / (count * 1e9) << "s";
            qDebug() << "FPS:" << ((double)count * 1e9) / nanoSec;
            count = 0;
            nanoSec = 0;
            //printlog
        }

        // Calculate delta time for next frame
        deltaTime = (double)deltaTimer.nsecsElapsed() / 1e9;
        deltaTimer.restart();
    }
}


////TODO: Camera to first rate citizen (add as class member pointer)
// bool GLWindow::isInCameraFrustumAndDistance(TransformComp &cameraTransformComp, TransformComp &actor)
// {
//     const float offsetBehindCamera = 12.0f; // units behind the camera to allow

//     const btVector3 forward   = -cameraTransformComp.transform.forward().normalized();
//     const btVector3 cameraPos = cameraTransformComp.transform.getPosition();
//     const btVector3 offsetPos = cameraPos + forward * offsetBehindCamera;

//     const btVector3 actorPos = actor.transform.getPosition();
//     const btVector3 toActor  = actorPos - offsetPos;

//     if (toActor.dot(forward) <= 0.0f)
//         return false;

//     // Optional max distance
//     if (toActor.length2() > 10.0f * 10.0f)
//         return false;

//     return true;
// }
// bool GLWindow::isInCameraFrustumAndDistance(TransformComp &cameraTransformComp, TransformComp &actor)
// {
//     btVector3 forward = cameraTransformComp.transform.forward();
//     btVector3 offsetPos = cameraTransformComp.transform.getPosition() + (-forward * -1.0f);
//     btVector3 relativPos=actor.transform.getPosition() - offsetPos;

//    btScalar nearPlaneDist=forward.dot( relativPos );
//    if(nearPlaneDist > 0 || relativPos.length2()>200*200 )
//        return false;


//     // Camera &camPersp = sceneWorld->reg()->get<CameraComp>(camera);
//     // if( //relativPos.length2()>200*200   ||
//     //     relativPos.angle(-forward) > qDegreesToRadians(camPersp.fov * aspectFowMult))
//     //     return false;
//     return true;
// }

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
        sceneWorld->DetachFromParent(camera);
    }
    if(keys[Qt::Key_2])
    {
        //print log in console sceneWorld->reg()->get<TransformComp>(camera)
        Transform t = sceneWorld->reg()->get<TransformComp>(camera).transform.getPosition();

        // Re-parent the camera to the light
        sceneWorld->AttachToParent(camera, light);

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
    //    qDebug() << "x: " << mouseEvent->globalPos().x()  << "y: " << misInFrustumClipSpaceouseEvent->globalPos().y();
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


bool GLWindow::isInFrustumClipSpace(const btVector3& worldPos, float radius, const glm::mat4& viewProj)
{
    glm::vec4 clipPos = viewProj * glm::vec4(worldPos.x(), worldPos.y(), worldPos.z(), 1.0f);
    float w = clipPos.w;
    return clipPos.x > -w - radius && clipPos.x < w + radius &&
           clipPos.y > -w - radius && clipPos.y < w + radius &&
           clipPos.z > -w - radius && clipPos.z < w + radius;
}

// bool GLWindow::isInFrustumClipSpace(const btVector3& worldPos, float radius, const glm::mat4& projection)
// {
//     // Build ViewProjection matrix from current camera
//     const glm::mat4 view = sceneWorld->reg()->get<TransformComp>(camera).transform.getInverseTransformMatrix();
//     const glm::mat4 viewProj = projection * view;

//     // Transform world position to clip space
//     glm::vec4 clipPos = viewProj * glm::vec4(worldPos.x(), worldPos.y(), worldPos.z(), 1.0f);

//     float w = clipPos.w;
//     float r = radius;

//     // Check if the transformed sphere is within clip space (-w to w)
//     return clipPos.x > -w - r && clipPos.x < w + r &&
//            clipPos.y > -w - r && clipPos.y < w + r &&
//            clipPos.z > -w - r && clipPos.z < w + r;
// }
