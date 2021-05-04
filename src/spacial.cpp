#include "spacial.h"

#include <algorithm>
#include <model.h>

#include <scene.h>
#include <components.h>

Spacial::Spacial():
    transform()//, bTransform(btQuaternion( btScalar(0),btScalar(0),btScalar(0)))
{
//    transMat = new btScalar[16];
    parentSpacial = nullptr;
//    BtTransf.getOpenGLMatrix(transMat);
}

Spacial::~Spacial()
{
//    delete[] transMat;
}


glm::vec3 Spacial::getPosition(void) const
{
    return transform.getGLMPosition();
}
Transform &Spacial::getTransform(void)
{
    return transform;
}

//btVector3 Spacial::getBPosition(void)
//{
//    return bTransform.getOrigin();
//}
//btTransform &Spacial::getBTransform(void)
//{
//    return bTransform;
//}

//btScalar* Spacial::getTransformMatrix() const
//{
////    btScalar *btmat= new btScalar[16];

//    glm::mat4 m=gTransform.getTransformMatrix();
//    glm::mat3 m3(m);
//    btTransform BtTransf(glmToBullet(m3), glmToBullet(glm::vec3(m[3][0], m[3][1], m[3][2])));
//    BtTransf.getOpenGLMatrix(transMat);
////    transform.getOpenGLMatrix(btmat);
//    return transMat;
//}

//glm::vec4 Spacial::getDirection(void)
//{
//    return transform.getDirection();
//}

Spacial *Spacial::getParentSpacial() const
{
    return parentSpacial;
}



void Spacial::setParentSpacial(Spacial *value)
{
    parentSpacial = value;
}

void Spacial::createEntity(Scene &scene)
{
    entity = scene.world.create();
    scene.world.emplace<transformComponent>(entity, getTransform());
    //ruzno za ent
    Scene *s = &scene;
    parentSpacial = s;
}

//const GLfloat *Spacial::getValuePtr()
//{
//    return glm::value_ptr(getTransform().getTransformMatrix());
//}

//glm::mat4 &Spacial::getWorldMatrix(void)
//{
//    return worldMatrix;
//}

//void Spacial::updateAll(Input *input)
//{
////    if (parentSpacial == nullptr)
////    {
////        worldMatrix = transform.getTransformMatrix();
////    }
////    else
////    {
////        worldMatrix = parentSpacial->worldMatrix * transform.getTransformMatrix();
////    }
//    for (auto child : children)
//    {
//        child->updateAll(input);
//    }
//}
