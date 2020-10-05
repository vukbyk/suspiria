#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>

class ShaderProgram: public QOpenGLShaderProgram, protected QOpenGLExtraFunctions
{
public:
    void initShaders();
};
