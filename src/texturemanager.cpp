#include "texturemanager.h"
#include "texture.h"


TextureManager::~TextureManager()
{
    for ( const auto &t : texture )
    {
        delete t.second;
    }
}

void TextureManager::load(const std::string fileName, bool gammaCorrection)
{
    if(texture.count(fileName))
    {
            qDebug("Texture \"%s\" already loaded", fileName.c_str());
            return;
    }
    texture[fileName] = new Texture(fileName, gammaCorrection);
}

Texture *TextureManager::get(const std::string fileName)
{

    if(texture.count(fileName))
    {
        return texture[fileName];
    }
    else
    {
        qDebug("Texture \"%s\" not loaded...", fileName.c_str());
        return nullptr;
    }
}

GLuint TextureManager::getId(const std::string fileName)
{

    if(texture.count(fileName))
    {
        return texture[fileName]->getTextureId();
    }
    else
    {
        qDebug("Texture \"%s\" not loaded...", fileName.c_str());
        return -1;
    }
}
