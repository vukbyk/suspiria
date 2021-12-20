#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

//btScalar Transform::tm[16];

//Transform::Transform(const Transform &val)
//{
//    transform = btTransform(val.transform);
//}


//Transform::Transform(const btTransform &val)
//{
//    transform = btTransform(val);
//}

Transform::Transform(const btVector3 &aPosition,
                     const btQuaternion &aRotation,
                     const btVector3 &aScale)
    :position(aPosition),rotation(aRotation),transform(aRotation, aPosition)
{
    //transform.setIdentity();
}

Transform::Transform(const glm::vec3 &aPosition
                    ,const glm::quat &aRotation
                    ,const glm::vec3 &scale)
{
    transform.setOrigin( glmToBullet(aPosition));
    transform.setRotation( glmToBullet( aRotation));
    rotation=glmToBullet(aRotation);
//    this->scale = scale;
}

Transform::~Transform(void)
{
}


void Transform::getOpenGLMatrix(btScalar *tm)
{
    transform.getOpenGLMatrix(tm);
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
//    glm::mat4 rot = glm::toMat4( bulletToGlm( btQuaternion(transform.getRotation()) ));
//    return rot * glm::translate(bulletToGlm(transform.getOrigin() * -1.0f) );
    glm::mat4 rot = glm::toMat4( bulletToGlm( btQuaternion(transform.getRotation()) ));
    rot = glm::inverse(rot);
    glm::mat4 pos = glm::translate(bulletToGlm(transform.getOrigin() * -1.0f) );
    return rot * pos;
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

void Transform::setOrigin(const btVector3 &position)
{
    transform.setOrigin(position);
}

void Transform::setPosition(const btVector3 &position)
{
    transform.setOrigin(position);
}

void Transform::translate(const btVector3 &position)
{
    setPosition(transform.getOrigin() + position);
}

void Transform::setPosition(const glm::vec3 &position)
{
//    this->position = position;
    transform.setOrigin(glmToBullet(position));
}

void Transform::translate(const glm::vec3 &position)
{
//    return setPosition(this->position + position);
//    btVector3 v = glmToBullet(position) + transform.getOrigin();
//    transform.setOrigin(v);
    setPosition(bulletToGlm(transform.getOrigin()) + position);
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

void Transform::setRotation(const btQuaternion &rotation)
{
    transform.setRotation(transform.getRotation() * rotation);
}

void Transform::setRotation(const btVector3 &aAxis, float aAngle)
{
    transform.setRotation(btQuaternion(aAxis, aAngle));
}



void Transform::setRotation(const glm::quat &rotation)
{
    transform.setRotation( glmToBullet(rotation) );
}

void Transform::setRotation(const glm::vec3 &aAxis, float aAngle)
{
    transform.setRotation(glmToBullet(glm::quat(aAngle ,aAxis)) );
}

void Transform::addYawPitch(float rotYaw, float rotPitch)
{
//    btQuaternion orientation(transform.getRotation());
//    orientation =

//    float y=0;
//    float p=0;
//    float r=0;
//    transform.getRotation().getEulerZYX(y, p, r);
////    if(rotYaw);
////    btTransform temp(btQuaternion(),transform.getOrigin());
////    temp.setOrigin(btVector3(temp.getOrigin().x(), temp.getOrigin().y(), temp.getOrigin().z()+1.0));
//    p+=rotPitch;
//    y+=rotYaw;
//    btQuaternion quatYaw  (y,0,0);
//    btQuaternion quatPitch(0,p,0);
//    btQuaternion rot = quatYaw;//*quatPitch;

//    btQuaternion transYaw(rotYaw, btVector3(0,1,0));
//    btQuaternion transPitch(quatPitch, btVector3(1,0,0));



////    transYaw *= transform;
////    transform *= transPitch;
////    temp.setRotation(quatPitch);
//    transform.setRotation(rot);
//    btVector3 v= transform.getBasis()[0];
    btVector3 right = glmToBullet(rightGLM());//btVector3(1.f ,0.f ,0.f));
    btQuaternion qPitch(right , rotPitch);
    transform.setRotation( qPitch * transform.getRotation() );
    btQuaternion qYaw(btVector3(0, 1, 0), rotYaw);
//    transform.setRotation(transform.getRotation() * qYaw);
    btQuaternion btQuat =  qYaw * qPitch ;
    transform.setRotation(transform.getRotation() * qYaw * qPitch );
}

void Transform::rotate(const float aAngle, const btVector3 &aAxis)
{
    transform.setRotation(btQuaternion(aAxis, aAngle));
}

void Transform::rotate(const btQuaternion aRot)
{
    transform.setRotation(transform.getRotation() * aRot);
}

void Transform::rotate(const btVector3 euler)
{
    btQuaternion quat(euler.x(), euler.y(), euler.z());
    transform.setRotation(transform.getRotation()*quat);
//    rotation *= glm::quat(rot);
}

void Transform::addYawPitch(const btVector3 aRot)
{
    addYawPitch(aRot.y(), aRot.x());
}

void Transform::rotate(const float aAngle, const glm::vec3 &aAxis)
{
    btVector3 axis = glmToBullet(aAxis);
    transform.setRotation(btQuaternion(axis, aAngle));
//    rotation = glm::rotate(rotation, angle, axis);
}

void Transform::rotate(const glm::quat aRot)
{
//    rotation *= rot;
    transform.setRotation(transform.getRotation() * glmToBullet(aRot));
}

void Transform::rotate(const glm::vec3 aRot)
{
    btQuaternion quat = glmToBullet(glm::quat(aRot));
    transform.setRotation(transform.getRotation()*quat);
//    rotation *= glm::quat(rot);
}

void Transform::addYawPitch(const glm::vec3 aRot)
{
    addYawPitch(aRot.y, aRot.x);
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
    btVector3 dir = btVector3(0.0f, 1.0, 0.0);
    dir.rotate(rot.getAxis(), rot.getAngle() );
    return dir;
}

glm::vec3 Transform::forwardGLM() const
{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 0.0, -1.0));
//    glm::quat rot = bulletToGlm(transform.getRotation());
//    glm::vec3 dir = glm::rotate(glm::inverse(rot), glm::vec3(0.0, 0.0, -1.0));

    glm::mat4 rot = bulletToGlm(transform);
    glm::vec3 dir = rot[2];
    return dir;
}

glm::vec3 Transform::rightGLM() const
{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(1.0, 0.0, 0.0));
//    glm::quat rot = bulletToGlm(transform.getRotation());
//    glm::vec3 dir = glm::rotate(glm::inverse(rot), glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 rot = bulletToGlm(transform);
    glm::vec3 dir = rot[0];
    return dir;

}

glm::vec3 Transform::upGLM() const
{
//    glm::vec3 dir = glm::rotate(glm::inverse(rotation), glm::vec3(0.0, 1.0, 0.0));
//    glm::quat rot = bulletToGlm(transform.getRotation());
//    glm::vec3 dir = glm::rotate(glm::inverse(rot), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 rot = bulletToGlm(transform);
    glm::vec3 dir = rot[1];
    return dir;
}

void Transform::moveForward(const float f)
{
    translate(forward() * f);
}

void Transform::moveRight(const float f)
{
    translate(right() * f );
}

void Transform::moveUp(const float f)
{
    return translate(up() * f );
}

void Transform::moveForwardGLM(const float f)
{
    translate(forwardGLM() * f );
}

void Transform::moveRightGLM(const float f)
{
    translate(rightGLM() * f );
}

void Transform::moveUpGLM(const float f)
{
    translate(upGLM() * f );
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
