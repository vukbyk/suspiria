#pragma once

#include "mesh.h"

#include <string>
#include <map>
#include <vector>

#include <assimp/scene.h>

class AssimpLoad
{
public:
    AssimpLoad(const std::string file);

private:
    void loadScene(const aiScene* scene, const std::string fileName);
//    const std::string fileName;

public:
    static std::map<std::string, std::vector<class Mesh*>> sceneMeshRendererDataCache;
};
