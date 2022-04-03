#pragma once

#include <QtOpenGL/QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>


class ShaderProgram: public QOpenGLShaderProgram, protected QOpenGLExtraFunctions
{
public:
    ShaderProgram();
    ShaderProgram(const std::string &shaderName);
    GLuint getUniform(const char* name);

    void initShaders(const std::string &shaderName);

    void bindShader();
    void setProjectionMat(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar);
    void setProjectionMat(const GLfloat *projectionMat);
    void setViewMat(const GLfloat viewMat[]);
    void bindSetPVMat(const GLfloat *projectionMat,  GLfloat viewMat[]);
};
