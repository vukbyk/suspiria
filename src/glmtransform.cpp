#include "glmtransform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/matrix_decompose.hpp>


//btScalar Transform::tm[16];

GLMTransform::GLMTransform(const GLMTransform &val)
{
    transform = val.transform;
}


GLMTransform::GLMTransform(const btTransform &val)
{
    transform = bulletToGlm(val);
}

GLMTransform::GLMTransform(const btVector3 &aPosition, const btQuaternion &aRotation)
    :transform(1)
{
    transform = glm::toMat4(bulletToGlm(aRotation));
    transform[3][0] = aPosition.x();
    transform[3][1] = aPosition.y();
    transform[3][2] = aPosition.z();
}

GLMTransform::GLMTransform(const glm::vec3 &aPosition
                    ,const glm::quat &aRotation
                    ,const glm::vec3 &scale):transform(1)
{
    transform = glm::toMat4(aRotation);
    transform[3][0] = aPosition.x;
    transform[3][1] = aPosition.y;
    transform[3][2] = aPosition.z;
    glm::scale(transform, scale);
}

GLMTransform::~GLMTransform(void)
{
}

glm::mat4 GLMTransform::getTransform() const
{
    return transform;
}

void GLMTransform::setTransform(const glm::mat4 &value)
{
    transform = value;
}

void GLMTransform::getOpenGLMatrix(btScalar *tm)
{
    tm = glm::value_ptr(transform);
}

btScalar *GLMTransform::getOpenGLMatrix()
{
    return glm::value_ptr(transform);
}

glm::mat4 GLMTransform::getTransformMatrix(void) const
{
    return transform;
}

glm::mat4 GLMTransform::getCameraTransformMatrix(void) const
{
    glm::mat4 viewMatrix(1);
    viewMatrix = glm::mat3x3(transform);
    viewMatrix = glm::inverse(viewMatrix);
    glm::vec3 pos(transform[3]);
    viewMatrix [3][0] = -1.0f * transform[3][0] ;
    viewMatrix [3][1] = -1.0f * transform[3][1] ;
    viewMatrix [3][2] = -1.0f * transform[3][2] ;
//    glm::translate(viewMatrix, glm::vec3(-transform[3].x, -transform[3].y, -transform[3].z));

    return viewMatrix;
}

btVector3 GLMTransform::getPosition() const
{
    glm::vec3 pos(transform[3]);
    return glmToBullet(pos);
}

glm::vec3 GLMTransform::getGLMPosition(void) const
{
    return glm::vec3(transform[3]);
}

GLMTransform &GLMTransform::setPosition(const btVector3 &position)
{
    transform[0][0] = position.x();
    transform[0][1] = position.y();
    transform[0][2] = position.z();
    return *this;
}

GLMTransform &GLMTransform::setOrigin(const btVector3 &position)
{
    transform[0][0] = position.x();
    transform[0][1] = position.y();
    transform[0][2] = position.z();
    return *this;
}

GLMTransform &GLMTransform::translate(const btVector3 &position)
{
    glm::translate(transform, bulletToGlm(position));
    return *this;
}

GLMTransform &GLMTransform::setPosition(const glm::vec3 &position)
{
    transform[0][0] = position.x;
    transform[0][1] = position.y;
    transform[0][2] = position.z;
    return *this;
}

GLMTransform &GLMTransform::translate(const glm::vec3 &position)
{
//    return setPosition(this->position + position);
//    btVector3 v = glmToBullet(position) + transform.getOrigin();
//    transform.setOrigin(v);
    glm::translate(transform, position);
    return *this;

}

btQuaternion GLMTransform::getRotation() const
{
    glm::quat glmRot(transform);
    btQuaternion btRot = glmToBullet(glmRot);
    return btRot;
}

glm::quat GLMTransform::getGLMRotation(void) const
{
    glm::quat rot(transform);
    return rot;
}

GLMTransform &GLMTransform::setRotation(const btQuaternion &rotation)
{
    glm::quat glmRot = bulletToGlm(rotation);
    glm::mat3 mat3(glmRot);
    transform = mat3;
    return *this;
}

GLMTransform &GLMTransform::setRotation(const btVector3 &aAxis, float aAngle)
{
    glm::quat rot(aAngle, bulletToGlm(aAxis));
    glm::mat3 mat3(rot);
    transform = mat3;
    return *this;
}



GLMTransform &GLMTransform::setRotation(const glm::quat &rotation)
{
    glm::mat3 mat3(rotation);
    transform = mat3;
    return *this;
}

GLMTransform &GLMTransform::setRotation(const glm::vec3 &aAxis, float aAngle)
{
    glm::quat rot(aAngle, aAxis);
    glm::mat3 mat3(rot);
    transform = mat3;
    return *this;
}

GLMTransform &GLMTransform::rotate(const float aAngle, const btVector3 &aAxis)
{
    glm::quat rot(aAngle, bulletToGlm(aAxis));
    glm::mat3 mat3(rot);
    transform = mat3;
    return *this;
}

GLMTransform &GLMTransform::rotate(const btQuaternion aRot)
{
    glm::mat3 mat3(bulletToGlm( aRot) );
    transform = mat3;
    return *this;
}

GLMTransform &GLMTransform::rotate(const btVector3 euler)
{
    glm::quat rot(bulletToGlm(euler));
    glm::mat3 mat3(rot);
    transform = mat3;
    return *this;
}

GLMTransform &GLMTransform::addYawPitch(const btVector3 aRot)
{
    btQuaternion qPitch(right(), aRot.x());
    btQuaternion qYaw(btVector3(0, 1, 0), aRot.y());
    btQuaternion btQuat = qPitch * qYaw ;
    glm::mat4 mat(bulletToGlm(btQuat));
    transform *= mat;
    return *this;
}

GLMTransform &GLMTransform::rotate(const float aAngle, const glm::vec3 &aAxis)
{
    glm::quat rot(aAngle, aAxis);
    glm::mat3 mat3(rot);
    transform = mat3;
    return *this;
}

GLMTransform &GLMTransform::rotate(const glm::quat aRot)
{
    glm::mat3 mat3(aRot);
    transform = mat3;
    return *this;
}

GLMTransform &GLMTransform::rotate(const glm::vec3 euler)
{
    glm::quat rot(euler);
    glm::mat3 mat3(rot);
    transform = mat3;
    return *this;
}

GLMTransform &GLMTransform::addYawPitch(const glm::vec3 aRot)
{
    btQuaternion qPitch(right(), aRot.x);
    btQuaternion qYaw(btVector3(0, 1, 0), aRot.y);
    btQuaternion btQuat = qPitch * qYaw ;
    glm::mat4 mat(bulletToGlm(btQuat));
    transform *= mat;
    return *this;
}

glm::vec3 GLMTransform::getDirectionGLM(void) const
{
    glm::vec3 v(transform[2]);
    return v;
}

btVector3 GLMTransform::forward() const
{
    glm::vec3 v(-transform[2]);
    return glmToBullet(v);
}

btVector3 GLMTransform::right() const
{
    glm::vec3 v(transform[0]);
    return glmToBullet(v);
}

btVector3 GLMTransform::up() const
{
    glm::vec3 v(transform[1]);
    return glmToBullet(v);
}

glm::vec3 GLMTransform::forwardGLM() const
{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 0.0, -1.0));
//    glm::quat rot = bulletToGlm(transform.getRotation());
//    glm::vec3 dir = glm::rotate(glm::inverse(rot), glm::vec3(0.0, 0.0, -1.0));

    glm::vec3 v(transform[2]);
    return v;
}

glm::vec3 GLMTransform::rightGLM() const
{
    glm::vec3 v(transform[0]);
    return v;

}

glm::vec3 GLMTransform::upGLM() const
{
    glm::vec3 v(transform[1]);
    return v;
}

GLMTransform &GLMTransform::moveForward(const float f)
{
    return translate(forward() * f);
}

GLMTransform &GLMTransform::moveRight(const float f)
{
    return translate(right() * f );
}

GLMTransform &GLMTransform::moveUp(const float f)
{
    return translate(up() * f );
}

GLMTransform &GLMTransform::moveForwardGLM(const float f)
{
    return translate(forwardGLM() * f );
}

GLMTransform &GLMTransform::moveRightGLM(const float f)
{
    return translate(rightGLM() * f );
}

GLMTransform &GLMTransform::moveUpGLM(const float f)
{
    return translate(upGLM() * f );
}

glm::vec3 GLMTransform::bulletToGlm(const btVector3& v) const { return glm::vec3(v.getX(), v.getY(), v.getZ()); }

btVector3 GLMTransform::glmToBullet(const glm::vec3& v) const { return btVector3(v.x, v.y, v.z); }

glm::quat GLMTransform::bulletToGlm(const btQuaternion& q) const  { return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ()); }

btQuaternion GLMTransform::glmToBullet(const glm::quat& q) const { return btQuaternion(q.x, q.y, q.z, q.w); }

btMatrix3x3 GLMTransform::glmToBullet(const glm::mat3& m) const { return btMatrix3x3(m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2]); }

// btTransform does not contain a full 4x4 matrix, so this transform is lossy.
// Affine transformations are OK but perspective transformations are not.
btTransform GLMTransform::glmToBullet(const glm::mat4& m) const
{
    glm::mat3 m3(m);
    return btTransform(glmToBullet(m3), glmToBullet(glm::vec3(m[3][0], m[3][1], m[3][2])));
}

glm::mat4 GLMTransform::bulletToGlm(const btTransform& t) const
{
//    glm::mat4 m(glm::mat4::_null);
    glm::mat4 m(1);
    const btMatrix3x3& basis = t.getBasis();
    // rotation
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            m[c][r] = basis[r][c];
        }
    }
    // traslation
    btVector3 origin = t.getOrigin();
    m[3][0] = origin.getX();
    m[3][1] = origin.getY();
    m[3][2] = origin.getZ();
    // unit scale
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[3][3] = 1.0f;
    return m;
}

//glm::vec3 TransformGLM::getScale(void) const
//{
//    return scale;
//}
//Transform &TransformGLM::setScale(const glm::vec3 &scale)
//{
//    this->scale = scale;
//    return *this;
//}
//Transform &TransformGLM::mulScale(const float scale)
//{
//    return setScale(getScale() * scale);
//}
//Transform &TransformGLM::mulScale(const glm::vec3 &scale)
//{
//    return setScale(getScale() * scale);
//}

//!GLM Version

//TransformGLM::Transform(const glm::vec3 &position
//                    ,const glm::quat &rotation)
////                    ,const glm::vec3 &scale)
//{
//    this->position = position;
//    this->rotation = rotation;
////    this->scale = scale;
//}

//TransformGLM::~Transform(void)
//{
//}

//Transform &TransformGLM::rotate(const float angle, const glm::vec3 &axis)
//{
//    rotation = glm::rotate(rotation, angle, axis);
//    return *this;
//}

//Transform &TransformGLM::rotate(const glm::quat rot)
//{
//    rotation *= rot;
//    return *this;
//}

//Transform &TransformGLM::rotate(const glm::vec3 rot)
//{
//    rotation *= glm::quat(rot);
//    return *this;
//}

//Transform &TransformGLM::addYawPitch(const glm::vec3 rot)
//{
//    glm::quat qPitch = glm::angleAxis(rot.x, right());
//    glm::quat qYaw =  glm::angleAxis(rot.y, glm::vec3(0, 1, 0));//up());
////    glm::quat qRoll = glm::angleAxis(z, glm::vec3(0, 0, 1));
////    glm::vec3 eul = glm::eulerAngles(rotation);
//    rotation *= qPitch * qYaw; // * qRoll;
//    return *this;
//}

//Transform &TransformGLM::translate(const glm::vec3 &position)
//{
//    return setPosition(this->position + position);
//}

//Transform &TransformGLM::setPosition(const glm::vec3 &position)
//{
//    this->position = position;
//    return *this;
//}

//Transform &TransformGLM::setRotation(const glm::vec3 &axis, float angle)
//{
//    rotation = glm::angleAxis(angle, axis);
//    return *this;
//}

//Transform &TransformGLM::setRotation(const glm::quat &rotation)
//{
//    this->rotation = rotation;
//    return *this;
//}

//glm::vec3 TransformGLM::getPosition(void) const
//{
//    return position;
//}

//glm::quat TransformGLM::getRotation(void) const
//{
//    return rotation;
//}
////TODO: optimise
//glm::mat4 TransformGLM::getTransformMatrix(void) const
//{
//    return glm::translate(position) * glm::toMat4(rotation);//* glm::scale(scale);
//}

//glm::mat4 TransformGLM::getCameraTransformMatrix(void) const
//{
//    return glm::toMat4(rotation) * glm::translate(position);
//}

//glm::vec4 TransformGLM::getDirection(void) const
//{
//    return rotation * glm::vec4(0, 0, 1, 0);
//}

//glm::vec3 TransformGLM::forward() const
//{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 0.0, -1.0));
//    return dir;
//}

//glm::vec3 TransformGLM::right() const
//{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(1.0, 0.0, 0.0));
//    return dir;
//}

//glm::vec3 TransformGLM::up() const
//{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 1.0, 0.0));
//    return dir;
//}

//Transform &TransformGLM::moveForward(const float f)
//{
//    return translate(forward() * f );
//}

//Transform &TransformGLM::moveRight(const float f)
//{
//    return translate(right() * f );
//}

//Transform &TransformGLM::moveUp(const float f)
//{
//    return translate(up() * f );
//}

////glm::vec3 TransformGLM::getScale(void) const
////{
////    return scale;
////}
////Transform &TransformGLM::setScale(const glm::vec3 &scale)
////{
////    this->scale = scale;
////    return *this;
////}
////Transform &TransformGLM::mulScale(const float scale)
////{
////    return setScale(getScale() * scale);
////}

////Transform &TransformGLM::mulScale(const glm::vec3 &scale)
////{
////    return setScale(getScale() * scale);
////}
