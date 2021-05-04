#include "model.h"
#include "texture.h"
#include "mesh.h"
#include "assimpload.h"
#include "material.h"

//for ETC
#include <entt/entity/fwd.hpp>
#include "scene.h"
#include "components.h"


Model::Model(): Spacial()
{
    mesh = nullptr;
    material = nullptr;
}

Model::Model(const char *aMeshPath, const char *aAlbedoPath, const char *aNormalPath)
{
//    "defaultNormal.png"
//    std::string texturePath = aTexturePath;
    Texture *albedo = new Texture(aAlbedoPath, true);//GL_TEXTURE_2D, GL_LINEAR);
    Texture *normal = new Texture(aNormalPath);//GL_TEXTURE_2D, GL_LINEAR);
    material = new Material(albedo, normal);
//    setMaterial(tex);


//    std::string meshPath = "assets/";
//    meshPath += aMeshPath;
    AssimpLoad a(aMeshPath);
    auto mp = a.sceneMeshRendererDataCache.at(aMeshPath)[0];
    setMesh(mp);

//    getTransform().setPosition(glm::vec3(-2.0, 0.0, -3.0));
    //    addChild(model);
}

Model::Model(Mesh *aMesh, Material *aMaterial)
{
    mesh=aMesh;
    material=aMaterial;
}

Model::Model(Mesh *aMesh, Texture *albedo, Texture *normal)
{
    mesh = aMesh;
    material = new Material(albedo, normal);
}

Model::~Model()
{
    delete material;
    delete mesh;
}

void Model::renderAll()
{
//    if(material)
        material->bind();
//    if(mesh)
        mesh->render();

//    Entity::renderAll();
}

Mesh *Model::getMesh() const
{
    return mesh;
}

void Model::setMesh(Mesh *value)
{
    mesh = value;
}

void Model::setMaterial(Material *value)//Matreial was texture in begining
{
    material = value;
}

void Model::createEntity(Scene &scene)
{    
//    Spacial::createEntity(scene);
//    SimpleRenderComponent cr;
////    cr.transform = getTransform();
////    transform.getOpenGLMatrix(cr.transform);
////    if( material->getAlbedo() )
//        cr.albedoId = material->getAlbedo()->getTextureId();
////    if(material->getNormal() )
//        cr.normalId = material->getNormal()->getTextureId();
//    cr.VAO = mesh->getVAO();
//    cr.indicesSize = mesh->getIndicesSize();

//    scene.world.emplace<SimpleRenderComponent>(entity, cr);


//    cMesh m(mesh->VAO, mesh->indicesSize);
//    scene.world.emplace<cMesh>(entity, m);
//    scene.world.emplace<cTextures>(entity, cTextures(material->getAlbedo()->getTextureId(),
//                                                     material->getNormal()->getTextureId()));
}
