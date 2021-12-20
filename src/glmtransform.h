#pragma once

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>


class GLMTransform
{
protected:
//    static btScalar tm[16];
//    btTransform transform;

    glm::mat4 transform;

    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;


public:
    GLMTransform(const GLMTransform &val);
    GLMTransform(const btTransform &val);
    GLMTransform(const btVector3 &aPosition = btVector3(0,0,0)
             ,const btQuaternion &aRotation = btQuaternion(0.0, 0.0, 0.0) );

    GLMTransform(const glm::vec3 &position// = glm::vec3(0)
             ,const glm::quat &rotation// = glm::quat(glm::vec3(0.0, 0.0, 0.0)));
             ,const glm::vec3 &scale = glm::vec3(1.0f));
    ~GLMTransform();

    glm::mat4 getTransform() const;
    void setTransform(const glm::mat4 &value);
    void getOpenGLMatrix(btScalar *tm);

    btScalar *getOpenGLMatrix();
    glm::mat4 getTransformMatrix() const;
    glm::mat4 getCameraTransformMatrix() const;


    btVector3 getPosition() const;
    glm::vec3 getGLMPosition() const;
    GLMTransform &setPosition(const btVector3 &position);
    GLMTransform &setOrigin(const btVector3 &position);
    GLMTransform &translate(const btVector3 &position);
    GLMTransform &setPosition(const glm::vec3 &position);
    GLMTransform &translate(const glm::vec3 &position);

    btQuaternion getRotation() const;
    glm::quat getGLMRotation() const;

    GLMTransform &setRotation(const btQuaternion &rotation);
    GLMTransform &setRotation(const btVector3 &aAxis, float aAngle);

    GLMTransform &setRotation(const glm::quat &rotation);
    GLMTransform &setRotation(const glm::vec3 &aAxis, float aAngle);

    GLMTransform &rotate(const float aAngle, const btVector3 &aAxis);
    GLMTransform &rotate(const btQuaternion aRot);
    GLMTransform &rotate(const btVector3 aRot);
    GLMTransform &addYawPitch(const btVector3 aRot);

    GLMTransform &rotate(const float aAngle, const glm::vec3 &aAxis);
    GLMTransform &rotate(const glm::quat aRot);
    GLMTransform &rotate(const glm::vec3 euler);
    GLMTransform &addYawPitch(const glm::vec3 aRot);

    glm::vec3 getDirectionGLM() const;
    btVector3 forward() const;
    btVector3 right() const;
    btVector3 up() const;

    glm::vec3 forwardGLM() const;
    glm::vec3 rightGLM() const;
    glm::vec3 upGLM() const;

    GLMTransform &moveForward(const float f);
    GLMTransform &moveRight(const float f);
    GLMTransform &moveUp(const float f);

    GLMTransform &moveForwardGLM(const float f);
    GLMTransform &moveRightGLM(const float f);
    GLMTransform &moveUpGLM(const float f);

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

