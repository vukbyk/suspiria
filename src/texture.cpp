#define STB_IMAGE_IMPLEMENTATION

#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <QFile>



Texture::Texture(const std::string &fileName)//, GLenum aTextureTarget, GLfloat filter)
{
    initializeOpenGLFunctions();
    int width, height, nChannels;
    qDebug("Loading texture: %s", std::string(":/assets/").append(fileName).c_str());

    QFile file( std::string(":/assets/").append(fileName).c_str() );
    if(!file.open(QIODevice::ReadOnly))
        qDebug("Failed to load file %s", fileName.c_str());
    else
        qDebug("File %s is loaded", fileName.c_str());

    QByteArray DataFile = file.readAll();
    unsigned char *data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(DataFile.data()), DataFile.size(), &width, &height, &nChannels, 0);

    if (data == NULL)
        qDebug("Unable to load texture: %s", "assets/defaultXY.png");


    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(nChannels==3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if(nChannels==4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

}

Texture::~Texture()
{
}

void Texture::bind(GLushort unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

//void Texture::bind(GLuint unit) const
//{
//    textureData->bind(unit);
//}

//std::map<std::string, std::weak_ptr<TextureData>> textureCache;
//Texture::Texture(const Asset &file, GLenum textureTarget, GLfloat filter)
//{
//    auto it = textureCache.find(file.getIOStream()->getFileName());

//    if(it == textureCache.end() || !(textureData = it->second.lock())) {
//        int x, y, bytesPerPixel;
//        unsigned char* data = stbi_load_from_memory(reinterpret_cast<const unsigned char *>(file.read()), file.getIOStream()->fileSize(), &x, &y, &bytesPerPixel, 4);

//        if (data == NULL) {
//            log_err("Unable to load texture: %s", file.getIOStream()->getFileName().c_str());
//        }
//        else {
//            textureData = std::make_shared<TextureData>(x, y, data, textureTarget, filter);
//            textureCache[file.getIOStream()->getFileName()] = textureData;
//            stbi_image_free(data);
//        }
//    }
//}
