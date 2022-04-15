#include <QFile>

#include "shaderprogram.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>


ShaderProgram::ShaderProgram(const std::string &shaderGroupName)
{
    initShaders(shaderGroupName);
}

ShaderProgram::ShaderProgram(const std::string &shaderGroupName,
                             std::vector<std::string> aTextureUniformName)
{
    initShaders(shaderGroupName);
    setUniformNamesAndIds(aTextureUniformName);
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

void ShaderProgram::initShaders(const std::string &shaderGroupName)
{
    initializeOpenGLFunctions();

    // Compile vertex shader

//    stream.setCodec("UTF-8");
//    QFile filevertexShaderSource(std::string(":/shaders/vshader.glsl").c_str());
    QFile filevertexShaderSource(std::string(":/shaders/").append(shaderGroupName).append(".vs").c_str());
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
    QFile fragmentShaderSource(std::string(":/shaders/").append(shaderGroupName).append(".fs").c_str());
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
    projectionid = glGetUniformLocation(programId(), "projection");
    view = glGetUniformLocation(programId(),"view");
}

void ShaderProgram::bindShader()
{
    // Bind shader pipeline for use
    if (!bind())
        qDebug() << "Error: Shader program bind!";
//    glUniform1i(glGetUniformLocation(programId(), "albedoTexture"), 0);
//    glUniform1i(glGetUniformLocation(programId(), "normalTexture"), 1);
}

void ShaderProgram::setTextureUniforms()
{
    for(GLuint i=0; i<textureUniformId.size(); i++)
    {
        glUniform1i(textureUniformId[i], i);
    }
}

void ShaderProgram::setProjectionMat(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    glUniformMatrix4fv(projectionid, 1, GL_FALSE, &projection[0][0]);
}

void ShaderProgram::setProjectionMat(const GLfloat *projectionMat)
{
    glUniformMatrix4fv(projectionid, 1, GL_FALSE, projectionMat);
}

void ShaderProgram::setViewMat(const GLfloat viewMat[])
{
    glUniformMatrix4fv(view, 1, GL_FALSE, viewMat);//&mtm[0][0]);
}

void ShaderProgram::bindSetPVMat(const GLfloat *projectionMat,  GLfloat viewMat[])
{
    bindShader();
    setProjectionMat(projectionMat);
    setViewMat(viewMat);
}

void ShaderProgram::setUniformNamesAndIds(std::vector<std::string> aTextureUniformName)
{
    textureUniformName = aTextureUniformName;
    for(GLuint i=0; i<textureUniformName.size(); i++)
    {
        textureUniformId.push_back(glGetUniformLocation(programId(), aTextureUniformName[i].c_str()));
    }
}
