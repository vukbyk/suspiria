#pragma once

#include "mesh.h"

#include <string>
#include <map>
#include <vector>

#include <assimp/scene.h>

class MeshManager
{
    void loadScene(const aiScene* scene, const std::string fileName);
//    If we import multiple mesh with assimp
//    std::map<std::string, std::vector<Mesh>> mesh;
    std::map<std::string, class Mesh *> mesh;
public:

    MeshManager() = default;
    ~MeshManager();

    void loadAssimp(const std::string file);
    class Mesh* get(const std::string file);

};
