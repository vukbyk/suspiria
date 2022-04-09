#pragma once

#include "texture.h"

#include <string>
#include <map>
#include <vector>

//#include "texture.h"

class TextureManager
{
//    std::map<std::string, std::vector<class Texture*>> texture;
    std::map<std::string, class Texture*> texture;
public:
    TextureManager() = default;
    ~TextureManager();

    void load(const std::string fileName, bool gammaCorrection);
    void load(const std::string boxTextureName, const std::vector<std::string> faces);
    Texture* get(const std::string fileName);
    GLuint getId(const std::string fileName);


};
