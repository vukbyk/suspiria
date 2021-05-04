#ifndef ECSCOMPONENTS_H
#define ECSCOMPONENTS_H

#include "transform.h"
#include <QOpenGLExtraFunctions>
#include <string>

struct SimpleRenderComponent
{

//    Transform transform;
//    btScalar transform[16];

    GLuint albedoId;
    GLuint normalId;

    GLuint VAO;
//    GLuint VBO;
//    GLuint IBO;
    GLuint indicesSize;


};

struct meshComponent
{
    GLuint VAO;
    GLuint indicesSize;
//    GLuint verticesSize;

    meshComponent(const GLuint aVAO,// const GLuint aVBO, const GLuint aIBO,
          const GLuint aIndicesSize)://, const GLuint aVerticesSize):
            VAO(aVAO),// VBO(aVBO), IBO(aIBO),
            indicesSize(aIndicesSize)//, verticesSize(aVerticesSize)
    {};

    operator GLuint&() {return VAO;};
    operator const GLuint&() const {return VAO;}
};

struct transformComponent
{
    Transform transform;
    transformComponent(const Transform val):transform(val){};

    operator Transform&() {return transform;};
    operator const Transform&() const {return transform;}
};


struct texturesComponent
{
    GLuint albedoId;
    GLuint normalId;
    //GLuint metalId;
    texturesComponent(const GLuint aAlbedoId, const GLuint aNormalID)
        :albedoId(aAlbedoId), normalId(aNormalID){};
};

struct albedoTextureComponent
{
    GLuint textureId;
    albedoTextureComponent(const GLuint val):textureId(val){};

    operator GLuint&() {return textureId;};
    operator const GLuint&() const {return textureId;}
};

struct normalTextureComponent
{
    GLuint textureId;
    normalTextureComponent(const GLuint val):textureId(val){};

    operator GLuint&() {return textureId;};
    operator const GLuint&() const {return textureId;}
};

#endif // ECSCOMPONENTS_H


