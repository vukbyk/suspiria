#ifndef ECSCOMPONENTS_H
#define ECSCOMPONENTS_H

#include "transform.h"
#include "glmtransform.h"
#include <QOpenGLExtraFunctions>
#include <string>

struct SimpleRenderComponent
{

//    Transform transform;
//    btScalar transform[16];


    GLuint VAO;
//    GLuint VBO;
//    GLuint IBO;
    GLuint indicesSize;

    GLuint albedoId;
    GLuint normalId;

    SimpleRenderComponent()=default;
    SimpleRenderComponent(const GLuint aVAO, const GLuint aIndicesSize, const GLuint albedo, const GLuint normal)
    :VAO(aVAO), indicesSize(aIndicesSize), albedoId(albedo), normalId(normal) {};
};

//struct CameraComponent
//{
//    GLfloat zoom=1.0f;
//    CameraComponent(const GLfloat val):zoom(val){};
//    operator GLfloat&() {return zoom;};
//    operator const GLfloat&() const {return zoom;}
//};

struct LightComponent
{
    GLint lightID;
    LightComponent(const GLint aLightID):lightID(aLightID){};

    operator GLint&() {return lightID;};
    operator const GLint&() const {return lightID;}
};

struct MeshComponent
{
    GLuint VAO;
    GLuint indicesSize;
//    GLuint verticesSize;

    MeshComponent(const GLuint aVAO,// const GLuint aVBO, const GLuint aIBO,
          const GLuint aIndicesSize)://, const GLuint aVerticesSize):
            VAO(aVAO),// VBO(aVBO), IBO(aIBO),
            indicesSize(aIndicesSize)//, verticesSize(aVerticesSize)
    {};

    operator GLuint&() {return VAO;};
    operator const GLuint&() const {return VAO;}
};

struct TransformComponent
{
    Transform transform;
    TransformComponent(const Transform val):transform(val){};

    operator Transform&() {return transform;};
    operator const Transform&() const {return transform;}
};

//struct TransformComponent
//{
//    GLMTransform transform;
//    TransformComponent(const GLMTransform val):transform(val){};
//    TransformComponent(const Transform val)
//    {
//        transform.setTransform( glm::mat4(1));
//        transform.setTransform( val.getTransformMatrix());
//    };

//    operator GLMTransform&() {return transform;};
//    operator const GLMTransform&() const {return transform;}
//};


struct TexturesComponent
{
    GLuint albedoId;
    GLuint normalId;
    //GLuint metalId;
    TexturesComponent(const GLuint aAlbedoId, const GLuint aNormalID)
        :albedoId(aAlbedoId), normalId(aNormalID){};
};

struct AlbedoTextureComponent
{
    GLuint textureId;
    AlbedoTextureComponent(const GLuint val):textureId(val){};

    operator GLuint&() {return textureId;};
    operator const GLuint&() const {return textureId;}
};

struct NormalTextureComponent
{
    GLuint textureId;
    NormalTextureComponent(const GLuint val):textureId(val){};

    operator GLuint&() {return textureId;};
    operator const GLuint&() const {return textureId;}
};

#endif // ECSCOMPONENTS_H


