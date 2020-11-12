#pragma once

#include <string>

#include <QOpenGLExtraFunctions>

class Texture: protected QOpenGLExtraFunctions
{
    GLuint textureId;
//    GLenum textureTarget;

//    std::shared_ptr<TextureData> textureData;
public:
    Texture(const std::string &fileName="defaultXY.png", const bool gammaCorrection=false );//, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR);
    ~Texture();

    void bind(GLushort unit);

    //  void bind(GLuint unit = 0) const;
};
