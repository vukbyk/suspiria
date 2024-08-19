#pragma once

class Texture;

class Material
{
private:
    Texture *albedo = nullptr;
    Texture *normal = nullptr;
//    Texture *metal;

public:
    Material(const char *aAlbedoPath = "defaultrgba.png", const char *aNormalPath = "normal1x1.png");

    Material(Texture *aAlbedo, Texture *aNormal = nullptr);

    ~Material();

    void bind();

    Texture *getAlbedo() const;
    Texture *getNormal() const;

};
