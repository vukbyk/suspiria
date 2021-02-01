#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <QFile>


#ifdef GL_ES_VERSION_2_0
    #include <qopengles2ext.h>
    #define GL_RED					0x1903
    #define GL_RGB8					0x8051
    #define GL_SRGB8                          0x8C41
#endif


Texture::Texture(const std::string &fileName, const bool gammaCorrection)//, GLenum aTextureTarget, GLfloat filter)
{
    initializeOpenGLFunctions();
    stbi_set_flip_vertically_on_load(true);

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
    {
        qDebug("Unable to load texture: %s", fileName.c_str());
        return;
    }



    GLenum internalFormat=0;
    GLenum dataFormat = GL_RGB;
    if (nChannels == 1)
    {
        internalFormat = dataFormat = GL_RED_BITS;
    }
    else if (nChannels == 3)
    {
        internalFormat = gammaCorrection ? GL_SRGB8 : GL_RGB8;
        dataFormat = GL_RGB;
    }
    else if (nChannels == 4)
    {
        internalFormat = gammaCorrection ? GL_SRGB8_ALPHA8_EXT : GL_RGBA;
        dataFormat = GL_RGBA;
    }
    else
    {
        qDebug("Unable to load texture, there is no solution if number of channels is %i", nChannels);
        return;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

//    glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
//    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, internalFormat, GL_UNSIGNED_BYTE, data);
//
//    if(nChannels==3)
//    {
//        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, width, height);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
////        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//    }
//    if(nChannels==4)
//    {
//        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, width, height);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
////        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//    }
//
//    if(nChannels==3)
//    {
//        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
////        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//    }
//    if(nChannels==4)
//    {
//        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
////        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
////        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//    }
//
//    if(nChannels==3)
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//    if(nChannels==4)
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);


//     set the texture wrapping/filtering options (on the currently bound texture object)
//    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);

}

Texture::~Texture()
{
}

void Texture::bind(GLushort unit)
{
    glActiveTexture(GL_TEXTURE0 + unit );
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
