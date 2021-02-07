#pragma once

#include <QOpenGLExtraFunctions>
#include "spacial.h"

class Scene: protected Spacial
{
protected:
    btScalar tm[16];
    Spacial *parentSpacial;
    std::vector<Spacial *> children;
    class ShaderProgram *shaderProgram;
    GLint model;
public:
//    Scene(const std::string &tag);
    Scene(class ShaderProgram *sp);
    virtual ~Scene();

    void addChild(Spacial *child);

    std::vector<Spacial *> getChildren(void);

//    virtual void updateAll(Input *input);
    virtual void renderAll();

    GLint getModel() const;
    void setModel(const GLint &value);
};
