#pragma once

#if defined(GLES2)
#include <GLES2/gl2.h>
#elif defined(GLES3)
#include <GLES3/gl3.h>
#else
//#include <GL/glew.h>
#endif

//#include "Asset.h"
//#include "TextureData.h"


#include <QOpenGLExtraFunctions>

class Texture: protected QOpenGLExtraFunctions
{
    GLuint textureId;
    GLenum textureTarget;

//    std::shared_ptr<TextureData> textureData;
public:
//    Texture(const Asset &file, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR);
    Texture();
//    Texture(const std::string &fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR);
    ~Texture();

    //  void bind(GLuint unit = 0) const;
    void bind(GLushort unit);
};
