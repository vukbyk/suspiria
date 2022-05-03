#pragma once

#include <QtOpenGL/QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>


class ShaderProgram: public QOpenGLShaderProgram, protected QOpenGLExtraFunctions
{
protected:
    GLint projectionid;
    GLint view;
    std::vector<std::string> textureUniformName;
    std::vector<GLuint> textureUniformId;
public:
    ShaderProgram(const std::string &shaderName);
    ShaderProgram(const std::string &vsShaderName, const std::string &fsShaderName);
//    ShaderProgram(const std::string &shaderName,
//                  std::vector<std::string> aTextureUniformList);
    void initShaders(const std::string &shaderName);
    void initShaders(const std::string &vsShaderName, const std::string &fsShaderName);

    GLuint getUniform(const char* name);
    void bindShader();
    void setProjectionMat(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar);
    void setProjectionMat(const GLfloat *projectionMat);
    void setViewMat(const GLfloat viewMat[]);
    void bindSetPVMat(const GLfloat *projectionMat,  GLfloat viewMat[]);
    void getAllUniformLocation();
    void setUniformNamesAndIds(std::vector<std::string> aTextureUniformName);
    void setTextureUniforms();
};
