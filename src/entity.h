#ifndef ENTITY_H
#define ENTITY_H

#include "qlogging.h"
//#include <entt/entity/registry.hpp>
#include <entt/entt.hpp>
#include "world.h"
//#include "camera.h"
//#include "components.h"

class Entity
{
    class World *world =nullptr;
public:
    entt::entity handle{ entt::null };

public:
    Entity() = default;
    Entity(class World *aWorld, entt::entity aHandle);

    operator entt::entity() const { return handle; }
    Entity(const Entity& other) = default;

    template<typename T>//,  typename... Args>
    Entity *addComponent(T component)
    {
        world->registry.emplace<T>(handle, component);
        return this;
    }

    Entity *addMeshComponent(const char *mesh);

    Entity *addTexturePBRComp(const char *albedo,
                              const char *normal,
                              const char *metallic,
                              const char *roughness,
                              const char *ao);

    Entity *addTextureAlbedoNormalComp(const char *albedo, const char *normal);

    Entity *addCubeMapComp(const char *albedo);

    Entity *addTransformComponent(const GLfloat x, const GLfloat y, const GLfloat z);
    Entity *addTransformComponent(TransformComp &transComp);

    Entity *addFixSphereBVComp(const float inRadius=2.5f);

    Entity *addCameraComp(CameraComp &camera);
    Entity *addCameraComp(const GLfloat aFar=0.3f, const GLfloat aNear=1000.0f,
                          const GLfloat aFov=45.0f, const GLfloat aAspect=0.66f);

//    Entity *addShaderComp(const std::string &val);

//    Entity *addPerspectiveComp(PerspectiveComp &perspective);
//    Entity *addPerspectiveComp( const GLfloat aFar=0.3f, const GLfloat aNear=1000.0f,
//                                const GLfloat aFov=45.0f, const GLfloat aAspect=0.66f);

//    Entity *addSimpleRenderComp(const char* mesh, const char* albedo, const char* normal);
//    Entity *addTransformComponent();
//    Entity *addTransformComponent(const glm::vec3 v);
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
