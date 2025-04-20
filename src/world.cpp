#include "world.h"
#include "entity.h"
#include "components.h"

#include "meshmanager.h"
#include "texturemanager.h"

World::World()
{
    meshManager = new MeshManager();
    textureManager = new TextureManager();
}

World::~World()
{
    for (auto entity : registry.view<entt::entity>()) {
        registry.destroy(entity);
    }

    // registry.clear();
    // delete meshManager;
    // delete textureManager;
}

Entity World::CreateEntity()
{
    Entity e(this, registry.create());
    return e;
}

TextureManager *World::getTextureManager() const
{
    return textureManager;
}

MeshManager *World::getMeshManager() const
{
    return meshManager;
}

entt::registry *World::reg()
{
    return &registry;
}

void World::AttachToParent(entt::entity child, entt::entity parent) {
    if (!registry.valid(child) || !registry.valid(parent)) return;
    if (!registry.all_of<TransformComp>(parent)) return;

    Transform* parentTransform = &registry.get<TransformComp>(parent).transform;
    registry.emplace_or_replace<ParentComponent>(child, ParentComponent{ parent, parentTransform });
}

void World::DetachFromParent(entt::entity child) {
    if (registry.all_of<ParentComponent>(child)) {
        registry.remove<ParentComponent>(child);
    }
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
