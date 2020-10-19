#pragma once

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
protected:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

public:
    Transform(const glm::vec3 &position = glm::vec3(0),
              const glm::quat &rotation = glm::quat(glm::vec3(0.0, 0.0, 0.0)),
              const glm::vec3 &scale = glm::vec3(1.0f));
    ~Transform(void);

    Transform &rotate(const float angle, const glm::vec3 &axis);
    Transform &rotate(const glm::quat rot);
    Transform &rotate(const glm::vec3 rot);
    Transform &addYawPitch(const glm::vec3 rot);
    Transform &translate(const glm::vec3 &position);
    Transform &mulScale(const float scale);
    Transform &mulScale(const glm::vec3 &scale);

    Transform &setPosition(const glm::vec3 &position);

    Transform &setScale(const glm::vec3 &scale);

    Transform &setRotation(const glm::quat &rotation);
    Transform &setRotation(const glm::vec3 &axis, float w);

    glm::vec3 getPosition(void) const;
    glm::vec3 getScale(void) const;
    glm::quat getRotation(void) const;

    glm::mat4 getTransformMatrix(void) const;
    glm::mat4 getCameraTransformMatrix() const;

    glm::vec4 getDirection(void) const;

    glm::vec3 forward(void) const;
    glm::vec3 right(void) const;
    glm::vec3 up(void) const;
    Transform &moveForward(const float f);
    Transform &moveRight(const float f);
    Transform &moveUp(const float f);
};
