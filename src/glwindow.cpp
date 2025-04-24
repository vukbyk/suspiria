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
    // Temporary matrix used to upload transforms (from Bullet to OpenGL)
    btScalar tm[16];

    // === Pass 1: Shadow Depth Map ===
    // Render the scene from the light's point of view to a depth texture.
    // This will later be used to determine if a fragment is in shadow.
    glm::mat4 lightViewMat; // View matrix from the light's perspective
    {
        // Set viewport to shadow map resolution
        glViewport(0, 0, widthShadow, heightShadow);

        // Bind the framebuffer used for shadow depth map
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);


        // Attach the depth texture to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);

        // We don't need to write or read color values, just depth
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        // Enable polygon offset to reduce shadow acne
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.0f, 4.0f);  // Tweak these values if needed

        // Clear previous frame's depth data
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable depth testing to store only the closest fragments
        glEnable(GL_DEPTH_TEST);

        // Cull front faces to reduce shadow artifacts like acne
        glCullFace(GL_FRONT);

        // Use the shadow shader program
        sceneWorld->shaderShadow->bindShader();

        // Create an orthographic projection for the light (good for directional lights)
        orthoLightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, 2.0f, 100.0f);

        // Send the projection matrix to the shader
        glUniformMatrix4fv(sceneWorld->shaderShadow->projection, 1, GL_FALSE, &orthoLightProjection[0][0]);

        {
            TransformComp &camTransform = sceneWorld->reg()->get<TransformComp>(camera);
            TransformComp &lightTransform = sceneWorld->reg()->get<TransformComp>(light);
            lightTransform.transform.setPosition(camTransform.transform.getGLMPosition());
            btVector3 camPos = camTransform.transform.getPosition();
            btVector3 lightPos = camPos + camTransform.transform.forward() * -10.0f;


            // Update light position

            lightTransform.transform.setPosition(lightPos);
            lightTransform.transform.translate( lightTransform.transform.forward().normalize() * 20.0f );
        }



        // Get the light's view matrix (position and orientation)
        TransformComp &lightTransformComp = sceneWorld->reg()->get<TransformComp>(light);
        lightViewMat = lightTransformComp.transform.getInverseTransformMatrix();

        // Upload view matrix to shader
        glUniformMatrix4fv(sceneWorld->shaderShadow->view, 1, GL_FALSE, &lightViewMat[0][0]);

        // Bind any additional textures required by the shader
        sceneWorld->shaderShadow->setTextureUniforms();

        // Loop through all shadow-casting entities
        auto shadowGroup = sceneWorld->reg()->group<MeshComp, MaterialPBRComp, TransformComp>();
        GLuint lastVAO = 0;

        for (auto [entity, mesh, material, transform] : shadowGroup.each()) {
            // Convert transform to OpenGL format and upload
            transform.transform.getOpenGLMatrix(tm);
            glUniformMatrix4fv(sceneWorld->shaderShadow->model, 1, GL_FALSE, tm);

            // Only bind VAO if it changed (to avoid unnecessary state change)
            if (mesh.VAO != lastVAO) {
                lastVAO = mesh.VAO;
                glBindVertexArray(mesh.VAO);
            }

            // Draw the mesh (indexed draw)
            glDrawElements(GL_TRIANGLES, mesh.indicesSize, GL_UNSIGNED_INT, 0);
        }
    }

    // === Pass 2: Main PBR Scene ===
    // Render the actual scene using physically-based rendering.
    // This includes lighting, shadows, and environment reflections.
    glm::mat4 viewMat; // View matrix from the camera
    {
        // Set viewport to window size
        glViewport(0, 0, width() * DPIScaleFactor * UPScale, height() * DPIScaleFactor * UPScale);

        // Render into our offscreen framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK); // Backface culling for performance

        // Bind main PBR shader and set texture bindings
        sceneWorld->shaderMain->bindShader();
        sceneWorld->shaderMain->setTextureUniforms();

        // Get current camera transform (handle parent-child transform if needed)
        Transform* cameraTransform = &sceneWorld->reg()->get<TransformComp>(camera).transform;
        if (sceneWorld->reg()->all_of<ParentComponent>(camera)) {
            auto& parent = sceneWorld->reg()->get<ParentComponent>(camera);
            if (parent.parentTransform)
                cameraTransform = parent.parentTransform;
        }

        // Generate view matrix (camera's inverse transform)
        viewMat = cameraTransform->getInverseTransformMatrix();
        glUniformMatrix4fv(sceneWorld->shaderMain->view, 1, GL_FALSE, &viewMat[0][0]);

        // Calculate light space matrix for shadows
        Transform& lightTransform = sceneWorld->reg()->get<TransformComp>(light);
        glm::mat4 lightSpaceMat = orthoLightProjection * lightViewMat;
        glUniformMatrix4fv(sceneWorld->shaderMain->light, 1, GL_FALSE, glm::value_ptr(lightTransform.getTransformMatrix()));
        glUniformMatrix4fv(sceneWorld->shaderMain->lightSpaceMat, 1, GL_FALSE, glm::value_ptr(lightSpaceMat));

        // Bind shadow depth texture to texture unit 5
        glActiveTexture(GL_TEXTURE0 + 5);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);

        // Bind environment cube map to texture unit 6
        GLuint cubeMapId = sceneWorld->reg()->get<CubeMapComp>(light).cubeTextureId;
        glActiveTexture(GL_TEXTURE0 + 6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);

        // Cache last-used textures to avoid rebinding
        GLuint lastVAO = 0, lastAlbedo = 0, lastNormal = 0, lastMetal = 0, lastRough = 0, lastAO = 0;
        auto mainGroup = sceneWorld->reg()->group<MeshComp, MaterialPBRComp, TransformComp>();

        // Helper to avoid redundant texture binds
        auto bindTex = [this](GLuint& last, GLuint current, int unit) {
            if (last != current) {
                last = current;
                this->glActiveTexture(GL_TEXTURE0 + unit);
                this->glBindTexture(GL_TEXTURE_2D, current);
            }
        };

        // Render each object in the scene
        for (auto [entity, mesh, material, transform] : mainGroup.each()) {
            transform.transform.getOpenGLMatrix(tm);
            glUniformMatrix4fv(sceneWorld->shaderMain->model, 1, GL_FALSE, tm);

            if (mesh.VAO != lastVAO) {
                lastVAO = mesh.VAO;
                glBindVertexArray(mesh.VAO);
            }

            // Bind each material texture if not already bound
            bindTex(lastAlbedo, material.albedoId, 0);
            bindTex(lastNormal, material.normalId, 1);
            bindTex(lastMetal,  material.metallicId, 2);
            bindTex(lastRough,  material.roughnessId, 3);
            bindTex(lastAO,     material.aoId, 4);

            // Draw the mesh with full PBR
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
        }

        // Calculate delta time for next frame
        deltaTime = (double)deltaTimer.nsecsElapsed() / 1e9;
        deltaTimer.restart();
    }
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
