#ifndef MODEL_H
#define MODEL_H

#include "spacial.h"
//#include "Mesh.h"

class Model: public Spacial
{
public:
    class Mesh *mesh;
    class Material *material;
//    class Texture *material;
public:
    Model();
    //TODO: change texture to material
//    Model(const char *aMeshPath);
    Model(const char *aMeshPath, const char *aAlbedoPath = "defaultComplex.png", const char *aNormalPath = "defaultNormal.png");
    ~Model();
//    virtual void updateAll(Input *input);
    virtual void renderAll() const;
    Mesh *getMesh() const;
    void setMesh(class Mesh *value);
    void setMaterial(class Material *value);
};

#endif // MODEL_H
