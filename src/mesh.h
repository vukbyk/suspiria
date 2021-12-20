#pragma once

//#if defined(GLES2)
//  #include <GLES2/gl2.h>
//#elif defined(GLES3)
//  #include <GLES3/gl3.h>
//#else
//  #include <GL/glew.h>
//#endif

#include <QOpenGLExtraFunctions>
#include <QtOpenGL/QOpenGLShaderProgram>
#include <QtOpenGL/QOpenGLBuffer>

#include "vertex.h"
//!!! Do not use, use assimp load instead !!!
class Mesh: protected QOpenGLExtraFunctions
{

public:
    GLuint VBO;
    GLuint IBO;
    GLuint verticesSize;

    GLuint VAO;
    GLuint indicesSize;

public:
    Mesh();
    Mesh(const Mesh &other);
    Mesh(const Vertex vertices[], const  GLuint vertSize, const  GLuint indices[],const  GLuint indexSize);
    virtual ~Mesh(void);
    void render(void);

//private:
    void createMesh(const Vertex vertices[], const GLuint vertSize, const GLuint indices[], const GLuint indexSize);

    GLuint getVAO() const;
    GLuint getIndicesSize() const;
};
