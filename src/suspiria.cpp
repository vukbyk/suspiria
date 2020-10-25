//#include "Suspiria.h"
#include "shaderprogram.h"
#include "glwindow.h"
#include "suspiria.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "spacial.h"
#include "camera.h"
//#include "Logger.h"
#include "vertex.h"
#include "mesh.h"
//#include "Texture.h"
#include "material.h"
#include "model.h"
#include "light.h"
//#include "Actor.h"
#include "assimpload.h"
//#include <string>

#include <glm/gtc/type_ptr.hpp>

using namespace glm;

Suspiria::Suspiria()
{
    //SDL_GL_SetSwapInterval(1);
//    glewExperimental = GL_TRUE;
//    glewInit();

    window.show();//Maximized);
    window.update();

    lastFrame = 0;
    dt=0.000001f;
//    shaderProgram = new ShaderProgram();//vert_src, frag_src);
//    camera = new Camera();
//    light = new Light(shaderProgram);
    light->getTransform().setPosition(vec3(0.0, 0.0, -4.46));
//    addChild(light);
}

Suspiria::~Suspiria()
{
//    for (auto Entity: children)
//    {
//        delete Entity;
//    }
}

void Suspiria::init()
{
    viewAng = 60.0f;
    moveSpeed = strafeSpeed = upSpeed = 0;
//    lastFrame = SDL_GetPerformanceCounter();
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
//    glEnable(GL_FRAMEBUFFER_SRGB);
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
    shaderProgram->link();

////    Material vulutureMat( "vulture.png", "Vulture_Diffuse.alpha_normal.jpg");
///     Model *room = new Model("cube.obj", "brickwall.jpg", "brickwall_normal.jpg");//, "AncientUgandan.png");
    Model *room = new Model("cube.obj", "brickwall.jpg", "brickwall_normal.jpg");
    room->getTransform().setPosition(vec3( 0.0, 0.0, 0.0));
    room->getTransform().setScale( vec3(30.0, 30.0, 30.0));
//    addChild(room);

//    Model *cube = new Model("cube.obj", "brickwall.jpg", "brickwall_normal.jpg");//, "AncientUgandan.png");
//    cube->getTransform().setPosition(vec3( 0.0, 0.0, -5.0));
//    addChild(cube);

    Model *vul = new Model("vulture.obj", "vulture.png", "Vulture_Diffuse.alpha_normal.jpg");
    vul->getTransform().setPosition(vec3(-2.0, 0.0, -3.0));
//    addChild(vul);

    Model *vult = new Model("vulture.obj", "vulture.png", "Vulture_Diffuse.alpha_normal.jpg");
    vult->getTransform().setPosition(vec3(-2.0, 0.0, 3.0));
//    addChild(vult);

//    Model *model = new Model("AncientUgandan.obj", "AncientUgandan.png");
//    model->getTransform().setPosition(vec3( 2.0, 0.0, -3.0));
//    addChild(model);

//    Model *sphare = new Model("megamars1od4.obj", "mars8kcompress.png");//, "marsnormal.png");
    Model *sphare = new Model("sphare.obj", "mars8kcompress.png", "marsnormal.png");
    sphare->getTransform().setPosition(vec3( 6.0, 0.0, -2.0));
//    sphare->getTransform().setScale( vec3(0.001, 0.001, 0.001));
//    addChild(sphare);

}

void Suspiria::renderAll()
{

    GLint texLoc;
    texLoc = glGetUniformLocation(shaderProgram->programId(), "albedoTexture");
    glUniform1i(texLoc, 0);

    texLoc = glGetUniformLocation(shaderProgram->programId(), "normalTexture");
    glUniform1i(texLoc, 1);

    //Model

//    for (auto Entity: children)
//    {
//        glm::mat4 modelTempMat = Entity->getTransform().getTransformMatrix();
//        glUniformMatrix4fv(modelTransMat, 1, GL_FALSE, &modelTempMat[0][0]);//&mtm[0][0]);
//        Entity->renderAll();
//    }
}

void Suspiria::loop()
{
//    double time=0;

    shaderProgram->bind(); //glUseProgram(shader->getProgram());

//    while (!window.quit)
//    {
//        setDeltaTime();
//        time += dt;
////        viewAng += window.getInput()->getMouseWheel().y/5000;

//        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//        glClearColor(0.0f, 0.0, 0.0, 1.0);

//        setProjectionMat();

//        //Camera&View
//        //TODO: Camera to be Entity
//        camera->getTransform().moveForward(moveSpeed * dt);
//        camera->getTransform().moveRight(strafeSpeed * dt);
//        camera->getTransform().moveUp(upSpeed * dt);

//        light->getTransform().moveForward(lmoveSpeed * dt);
//        light->getTransform().moveRight(lstrafeSpeed * dt);
//        light->getTransform().moveUp(lupSpeed * dt);

        setViewMat();
        light->getTransform().getTransformMatrix();
        renderAll();

////        model->renderAll();
////        model2->renderAll();

//        window.tick();
//        window.swapBuffer();

////        std::cout << "Time: " << (int)time<< "  DT: " << dt * 1000.0f << "ms"<< " FPS:"<< 1.0/dt << std::endl;
////        SDL_Delay(5-dt);
//    }
}


void Suspiria::setDeltaTime()
{
//    currentFrame = SDL_GetPerformanceCounter();
//    dt = (double)( ( currentFrame - lastFrame) //* 10000    // ~ (curentFrameTime - lastFrameTime)
//              / (double)SDL_GetPerformanceFrequency() );// * 0.0001;
//    lastFrame = currentFrame;
}

void Suspiria::setProjectionMat()
{
    const float w = window.geometry().width();
    const float h = window.geometry().height();
    float proportion =w/h;
    mat4 projection = perspective(glm::degrees(viewAng), proportion, 0.01f, 300.0f);
    GLint projectionid = glGetUniformLocation(shaderProgram->programId(), "projection");
    glUniformMatrix4fv(projectionid, 1, GL_FALSE, &projection[0][0]);
}

void Suspiria::setViewMat()
{

    GLint camPos=glGetUniformLocation(shaderProgram->programId(),"viewPos");;
    const vec3 v = camera->getTransform().getPosition();
    glUniform3fv(camPos, 1, &v[0]);

    GLint view = glGetUniformLocation(shaderProgram->programId(),"view");
//    window.input.getKeyModState();

//    if(window.getInput()->getMouseWheel().y)
//        camera->getTransform().moveForward(window.getInput()->getMouseWheel().y);

//    vec2 rotator(window.getInput()->getMouseDelta().y * glm::radians(0.5f), window.getInput()->getMouseDelta().x * glm::radians(0.5f));
//    camera->getTransform().addYawPitch(vec3(rotator,0));

    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera->getTransform().getCameraTransformMatrix()) );//&mtm[0][0]);
}
