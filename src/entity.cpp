#include "entity.h"
#include "world.h"
#include "components.h"


Entity::Entity(class World *aWorld, entt::entity aHandle)
    : world(aWorld), handle(aHandle){}


Entity &Entity::addMeshComponent(const char *mesh)
{
    addComponent(MeshComp(world->getMeshManager()->get(mesh)->getVAO(),
                               world->getMeshManager()->get(mesh)->getIndicesSize()));
    return *this;
}

Entity &Entity::addTexturePBRComp(const char *albedo, const char *normal, const char *metallic, const char *roughness, const char *ao)
{
    addComponent(MaterialPBRComp(world->getTextureManager()->getId(albedo),
                                 world->getTextureManager()->getId(normal),
                                 world->getTextureManager()->getId(metallic),
                                 world->getTextureManager()->getId(roughness),
                                 world->getTextureManager()->getId(ao) ));
    return *this;
}


Entity &Entity::addTextureAlbedoNormalComp(const char *albedo, const char *normal)
{
    addComponent(MaterialAlbedoNormalComp(world->getTextureManager()->getId(albedo),
                                          world->getTextureManager()->getId(normal)));
    return *this;
}


Entity &Entity::addCubeMapComp(const char *albedo)
{
    addComponent(CubeMapComp(world->getTextureManager()->getId(albedo)));
    return *this;
}

Entity &Entity::addTransformComponent(const GLfloat x, const GLfloat y, const GLfloat z)
{
    addComponent(TransformComp(btVector3(x , y, z)));
    return *this;
}

Entity &Entity::addTransformComponent(TransformComp &transComp)
{
    addComponent(TransformComp(transComp));
    return *this;
}

Entity &Entity::addFixSphereBVComp(const float inRadius)
{
    addComponent(FixSphereBVComp());
    return *this;
}

Entity &Entity::addDynamicShadowComp()
{
    addComponent(DynamicShadowComp());
    return *this;
}

// Entity *Entity::addCameraComp(CameraComp &aCamera)
// {
//     addComponent(CameraComp(CameraComp(aCamera)));
//     return this;
// }

// Entity *Entity::addCameraComp(const GLfloat aNear, const GLfloat aFar, const GLfloat aFov, const GLfloat aAspect)
// {
//     addComponent(CameraComp(aNear, aFar, aFov, aAspect));
//     return this;
// }


Entity &Entity::addCameraComp(const GLfloat aNear, const GLfloat aFar, const GLfloat aFov, const GLfloat aAspect)
{
    if (world->registry.any_of<CameraComp>(handle))
        world->registry.remove<CameraComp>(handle);

    addComponent(CameraComp(aNear, aFar, aFov, aAspect));
    return *this;
}
//Entity &Entity::addShaderComp(const std::string &val)
//{
//    addComponent(ShaderComp( val ));
//    return this;
//}

//Entity &Entity::addPerspectiveComp(PerspectiveComp &perspective)
//{
//    addComponent(PerspectiveComp(perspective));
//    return this;
//}
//Entity &Entity::addPerspectiveComp(const GLfloat aNear, const GLfloat aFar, const GLfloat aFov, const GLfloat aAspect)
//{
//    addComponent(PerspectiveComp(aNear, aFar, aFov, aAspect));
//    return this;
//}




//Entity &Entity::addSimpleRenderComp(const char *mesh, const char *albedo, const char *normal)
//{
//    addComponent(SimpleRenderComponent(world->getMeshManager()->get(mesh)->getVAO(),
//                                         world->getMeshManager()->get(mesh)->getIndicesSize(),
//                                         world->getTextureManager()->getId(albedo),
//                                         world->getTextureManager()->getId(normal)));
//    return this;
//}


//Entity *Entity::addTransformComponent(const glm::vec3 v)
//{
//    glm::mat4 mat(1);
//    mat[3][0] = v[0];
//    mat[3][1] = v[1];
//    mat[3][2] = v[2];
//    addComponent(mat);
//    return this;
//}

//bool Entity::operator==(const Entity& other) const
//{
//    return handle == other.handle && world == other.world;
//}

//bool Entity::operator!=(const Entity &other) const
//{
//    return !(*this == other);
//}
