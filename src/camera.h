#pragma once

#include "spacial.h"

#include <glm/glm.hpp>

class Camera : public Spacial
{
public:
    glm::mat4 getViewMatrix(void) const;
    virtual ~Camera(void) {}

//    Transform &moveForward(const float f);
//    glm::vec3 getForward(void) const;
//    virtual glm::mat4 getProjectionMatrix(void) const = 0;
//    inline virtual const char *getType(void) { return "CAMERA"; }
};
