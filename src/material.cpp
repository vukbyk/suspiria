#include "material.h"
#include "texture.h"

Material::Material(Texture *aAlbedo, Texture *aNormal)
{
  albedo = aAlbedo;
  normal = aNormal;
//  metal = aMetal;
}

Material::Material(const char *aAlbedoPath, const char *aNormalPath)
{
    albedo = new Texture(aAlbedoPath, true);//, GL_TEXTURE_2D, GL_LINEAR);
    normal = new Texture(aNormalPath);//, GL_TEXTURE_2D, GL_LINEAR);
    Material(albedo, normal);
//  metal = aMetal;
}

Material::~Material()
{
    delete albedo;
    delete normal;
}

void Material::bind()
{
    albedo->bind(0);
    normal->bind(1);
    //metal->bind();
}

