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
//    glUniformMatrix4fv(model, 1, GL_FALSE, &(getChildren()[0]->getGlmTransform().getTransformMatrix())[0][0]);
//    getChildren()[0]->renderAll();
    for (auto child : children)
    {
//        child->getPosition();
//        glm::mat4 modelTempMat = child->getTransform().getTransformMatrix();

//        glUniformMatrix4fv(model, 1, GL_FALSE, &(child->getTransform().getTransformMatrix())[0][0]);//&modelTempMat[0][0]);//&mtm[0][0]);
//        glUniformMatrix4fv(model, 1, GL_FALSE, child->getTransform().getOpenGLMatrix());

        child->getTransform().getOpenGLMatrix(tm);
        glUniformMatrix4fv(model, 1, GL_FALSE, tm);
        child->renderAll();
    }
}
