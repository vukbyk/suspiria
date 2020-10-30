#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

Transform::Transform(const glm::vec3 &position,
                     const glm::quat &rotation,
                     const glm::vec3 &scale)
{
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

Transform::~Transform(void)
{
}

Transform &Transform::rotate(const float angle, const glm::vec3 &axis)
{
    rotation = glm::rotate(rotation, angle, axis);
    return *this;
}

Transform &Transform::rotate(const glm::quat rot)
{
    rotation *= rot;
    return *this;
}

Transform &Transform::rotate(const glm::vec3 rot)
{
    rotation *= glm::quat(rot);
    return *this;
}

Transform &Transform::addYawPitch(const glm::vec3 rot)
{
    glm::quat qPitch = glm::angleAxis(rot.x, right());
    glm::quat qYaw =  glm::angleAxis(rot.y, glm::vec3(0, 1, 0));//up());
//    glm::quat qRoll = glm::angleAxis(z, glm::vec3(0, 0, 1));
//    glm::vec3 eul = glm::eulerAngles(rotation);
    rotation *= qPitch * qYaw; // * qRoll;
    return *this;
}

Transform &Transform::mulScale(const float scale)
{
    return setScale(getScale() * scale);
}

Transform &Transform::mulScale(const glm::vec3 &scale)
{
    return setScale(getScale() * scale);
}

Transform &Transform::translate(const glm::vec3 &position)
{
    return setPosition(this->position + position);
}

Transform &Transform::setPosition(const glm::vec3 &position)
{
    this->position = position;
    return *this;
}

Transform &Transform::setScale(const glm::vec3 &scale)
{
    this->scale = scale;
    return *this;
}

Transform &Transform::setRotation(const glm::vec3 &axis, float angle)
{
    rotation = glm::angleAxis(angle, axis);
    return *this;
}

Transform &Transform::setRotation(const glm::quat &rotation)
{
    this->rotation = rotation;
    return *this;
}

glm::vec3 Transform::getPosition(void) const
{
    return position;
}

glm::vec3 Transform::getScale(void) const
{
    return scale;
}

glm::quat Transform::getRotation(void) const
{
    return rotation;
}

glm::mat4 Transform::getTransformMatrix(void) const
{
    return glm::translate(position) * glm::toMat4(rotation) * glm::scale(scale);
}

glm::mat4 Transform::getCameraTransformMatrix(void) const
{
    return glm::toMat4(rotation) * glm::translate(position);
}

glm::vec4 Transform::getDirection(void) const
{
    return rotation * glm::vec4(0, 0, 1, 0);
}

glm::vec3 Transform::forward() const
{
    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 0.0, -1.0));
    return dir;
}

glm::vec3 Transform::right() const
{
    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(1.0, 0.0, 0.0));
    return dir;
}

glm::vec3 Transform::up() const
{
    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 1.0, 0.0));
    return dir;
}

Transform &Transform::moveForward(const float f)
{
    return translate(forward() * f );
}

Transform &Transform::moveRight(const float f)
{
    return translate(right() * f );
}

Transform &Transform::moveUp(const float f)
{
    return translate(up() * f );
}
