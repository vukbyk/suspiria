#pragma once

#include <QOpenGLExtraFunctions>

//#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "entity.h"



class Scene: protected QOpenGLExtraFunctions, public World
{
protected:
//    class World *world;//for multy scene
    Entity camera;
    Entity skyCube;



public:

    class Mesh *renderQuad;

    class ShaderPBR *shaderMain;
    class ShaderVP *shaderIrradiance;
    class ShaderMVP *shaderShadow;
    class ShaderVP *shaderSky;
    class ShaderProgram *shaderProgramFBScr;
    class ShaderProgram *shaderDebugQuad;
    class ShaderProgram *shaderBrdf;

    Scene();
//    Scene(class World *val);
    virtual ~Scene();

    void initEntities();
    void initShaders();

    const Entity getCamera();
    const Entity getSkyCube();

    void importTextures();
    void importMeshes();

    void prepareAssetsManyCubes();
    void prepareAssetsPlane();

////// For tuples use world->reg()->get<T>
////// So maybe better to use regular
//    template<typename T>//,  typename... Args>
//    T& get(Entity component)
//    {
//        return reg()->get<T>(component);
//    }

};
