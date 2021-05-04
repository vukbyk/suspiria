#ifndef ENTITY_H
#define ENTITY_H

#include "qlogging.h"
//#include <entt/entity/registry.hpp>
#include <entt/entt.hpp>
#include "world.h"
//#include "components.h"

class Entity
{
    class World *world =nullptr;
    entt::entity handle{ entt::null };

public:
    Entity() = default;
    Entity(class World *aWorld, entt::entity aHandle);

    operator entt::entity() const { return handle; }
    Entity(const Entity& other) = default;
//    operator bool() const { return handle != entt::null; }
//    operator uint32_t() const { return (uint32_t)handle; }

//    bool operator==(const Entity& other) const;
//    bool operator!=(const Entity& other) const;

//    template<typename T>
//    bool HasComponent()
//    {
//        return world->registry.all_of<T>(handle);
//    }


//    template<typename T, typename... Args>
//    T &AddComponent(Args&&... args)
//    {
////        if(!HasComponent<T>())
////            qDebug("Entity already has component!");
//        T &component = world->registry.emplace<T>(handle, std::forward<Args>(args)...);

////        world->OnComponentAdded<T>(*this, component);
//        return component;
//    }

    template<typename T1,  typename... Args>
    Entity *addComponent(T1 component)
    {
        world->registry.emplace<T1>(handle, component);
        return this;
    }

//    template<typename T>
//    T &GetComponent()
//    {
//        if(HasComponent<T>())
//            qDebug("Entity does not have component!");
//        return world->registry.get<T>(handle);
//    }


//    template<typename T>
//    void RemoveComponent()
//    {
//        if(HasComponent<T>())
//            qDebug("Entity does not have component!");
//        world->registry.remove<T>(handle);
//    }
};

#endif // ENTITY_H
