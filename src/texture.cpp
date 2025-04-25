#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <QFile>

#ifdef GL_ES_VERSION_2_0
    #include <qopengles2ext.h>
    #define GL_RED					0x1903
    #define GL_RGB8					0x8051
    #define GL_SRGB8                0x8C41
#endif

Texture::Texture(const std::string &fileName, const bool gammaCorrection, const bool flipNormalVertically)//, GLenum aTextureTarget, GLfloat filter)
{
    initializeOpenGLFunctions();
    stbi_set_flip_vertically_on_load(flipNormalVertically);

    int width, height, nChannels;

    qDebug("Loading texture: %s", std::string(":/assets/").append(fileName).c_str());

    QFile file( std::string(":/assets/").append(fileName).c_str() );
    if(!file.open(QIODevice::ReadOnly))
        qDebug("!!! ERROR !!! Failed to load file %s", fileName.c_str());
    else
        qDebug("File %s is loaded", fileName.c_str());

    QByteArray DataFile = file.readAll();
    unsigned char *data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(DataFile.data()),
                                                DataFile.size(), &width, &height, &nChannels, 0);

    if (data == NULL)
    {
        qDebug("Unable to load texture: %s", fileName.c_str());
        return;
    }

    GLenum internalFormat=0;
    GLenum dataFormat = GL_RGB;
    if (nChannels == 1)
    {
        internalFormat = GL_R8;          // Internal storage (8-bit red channel)
        dataFormat    = GL_RED;         // Pixel data format (single channel)
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
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0,
                 dataFormat, GL_UNSIGNED_BYTE, data);

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------
Texture::Texture(std::vector<std::string> faces, const bool flip, const bool gammaCorrection)
{
    initializeOpenGLFunctions();
    stbi_set_flip_vertically_on_load(flip);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    // generate mipmaps for all 6 faces
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    int width, height, nChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        qDebug("Loading texture: %s", std::string(":/assets/").append(faces[i]).c_str());

        QFile file( std::string(":/assets/").append(faces[i]).c_str() );
        if(!file.open(QIODevice::ReadOnly))
            qDebug("!!! ERROR !!!  Failed to load file %s", faces[i].c_str());
        else
            qDebug("File %s is loaded", faces[i].c_str());

        QByteArray DataFile = file.readAll();

        unsigned char *data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(DataFile.data()),
                                                    DataFile.size(), &width, &height, &nChannels, 0);
        if (data == NULL)
        {
            qDebug("Unable to load texture: %s", faces[i].c_str());
            return;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }


    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 8);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

}

Texture::~Texture()
{
}

//void Texture::bind(GLushort unit)
//{
//    glActiveTexture(GL_TEXTURE0 + unit );
//    glBindTexture(GL_TEXTURE_2D, textureId);
//}

GLuint Texture::getTextureId() const
{
    return textureId;
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
