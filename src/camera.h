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

    //    glm::mat4 getViewMatrix(void) const;
//    Transform &moveForward(const float f);
//    glm::vec3 getForward(void) const;
//    virtual glm::mat4 getProjectionMatrix(void) const = 0;
//    inline virtual const char *getType(void) { return "CAMERA"; }
};
