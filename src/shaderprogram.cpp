#include <QFile>

#include "shaderprogram.h"

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::ShaderProgram(const std::string &shaderName)
{
    initShaders(shaderName);
}

QString versionedShaderCode(const QString &src)
{
    QString versionedSrc;

    if (QOpenGLContext::currentContext()->isOpenGLES())
       return versionedSrc.append("#version 300 es\n" + src);
    else
       return versionedSrc.append("#version 430\n" + src);
}

void ShaderProgram::initShaders(const std::string &shaderName)
{
    // Compile vertex shader

//    stream.setCodec("UTF-8");
//    QFile filevertexShaderSource(std::string(":/shaders/vshader.glsl").c_str());
    QFile filevertexShaderSource(std::string(":/shaders/").append(shaderName).append(".vs").c_str());
    if (!filevertexShaderSource.open(QIODevice::ReadOnly|QFile::Text))
    {
        qDebug() << "Can not open file ";
//        close();
    }
    QTextStream invs(&filevertexShaderSource);
    QString vsSource = invs.readAll();
//    /*qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << */versionedShaderCode(vsSource);

    filevertexShaderSource.close();

    if (!addShaderFromSourceCode(QOpenGLShader::Vertex, versionedShaderCode(vsSource)))
    {
        qDebug() << "Error: Vertex shader, versionedShaderCode(vsSource)";
//        close();
    }
//    QFile fragmentShaderSource(std::string(":/shaders/fshader.glsl").c_str());
    QFile fragmentShaderSource(std::string(":/shaders/").append(shaderName).append(".fs").c_str());
    if (!fragmentShaderSource.open(QIODevice::ReadOnly|QFile::Text))
    {
        qDebug() << "Can not open file ";
//        close();
    }
    QTextStream infs(&fragmentShaderSource);
    QString fsSource = infs.readAll();
    /*qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << */versionedShaderCode(fsSource);
    fragmentShaderSource.close();

    // Compile fragment shader
    if (!addShaderFromSourceCode(QOpenGLShader::Fragment, versionedShaderCode(fsSource)))
    {
        qDebug() << "Error: Fragment shader, versionedShaderCode(fsSource)! ";
//        close();
    }
    // Link shader pipeline
    if (!link())
    {
        qDebug() << "Error: Shader program link!";
//        close();
    }

    // Bind shader pipeline for use
    if (!bind())
    {
        qDebug() << "Error: Shader program bind!";
//        close();
    }
    initializeOpenGLFunctions();

    glUniform1i(glGetUniformLocation(programId(), "albedoTexture"), 0);
    glUniform1i(glGetUniformLocation(programId(), "normalTexture"), 1);
}
