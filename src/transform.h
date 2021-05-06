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
    Transform(const Transform &val);
    Transform(const btTransform &val);
    Transform(const btVector3 &aPosition = btVector3(0,0,0)
             ,const btQuaternion &aRotation = btQuaternion(0.0, 0.0, 0.0) );

    Transform(const glm::vec3 &position// = glm::vec3(0)
             ,const glm::quat &rotation);// = glm::quat(glm::vec3(0.0, 0.0, 0.0)));
//             ,const glm::vec3 &scale = glm::vec3(1.0f));
    ~Transform();

    void getOpenGLMatrix(btScalar *tm);
    btScalar *getOpenGLMatrix();
    glm::mat4 getTransformMatrix() const;
    glm::mat4 getCameraTransformMatrix() const;


    btVector3 getPosition() const;
    glm::vec3 getGLMPosition() const;
    Transform &setPosition(const btVector3 &position);
    Transform &setOrigin(const btVector3 &position);
    Transform &translate(const btVector3 &position);
    Transform &setPosition(const glm::vec3 &position);
    Transform &translate(const glm::vec3 &position);

    btQuaternion getRotation() const;
    glm::quat getGLMRotation() const;

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

    glm::vec3 getDirectionGLM() const;
    btVector3 forward() const;
    btVector3 right() const;
    btVector3 up() const;

    glm::vec3 forwardGLM() const;
    glm::vec3 rightGLM() const;
    glm::vec3 upGLM() const;

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

//    glm::vec3 getScale() const;
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
//    ~Transform();

//    Transform &rotate(const float angle, const glm::vec3 &axis);
//    Transform &rotate(const glm::quat rot);
//    Transform &rotate(const glm::vec3 rot);
//    Transform &addYawPitch(const glm::vec3 rot);
//    Transform &translate(const glm::vec3 &position);

//    Transform &setPosition(const glm::vec3 &position);

//    Transform &setRotation(const glm::quat &rotation);
//    Transform &setRotation(const glm::vec3 &axis, float w);

//    glm::vec3 getPosition() const;

//    glm::quat getRotation() const;

//    glm::mat4 getTransformMatrix() const;
//    glm::mat4 getCameraTransformMatrix() const;

//    glm::vec4 getDirection() const;

//    glm::vec3 forward() const;
//    glm::vec3 right() const;
//    glm::vec3 up() const;
//    Transform &moveForward(const float f);
//    Transform &moveRight(const float f);
//    Transform &moveUp(const float f);


////    glm::vec3 getScale() const;
////    Transform &setScale(const glm::vec3 &scale);
////    Transform &mulScale(const float scale);
////    Transform &mulScale(const glm::vec3 &scale);
//};

