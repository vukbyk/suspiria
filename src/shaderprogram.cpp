#include <QFile>

#include "shaderprogram.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::ShaderProgram(const std::string &shaderName)
{
    initShaders(shaderName);
}


GLuint ShaderProgram::getUniform(const char *name)
{
    return glGetUniformLocation(programId(), name);
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
    initializeOpenGLFunctions();

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
}

void ShaderProgram::bindShader()
{
    // Bind shader pipeline for use
    if (!bind())
    {
        qDebug() << "Error: Shader program bind!";
//        close();
    }
    glUniform1i(glGetUniformLocation(programId(), "albedoTexture"), 0);
    glUniform1i(glGetUniformLocation(programId(), "normalTexture"), 1);
}

void ShaderProgram::setProjectionMat(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    GLint projectionid = glGetUniformLocation(programId(), "projection");
    glUniformMatrix4fv(projectionid, 1, GL_FALSE, &projection[0][0]);
}

void ShaderProgram::setProjectionMat(const GLfloat *projectionMat)
{
    GLint projectionid = glGetUniformLocation(programId(), "projection");
    glUniformMatrix4fv(projectionid, 1, GL_FALSE, projectionMat);
}

void ShaderProgram::setViewMat(const GLfloat viewMat[])
{
    GLint view = glGetUniformLocation(programId(),"view");
    glUniformMatrix4fv(view, 1, GL_FALSE, viewMat);//&mtm[0][0]);
}

void ShaderProgram::bindSetPVMat(const GLfloat *projectionMat, const GLfloat viewMat[])
{
    bindShader();
    setProjectionMat(projectionMat);
    setViewMat(viewMat);
}
