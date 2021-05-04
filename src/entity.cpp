#include "entity.h"
#include "world.h"

Entity::Entity(class World *aWorld, entt::entity aHandle)
        : world(aWorld), handle(aHandle){}


//bool Entity::operator==(const Entity& other) const
//{
//    return handle == other.handle && world == other.world;
//}

//bool Entity::operator!=(const Entity &other) const
//{
//    return !(*this == other);
//}
