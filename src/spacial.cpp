#include "spacial.h"

//#include "Component.h"

#include <algorithm>
#include <model.h>

//Spacial::Spacial(const std::string &aTag)
//{
//    this->tag = aTag;
//    parentSpacial = nullptr;
//}

Spacial::Spacial()
{
    parentSpacial = nullptr;
}

Spacial::~Spacial()
{

}

void Spacial::addChild( Spacial *child)
{
    child->parentSpacial = this;
    children.push_back(child);
}

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

void Spacial::renderAll() const
{
    for (auto child : children)
    {
        child->renderAll();
    }
}


Transform &Spacial::getTransform(void)
{
    return transform;
}

std::vector<Spacial *> Spacial::getChildren(void)
{
    return children;
}

//glm::mat4 &Spacial::getWorldMatrix(void)
//{
//    return worldMatrix;
//}

glm::vec3 Spacial::getPosition(void)
{
    if (parentSpacial == nullptr)
    {
        return transform.getPosition();
    }
    else
    {
        return (/*parentSpacial->worldMatrix*/transform.getTransformMatrix() * glm::vec4(transform.getPosition(), 1)).xyz();
    }
}

glm::vec4 Spacial::getDirection(void)
{
    if (parentSpacial == nullptr)
    {
        return transform.getDirection();
    }
    else
    {
        return glm::normalize(/*parentSpacial->worldMatrix*/transform.getTransformMatrix() * transform.getDirection());
    }
}

//const GLfloat *Spacial::getValuePtr()
//{
//    return glm::value_ptr(getTransform().getTransformMatrix());
//}
