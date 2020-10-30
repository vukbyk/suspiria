#include "spacial.h"

//#include "Component.h"

#include <algorithm>
#include <model.h>


Spacial::Spacial():transform()
{
    parentSpacial = nullptr;
    //    initializeOpenGLFunctions();
}

Spacial::~Spacial()
{

}

Transform &Spacial::getTransform(void)
{
    return transform;
}

glm::vec3 Spacial::getPosition(void)
{     
    return transform.getPosition();
}

glm::vec4 Spacial::getDirection(void)
{
    return transform.getDirection();
}

Spacial *Spacial::getParentSpacial() const
{
    return parentSpacial;
}

void Spacial::setParentSpacial(Spacial *value)
{
    parentSpacial = value;
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
