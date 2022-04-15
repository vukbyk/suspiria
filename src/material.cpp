#include "material.h"
#include "texturemanager.h"


Material::Material(const char *aAlbedoPath, const char *aNormalPath)
{
    albedo = new Texture(aAlbedoPath, true);//, GL_TEXTURE_2D, GL_LINEAR);
    normal = new Texture(aNormalPath);//, GL_TEXTURE_2D, GL_LINEAR);
//  metal = aMetal;
}

Material::Material(Texture *aAlbedo, Texture *aNormal)
{
  albedo = aAlbedo;
  normal = aNormal;
//  metal = aMetal;
}

Material::~Material()
{
    delete albedo;
    delete normal;
}

void Material::bind()
{
//    albedo->bind(0);
//    normal->bind(1);
    //metal->bind();
}

Texture *Material::getAlbedo() const
{
    return albedo;
}

Texture *Material::getNormal() const
{
    return normal;
}
