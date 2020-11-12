#include "model.h"
#include "texture.h"
#include "mesh.h"
#include "assimpload.h"
#include "material.h"


Model::Model(): Spacial()
{
    mesh = nullptr;
}


//Model::Model(const char *aMeshPath)
//{


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
    setMesh(mp.mesh);

//    getTransform().setPosition(glm::vec3(-2.0, 0.0, -3.0));
//    addChild(model);
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
