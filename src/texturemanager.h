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

    void load(const std::string fileName, bool gammaCorrection=false, bool flip=false);
    void loadBoxTexture(const std::string boxTextureName,
                        std::vector<std::string> face,
                        bool flip=true,
                        bool gammaCorrection=false);
    void loadBoxTexture(const std::string boxTextureName,
                        std::string *face,
                        bool flip=true,
                        bool gammaCorrection=false);
    Texture* get(const std::string fileName);
    GLuint getId(const std::string fileName);


};
