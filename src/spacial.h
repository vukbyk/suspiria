//
//  Author: Shervin Aflatooni
//

#pragma once

#include <vector>
#include <QOpenGLExtraFunctions>

//#include <glm/glm.hpp>
//#include <glm/gtc/quaternion.hpp>

#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include "transform.h"
//#include "glmtransform.h"
//for ECS
//#include "scene.h"
#include <entt/entity/fwd.hpp>

class Spacial: protected QOpenGLExtraFunctions
{
protected:
    Transform transform;
//    btTransform bTransform;
//    btScalar *transMat;
//    std::vector<Spacial *> children;

//    glm::mat4 worldMatrix;
    Spacial *parentSpacial;
//    Engine *sceene;

public:
    entt::entity entity;
    Spacial();
    virtual ~Spacial();

    void addChild(Spacial *child);

    virtual void renderAll() {};

    std::vector<Spacial *> getChildren(void);
    btVector3 getPosition(void) const;
    glm::vec3 getGLMPosition(void) const;
    Transform &getTransform(void);

    btVector3 getBPosition(void);
    btTransform &getBTransform(void);
////zamena za glm poziv sa lm pozivom
    btScalar* getTransformMatrix(void) const;
    btScalar* getTransformMatrixTemp(void) const;

//    glm::vec4 getDirection(void);

    Spacial *getParentSpacial() const;
    void setParentSpacial(Spacial *value);

    virtual void createEntity(class Scene &scene);

//    glm::mat4 &getWorldMatrix(void);
//    const GLfloat *getValuePtr();
//    virtual void updateAll(Input *input);

};
