#pragma once

//#include "Shaders.h"
#include "glwindow.h"
//#include "Vertex.h"
#include "spacial.h"
#include <glm/glm.hpp>
#include <vector>

class Suspiria: public Spacial
{
    GLWindow window;
    class ShaderProgram *shaderProgram;
    float dt;

    float viewAng=0;
    float moveSpeed=0;
    float strafeSpeed=0;
    float upSpeed=0;

    float lviewAng=0;
    float lmoveSpeed=0;
    float lstrafeSpeed=0;
    float lupSpeed=0;
//    glm::vec3 cam;
    class Camera *camera;
    class Light *light;
//    std::vector<std::shared_ptr<Entity>> children;
//    class Model *model;
//    class Model *model2;

//    GLuint VAO, VBO, EBO, VBO2, EBO2, VAO2;
    GLint64 lastFrame, currentFrame;

public:
    Suspiria();
    ~Suspiria();

    void init();
    virtual void renderAll();
    void loop();
    void setDeltaTime();
    void setProjectionMat();
//    //TODO: Make regular entity
    void setViewMat();

};
