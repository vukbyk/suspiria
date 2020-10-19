#pragma once

class Texture;

class Material
{
public:
    Material(Texture *aAlbedo, Texture *aNormal = nullptr);
    Material(const char *aAlbedoPath = "defaultComplex.png", const char *aNormalPath = "defaultNormal.png");
    ~Material();

    void bind();

private:
    Texture *albedo;
    Texture *normal;
//    Texture *metal;
};
