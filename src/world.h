#ifndef WORLD_H
#define WORLD_H

#include <entt/entity/registry.hpp>
#include "components.h"

class World
{   
//    template<typename T>
//    void OnComponentAdded(Entity entity, T& component);

public:
    World();
    ~World();
    entt::registry registry;

    class Entity CreateEntity();

//    void DestroyEntity(Entity entity);

    //    operator entt::registry&() { return world;};
    //    operator const entt::registry&() const {return world;}

    friend class Entity;

};

#endif // WORLD_H
