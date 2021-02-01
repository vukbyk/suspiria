#pragma once

#include "spacial.h"

class Model: public Spacial
{
public:
    class Material *material;
    class Mesh *mesh;
//    class Texture *material;
public:
    Model();
//TODO: change texture to material
//    Model(const char *aMeshPath);
    Model(const char *aMeshPath, const char *aAlbedoPath = "defaultComplex.png", const char *aNormalPath = "defaultNormal.png");
    Model(class Material *material, class Mesh *mesh);

    ~Model();
//    virtual void updateAll(Input *input);
    virtual void renderAll();
    Mesh *getMesh() const;
    void setMesh(class Mesh *value);
    void setMaterial(class Material *value);
};
