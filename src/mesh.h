#pragma once

//#if defined(GLES2)
//  #include <GLES2/gl2.h>
//#elif defined(GLES3)
//  #include <GLES3/gl3.h>
//#else
//  #include <GL/glew.h>
//#endif

#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "vertex.h"

class Mesh: protected QOpenGLExtraFunctions
{
public:
    Mesh(Mesh &other);
    Mesh(const std::string aFile);
    Mesh(const Vertex vertices[], const  GLuint vertSize, const  GLuint indices[],const  GLuint indexSize);
    virtual ~Mesh(void);
    void render(void);

private:
    void createMesh(const Vertex vertices[], const GLuint vertSize, const GLuint indices[], const GLuint indexSize);

    GLuint VAO;
    GLuint VBO;
    GLuint IBO;

    int indicesSize, verticesSize;
};
