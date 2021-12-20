#pragma once

#include <string>

class Texture;

class Material
{
private:
    Texture *albedo = nullptr;
    Texture *normal = nullptr;
//    Texture *metal;

public:
    Material(const char *aAlbedoPath = "defaultComplex.png", const char *aNormalPath = "defaultNormal.png");

    Material(Texture *aAlbedo, Texture *aNormal = nullptr);

    ~Material();

    void bind();

    Texture *getAlbedo() const;
    Texture *getNormal() const;

};
