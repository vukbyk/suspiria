#include "entity.h"
#include "world.h"
#include "components.h"

Entity::Entity(class World *aWorld, entt::entity aHandle)
    : world(aWorld), handle(aHandle){}

Entity *Entity::addSimpleRenderComponent(const char *mesh, const char *albedo, const char *normal)
{
    addComponent(SimpleRenderComponent(world->getMeshManager()->get(mesh)->getVAO(),
                                         world->getMeshManager()->get(mesh)->getIndicesSize(),
                                         world->getTextureManager()->getId(albedo),
                                         world->getTextureManager()->getId(normal)));
    return this;
}

Entity *Entity::addTransformComponent(const GLfloat x, const GLfloat y, const GLfloat z)
{
    addComponent(TransformComponent(btVector3(x , y, z)));
    return this;
}


//bool Entity::operator==(const Entity& other) const
//{
//    return handle == other.handle && world == other.world;
//}

//bool Entity::operator!=(const Entity &other) const
//{
//    return !(*this == other);
//}
