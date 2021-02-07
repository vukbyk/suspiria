#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

//btScalar Transform::tm[16];

Transform::Transform(const btVector3 &aPosition, const btQuaternion &aRotation)
    :transform(aRotation, aPosition)
{
    //transform.setIdentity();
}

Transform::Transform(const glm::vec3 &aPosition
                    ,const glm::quat &aRotation
                    /*,const glm::vec3 &scale*/)
{
    transform.setOrigin( glmToBullet(aPosition));
    transform.setRotation( glmToBullet( aRotation));
//    this->scale = scale;
}

Transform::~Transform(void)
{
}


void Transform::getOpenGLMatrix(btScalar *tm)
{
    transform.getOpenGLMatrix(tm);
}

btScalar *Transform::getOpenGLMatrix()
{
    static btScalar tm[16];
    transform.getOpenGLMatrix(tm);
    return tm;
}

glm::mat4 Transform::getTransformMatrix(void) const
{
//    return glm::translate(position) * glm::toMat4(rotation);//* glm::scale(scale);
    return bulletToGlm(transform);
}

glm::mat4 Transform::getCameraTransformMatrix(void) const
{
//    return bulletToGlm(transform);
//    return glm::toMat4(rotation) * glm::translate(position);
    glm::mat4 rot = glm::toMat4( bulletToGlm( btQuaternion (transform.getRotation()) ));
    return rot * glm::translate(bulletToGlm(transform.getOrigin()) );
}

btVector3 Transform::getPosition() const
{
    return transform.getOrigin();
}

glm::vec3 Transform::getGLMPosition(void) const
{
//    return position;
//    return glm::vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
    return bulletToGlm(transform.getOrigin());
}

Transform &Transform::setOrigin(const btVector3 &position)
{
    transform.setOrigin(position);
    return *this;
}

Transform &Transform::translate(const btVector3 &position)
{
    return setPosition(transform.getOrigin() + position);
}

Transform &Transform::setPosition(const btVector3 &position)
{
    transform.setOrigin(position);
    return *this;
}

Transform &Transform::setPosition(const glm::vec3 &position)
{
//    this->position = position;
    transform.setOrigin(glmToBullet(position));
    return *this;
}

Transform &Transform::translate(const glm::vec3 &position)
{
//    return setPosition(this->position + position);
//    btVector3 v = glmToBullet(position) + transform.getOrigin();
//    transform.setOrigin(v);
    return setPosition(bulletToGlm(transform.getOrigin()) + position);
}

btQuaternion Transform::getRotation() const
{
    return transform.getRotation();
}

glm::quat Transform::getGLMRotation(void) const
{
//    return rotation;
    return bulletToGlm(transform.getRotation());
}

Transform &Transform::setRotation(const btQuaternion &rotation)
{
    transform.setRotation(transform.getRotation() * rotation);
    return *this;
}

Transform &Transform::setRotation(const btVector3 &aAxis, float aAngle)
{
    transform.setRotation(btQuaternion(aAxis, aAngle));
    return *this;
}



Transform &Transform::setRotation(const glm::quat &rotation)
{
    transform.setRotation( glmToBullet(rotation) );
    return *this;
}

Transform &Transform::setRotation(const glm::vec3 &aAxis, float aAngle)
{
    transform.setRotation(glmToBullet(glm::quat(aAngle ,aAxis)) );
    return *this;
}

Transform &Transform::rotate(const float aAngle, const btVector3 &aAxis)
{
    transform.setRotation(btQuaternion(aAxis, aAngle));
    return *this;
}

Transform &Transform::rotate(const btQuaternion aRot)
{
    transform.setRotation(transform.getRotation() * aRot);
    return *this;
}

Transform &Transform::rotate(const btVector3 aRot)
{
    btQuaternion quat(aRot.x(), aRot.y(), aRot.z());
    transform.setRotation(transform.getRotation()*quat);
//    rotation *= glm::quat(rot);
    return *this;
}

Transform &Transform::addYawPitch(const btVector3 aRot)
{
//    glm::quat qPitch = glm::angleAxis(aRot.x, right());
//    glm::quat qYaw =  glm::angleAxis(aRot.y, glm::vec3(0, 1, 0));//up());
//    rotation *= qPitch * qYaw; // * qRoll;
    btQuaternion qPitch(right(), aRot.x());
    btQuaternion qYaw(btVector3(0, 1, 0), aRot.y());
    btQuaternion btQuat = qPitch * qYaw ;
    transform.setRotation(transform.getRotation() * btQuat);
    return *this;
}

Transform &Transform::rotate(const float aAngle, const glm::vec3 &aAxis)
{
    btVector3 axis = glmToBullet(aAxis);
    transform.setRotation(btQuaternion(axis, aAngle));
//    rotation = glm::rotate(rotation, angle, axis);
    return *this;
}

Transform &Transform::rotate(const glm::quat aRot)
{
//    rotation *= rot;
    transform.setRotation(transform.getRotation() * glmToBullet(aRot));
    return *this;
}

Transform &Transform::rotate(const glm::vec3 aRot)
{
    btQuaternion quat = glmToBullet(glm::quat(aRot));
    transform.setRotation(transform.getRotation()*quat);
//    rotation *= glm::quat(rot);
    return *this;
}

Transform &Transform::addYawPitch(const glm::vec3 aRot)
{
    glm::quat qPitch = glm::angleAxis(aRot.x, rightGLM());
    glm::quat qYaw =  glm::angleAxis(aRot.y, glm::vec3(0, 1, 0));//up());
//    rotation *= qPitch * qYaw; // * qRoll;
    btQuaternion btQuat = glmToBullet(glm::quat( qPitch * qYaw ));
    transform.setRotation(transform.getRotation() * btQuat);
    return *this;
}

glm::vec3 Transform::getDirectionGLM(void) const
{
//    return rotation * glm::vec4(0, 0, 1, 0);
    return bulletToGlm(transform.getRotation())* glm::vec3(0, 0, 1);
}

btVector3 Transform::forward() const
{
    const btQuaternion rot = transform.getRotation();
    btVector3 dir = btVector3(0.0, 0.0, -1.0);
    dir.rotate(rot.getAxis(), rot.getAngle() );
    return dir;
}

btVector3 Transform::right() const
{
    const btQuaternion rot = transform.getRotation();
    btVector3 dir = btVector3(1.0, 0.0, 0.0);
    dir.rotate(rot.getAxis(), rot.getAngle() );
    return dir;
}

btVector3 Transform::up() const
{
    const btQuaternion rot = transform.getRotation();
    btVector3 dir = btVector3(1.0f, 0.0, -1.0);
    dir.rotate(rot.getAxis(), rot.getAngle() );
    return dir;
}

glm::vec3 Transform::forwardGLM() const
{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 0.0, -1.0));
    glm::quat rot = bulletToGlm(transform.getRotation());
    glm::vec3 dir = glm::rotate(glm::inverse(rot), glm::vec3(0.0, 0.0, -1.0));
    return dir;
}

glm::vec3 Transform::rightGLM() const
{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(1.0, 0.0, 0.0));
    glm::quat rot = bulletToGlm(transform.getRotation());
    glm::vec3 dir = glm::rotate(glm::inverse(rot), glm::vec3(1.0, 0.0, 0.0));
    return dir;
}

glm::vec3 Transform::upGLM() const
{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 1.0, 0.0));
    glm::quat rot = bulletToGlm(transform.getRotation());
    glm::vec3 dir = glm::rotate(glm::inverse(rot), glm::vec3(0.0, 1.0, 0.0));
    return dir;
}

Transform &Transform::moveForward(const float f)
{
    return translate(forward() * f);
}

Transform &Transform::moveRight(const float f)
{
    return translate(right() * f );
}

Transform &Transform::moveUp(const float f)
{
    return translate(up() * f );
}

Transform &Transform::moveForwardGLM(const float f)
{
    return translate(forwardGLM() * f );
}

Transform &Transform::moveRightGLM(const float f)
{
    return translate(rightGLM() * f );
}

Transform &Transform::moveUpGLM(const float f)
{
    return translate(upGLM() * f );
}

glm::vec3 Transform::bulletToGlm(const btVector3& v) const { return glm::vec3(v.getX(), v.getY(), v.getZ()); }

btVector3 Transform::glmToBullet(const glm::vec3& v) const { return btVector3(v.x, v.y, v.z); }

glm::quat Transform::bulletToGlm(const btQuaternion& q) const  { return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ()); }

btQuaternion Transform::glmToBullet(const glm::quat& q) const { return btQuaternion(q.x, q.y, q.z, q.w); }

btMatrix3x3 Transform::glmToBullet(const glm::mat3& m) const { return btMatrix3x3(m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2]); }

// btTransform does not contain a full 4x4 matrix, so this transform is lossy.
// Affine transformations are OK but perspective transformations are not.
btTransform Transform::glmToBullet(const glm::mat4& m) const
{
    glm::mat3 m3(m);
    return btTransform(glmToBullet(m3), glmToBullet(glm::vec3(m[3][0], m[3][1], m[3][2])));
}

glm::mat4 Transform::bulletToGlm(const btTransform& t) const
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

//glm::vec3 Transform::getScale(void) const
//{
//    return scale;
//}
//Transform &Transform::setScale(const glm::vec3 &scale)
//{
//    this->scale = scale;
//    return *this;
//}
//Transform &Transform::mulScale(const float scale)
//{
//    return setScale(getScale() * scale);
//}
//Transform &Transform::mulScale(const glm::vec3 &scale)
//{
//    return setScale(getScale() * scale);
//}

//!GLM Version

//Transform::Transform(const glm::vec3 &position
//                    ,const glm::quat &rotation)
////                    ,const glm::vec3 &scale)
//{
//    this->position = position;
//    this->rotation = rotation;
////    this->scale = scale;
//}

//Transform::~Transform(void)
//{
//}

//Transform &Transform::rotate(const float angle, const glm::vec3 &axis)
//{
//    rotation = glm::rotate(rotation, angle, axis);
//    return *this;
//}

//Transform &Transform::rotate(const glm::quat rot)
//{
//    rotation *= rot;
//    return *this;
//}

//Transform &Transform::rotate(const glm::vec3 rot)
//{
//    rotation *= glm::quat(rot);
//    return *this;
//}

//Transform &Transform::addYawPitch(const glm::vec3 rot)
//{
//    glm::quat qPitch = glm::angleAxis(rot.x, right());
//    glm::quat qYaw =  glm::angleAxis(rot.y, glm::vec3(0, 1, 0));//up());
////    glm::quat qRoll = glm::angleAxis(z, glm::vec3(0, 0, 1));
////    glm::vec3 eul = glm::eulerAngles(rotation);
//    rotation *= qPitch * qYaw; // * qRoll;
//    return *this;
//}

//Transform &Transform::translate(const glm::vec3 &position)
//{
//    return setPosition(this->position + position);
//}

//Transform &Transform::setPosition(const glm::vec3 &position)
//{
//    this->position = position;
//    return *this;
//}

//Transform &Transform::setRotation(const glm::vec3 &axis, float angle)
//{
//    rotation = glm::angleAxis(angle, axis);
//    return *this;
//}

//Transform &Transform::setRotation(const glm::quat &rotation)
//{
//    this->rotation = rotation;
//    return *this;
//}

//glm::vec3 Transform::getPosition(void) const
//{
//    return position;
//}

//glm::quat Transform::getRotation(void) const
//{
//    return rotation;
//}
////TODO: optimise
//glm::mat4 Transform::getTransformMatrix(void) const
//{
//    return glm::translate(position) * glm::toMat4(rotation);//* glm::scale(scale);
//}

//glm::mat4 Transform::getCameraTransformMatrix(void) const
//{
//    return glm::toMat4(rotation) * glm::translate(position);
//}

//glm::vec4 Transform::getDirection(void) const
//{
//    return rotation * glm::vec4(0, 0, 1, 0);
//}

//glm::vec3 Transform::forward() const
//{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 0.0, -1.0));
//    return dir;
//}

//glm::vec3 Transform::right() const
//{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(1.0, 0.0, 0.0));
//    return dir;
//}

//glm::vec3 Transform::up() const
//{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 1.0, 0.0));
//    return dir;
//}

//Transform &Transform::moveForward(const float f)
//{
//    return translate(forward() * f );
//}

//Transform &Transform::moveRight(const float f)
//{
//    return translate(right() * f );
//}

//Transform &Transform::moveUp(const float f)
//{
//    return translate(up() * f );
//}

////glm::vec3 Transform::getScale(void) const
////{
////    return scale;
////}
////Transform &Transform::setScale(const glm::vec3 &scale)
////{
////    this->scale = scale;
////    return *this;
////}
////Transform &Transform::mulScale(const float scale)
////{
////    return setScale(getScale() * scale);
////}

////Transform &Transform::mulScale(const glm::vec3 &scale)
////{
////    return setScale(getScale() * scale);
////}
