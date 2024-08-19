#pragma once

#include "mesh.h"

#include <string>
#include <map>
// #include <vector>

#include <assimp/scene.h>

namespace tinygltf {
class Model;
}

// Forward declare the function
void loadGltfScene(const tinygltf::Model &gltfmodel, const std::string &fileName);

class MeshManager
{
    void loadAssimpScene(const aiScene* scene, const std::string fileName);
    // void loadGltfScene(const tinygltf::Model &gltfmodel, const std::string fileName);

//    If we import multiple mesh with assimp
//    std::map<std::string, std::vector<Mesh>> mesh;
    std::map<std::string, class Mesh *> mesh;
public:

    MeshManager() = default;
    ~MeshManager();

    void loadAssimp(const std::string file);
    // void loadGltf(const std::string file);
    class Mesh* get(const std::string file);

};
