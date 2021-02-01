#pragma once

#include "mesh.h"

#include <string>
#include <map>
#include <vector>

#include <assimp/scene.h>

//struct AssetData
//{
//    class Mesh *mesh;
//};

class AssimpLoad
{
public:
    AssimpLoad(const std::string file);
    ~AssimpLoad(void);

private:
    void loadScene(const aiScene* scene);
    const std::string fileName;

public:
    static std::map<std::string, std::vector<class Mesh*>> sceneMeshRendererDataCache;
};
