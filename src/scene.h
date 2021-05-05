#pragma once

#include <QOpenGLExtraFunctions>
#include "spacial.h"
#include <entt/entity/registry.hpp>

class Scene: /*protected*/public Spacial
{
protected:
    btScalar tm[16];
    Spacial *parentSpacial;
//    std::vector<Spacial *> children;
    std::vector<class Spacial *> children;
//    class ShaderProgram *shaderProgram;
public:
    GLint model;

public:
    entt::registry world;
//    Scene(const std::string &tag);
    Scene();
    virtual ~Scene();

    void addChild(class Spacial *child);

    std::vector<class Spacial *> getChildren(void);

//    virtual void updateAll(Input *input);
    virtual void renderAll();

    GLint getModel() const;
    void setModel(const GLint &value);
    virtual void createEntity(Scene &scene)override{};
};
