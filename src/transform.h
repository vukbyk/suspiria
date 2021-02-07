#pragma once

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>


class Transform
{
protected:
//    static btScalar tm[16];
    btTransform transform;
//    glm::vec3 position;
//    glm::quat rotation;
//    glm::vec3 scale;

public:
    Transform(const btVector3 &aPosition = btVector3(0,0,0)
             ,const btQuaternion &aRotation = btQuaternion(0.0, 0.0, 0.0) );

    Transform(const glm::vec3 &position// = glm::vec3(0)
             ,const glm::quat &rotation);// = glm::quat(glm::vec3(0.0, 0.0, 0.0)));
//             ,const glm::vec3 &scale = glm::vec3(1.0f));
    ~Transform(void);

    void getOpenGLMatrix(btScalar *tm);
    btScalar *getOpenGLMatrix();
    glm::mat4 getTransformMatrix(void) const;
    glm::mat4 getCameraTransformMatrix() const;


    btVector3 getPosition(void) const;
    glm::vec3 getGLMPosition(void) const;
    Transform &setPosition(const btVector3 &position);
    Transform &setOrigin(const btVector3 &position);
    Transform &translate(const btVector3 &position);
    Transform &setPosition(const glm::vec3 &position);
    Transform &translate(const glm::vec3 &position);

    btQuaternion getRotation(void) const;
    glm::quat getGLMRotation(void) const;

    Transform &setRotation(const btQuaternion &rotation);
    Transform &setRotation(const btVector3 &aAxis, float aAngle);

    Transform &setRotation(const glm::quat &rotation);
    Transform &setRotation(const glm::vec3 &aAxis, float aAngle);

    Transform &rotate(const float aAngle, const btVector3 &aAxis);
    Transform &rotate(const btQuaternion aRot);
    Transform &rotate(const btVector3 aRot);
    Transform &addYawPitch(const btVector3 aRot);

    Transform &rotate(const float aAngle, const glm::vec3 &aAxis);
    Transform &rotate(const glm::quat aRot);
    Transform &rotate(const glm::vec3 aRot);
    Transform &addYawPitch(const glm::vec3 aRot);

    glm::vec3 getDirectionGLM(void) const;
    btVector3 forward(void) const;
    btVector3 right(void) const;
    btVector3 up(void) const;

    glm::vec3 forwardGLM(void) const;
    glm::vec3 rightGLM(void) const;
    glm::vec3 upGLM(void) const;

    Transform &moveForward(const float f);
    Transform &moveRight(const float f);
    Transform &moveUp(const float f);

    Transform &moveForwardGLM(const float f);
    Transform &moveRightGLM(const float f);
    Transform &moveUpGLM(const float f);

    glm::vec3 bulletToGlm(const btVector3 &v)const;
    btVector3 glmToBullet(const glm::vec3 &v)const;
    glm::quat bulletToGlm(const btQuaternion &q) const;
    btQuaternion glmToBullet(const glm::quat &q)const;
    btMatrix3x3 glmToBullet(const glm::mat3 &m)const;
    btTransform glmToBullet(const glm::mat4 &m)const;
    glm::mat4 bulletToGlm(const btTransform &t)const;

//    glm::vec3 getScale(void) const;
//    Transform &setScale(const glm::vec3 &scale);
//    Transform &mulScale(const float scale);
//    Transform &mulScale(const glm::vec3 &scale);
};


//!GLM Version

//class Transform
//{
//protected:
//    glm::vec3 position;
//    glm::quat rotation;
////    glm::vec3 scale;

//public:
//    Transform(const glm::vec3 &position = glm::vec3(0)
//             ,const glm::quat &rotation = glm::quat(glm::vec3(0.0, 0.0, 0.0)));
////             ,const glm::vec3 &scale = glm::vec3(1.0f));
//    ~Transform(void);

//    Transform &rotate(const float angle, const glm::vec3 &axis);
//    Transform &rotate(const glm::quat rot);
//    Transform &rotate(const glm::vec3 rot);
//    Transform &addYawPitch(const glm::vec3 rot);
//    Transform &translate(const glm::vec3 &position);

//    Transform &setPosition(const glm::vec3 &position);

//    Transform &setRotation(const glm::quat &rotation);
//    Transform &setRotation(const glm::vec3 &axis, float w);

//    glm::vec3 getPosition(void) const;

//    glm::quat getRotation(void) const;

//    glm::mat4 getTransformMatrix(void) const;
//    glm::mat4 getCameraTransformMatrix() const;

//    glm::vec4 getDirection(void) const;

//    glm::vec3 forward(void) const;
//    glm::vec3 right(void) const;
//    glm::vec3 up(void) const;
//    Transform &moveForward(const float f);
//    Transform &moveRight(const float f);
//    Transform &moveUp(const float f);


////    glm::vec3 getScale(void) const;
////    Transform &setScale(const glm::vec3 &scale);
////    Transform &mulScale(const float scale);
////    Transform &mulScale(const glm::vec3 &scale);
//};

