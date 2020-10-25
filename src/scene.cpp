#include "scene.h"
#include "shaderprogram.h"

GLint Scene::getModel() const
{
    return model;
}

void Scene::setModel(const GLint &value)
{
    model = value;
}

Scene::Scene(ShaderProgram *sp)
{
    shaderProgram = sp;
    parentSpacial = nullptr;
    //    init();
}

Scene::~Scene()
{

}

void Scene::addChild( Spacial *child)
{
    child->setParentSpacial(this);
    children.push_back(child);
}

std::vector<Spacial *> Scene::getChildren(void)
{
    return children;
}

void Scene::renderAll()
{
    initializeOpenGLFunctions();
    for (auto child : children)
    {
        child->getPosition();

        glm::mat4 modelTempMat = child->getTransform().getTransformMatrix();
        glUniformMatrix4fv(model, 1, GL_FALSE, &modelTempMat[0][0]);//&mtm[0][0]);
        child->renderAll();
    }
}
