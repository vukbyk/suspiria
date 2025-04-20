#ifndef WORLD_H
#define WORLD_H

#include <entt/entity/registry.hpp>
#include "components.h"
#include "meshmanager.h"
#include "texturemanager.h"

class World
{   
protected:
    entt::registry registry;

    MeshManager *meshManager;
    TextureManager *textureManager;

public:
    World();
    virtual ~World();

    class Entity CreateEntity();
//    void DestroyEntity(Entity entity);

    operator entt::registry&() { return registry;};
    operator const entt::registry&() const {return registry;}

    friend class Entity;

    MeshManager *getMeshManager() const;
    TextureManager *getTextureManager() const;
    entt::registry *reg();

    void AttachToParent(entt::entity child, entt::entity parent);
    void DetachFromParent(entt::entity child);

};

#endif // WORLD_H

