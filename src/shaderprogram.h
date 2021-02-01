#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>

class ShaderProgram: public QOpenGLShaderProgram, protected QOpenGLExtraFunctions
{
public:
    ShaderProgram();
    ShaderProgram(const std::string &shaderName);
    void initShaders(const std::string &shaderName);
    void bindShader();
};
