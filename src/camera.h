#pragma once

#include <QOpenGLExtraFunctions>

#include <glm/glm.hpp>



class Camera
{
public:
    GLfloat fov    ;//= 45.0f;
    GLfloat zNear  ;//= 0.3f;
    GLfloat zFar   ;//= 100.0f;
    GLfloat aspect ;//= 0.66;
public:
    Camera(const GLfloat aNear, const GLfloat aFar, const GLfloat aFov, const GLfloat aAspect);
    virtual ~Camera() {}
};
