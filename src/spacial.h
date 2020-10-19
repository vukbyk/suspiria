//
//  Author: Shervin Aflatooni
//

#pragma once

#include <vector>
#include "transform.h"

#include <QOpenGLExtraFunctions>

//#include <glm/glm.hpp>
//#include <glm/gtc/quaternion.hpp>

class Spacial: protected QOpenGLExtraFunctions
{
protected:
    Transform transform;
//    glm::mat4 worldMatrix;

    Spacial *parentSpacial;

    std::vector<Spacial *> children;

//    Engine *sceene;

public:
//    Spacial(const std::string &tag);
    Spacial();
    virtual ~Spacial();

    void addChild(Spacial *child);

//    virtual void updateAll(Input *input);
    virtual void renderAll() const;

    std::vector<Spacial *> getChildren(void);
    glm::vec3 getPosition(void);

    glm::vec4 getDirection(void);

    Transform &getTransform(void);
//    glm::mat4 &getWorldMatrix(void);

//    const GLfloat *getValuePtr();
};
