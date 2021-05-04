#include "world.h"
#include "entity.h"
#include "components.h"

World::World()
{
    ;
}

World::~World()
{
    registry.clear();
}

Entity World::CreateEntity()
{
    Entity e(this, registry.create());
    return e;
}

//void World::DestroyEntity(Entity entity)
//{
//    registry.destroy(entity);
//}

//template<typename T>
//void World::OnComponentAdded(Entity entity, T& component)
//{
//    static_assert(false);
//}

//template<>
//void World::OnComponentAdded<cTransform>(Entity entity, cTransform& component)
//{
//    ;
//}

//template<>
//void World::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
//{
//    component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
//}

//template<>
//void World::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
//{
//}

//template<>
//void World::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
//{
//}

//template<>
//void World::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
//{
//}
