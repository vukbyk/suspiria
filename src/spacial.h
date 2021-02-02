//
//  Author: Shervin Aflatooni
//

#pragma once

#include <vector>
#include "transform.h"

#include <QOpenGLExtraFunctions>

//#include <glm/glm.hpp>
//#include <glm/gtc/quaternion.hpp>

#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

class Spacial: protected QOpenGLExtraFunctions
{
protected:
    Transform transform;
//    std::vector<Spacial *> children;

//    glm::mat4 worldMatrix;
    Spacial *parentSpacial;
//    Engine *sceene;

public:
    Spacial();
    virtual ~Spacial();

    void addChild(Spacial *child);

    virtual void renderAll() {};

    std::vector<Spacial *> getChildren(void);
    glm::vec3 getPosition(void);

//    glm::vec4 getDirection(void);

    Transform &getTransform(void);

    Spacial *getParentSpacial() const;
    void setParentSpacial(Spacial *value);

//    glm::mat4 &getWorldMatrix(void);
//    const GLfloat *getValuePtr();
//    virtual void updateAll(Input *input);

};
