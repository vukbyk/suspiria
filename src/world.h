#ifndef WORLD_H
#define WORLD_H

#include <entt/entity/registry.hpp>
#include "components.h"
#include "meshmanager.h"
#include "texturemanager.h"

class World
{   
//    template<typename T>
//    void OnComponentAdded(Entity entity, T& component);

public:
    World();
    ~World();
    entt::registry registry;

    MeshManager *meshManager;
    TextureManager *textureManager;

    class Entity CreateEntity();

//    void DestroyEntity(Entity entity);

    operator entt::registry&() { return registry;};
    operator const entt::registry&() const {return registry;}

    friend class Entity;

    MeshManager *getMeshManager() const;
    TextureManager *getTextureManager() const;
};

#endif // WORLD_H

