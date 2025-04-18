#pragma once

#include <QtOpenGL/QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>

//TODO: Shader system with VS and FS
class ShaderProgram: public QOpenGLShaderProgram, protected QOpenGLExtraFunctions
{
protected:
//    GLint projection;
//    GLint view;
    std::vector<std::string> textureUniformName;
    std::vector<GLuint> textureUniformId;
public:
    // class Entity *uniform;

    ShaderProgram(const std::string &shaderName);
    ShaderProgram(const std::string &vsShaderName, const std::string &fsShaderName);
//    ShaderProgram(const std::string &shaderName,
//                  std::vector<std::string> aTextureUniformList);
    ~ShaderProgram();
    void initShaders(const std::string &shaderName);
    void initShaders(const std::string &vsShaderName, const std::string &fsShaderName);

    GLuint getUniform(const char* name);
    void bindShader();
//    GLint getProjection() const;
//    void setProjectionMat(const GLfloat *projectionMat);
//    void setViewMat(const GLfloat *viewMat);
//    void getAllUniformLocation();
    void setUniformNamesAndIds(std::vector<std::string> aTextureUniformName);
    void setTextureUniforms();
//    void bindSetPVMat(const GLfloat *projectionMat,  GLfloat viewMat[]);
//    void setProjectionMat(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar);
};

class ShaderVP: public ShaderProgram
{
public:
    GLuint view;
    GLuint projection;

    ShaderVP( const std::string &shaderName): ShaderProgram(shaderName)
    { initUniforms(); };
    ShaderVP( const std::string &vsName, const std::string &fsName): ShaderProgram(vsName, fsName)
    { initUniforms(); };

    void initUniforms()
    {
        view=getUniform("view");
        projection=getUniform("projection");
    };
};

class ShaderMVP: public ShaderProgram
{
public:
    GLuint model;
    GLuint view;
    GLuint projection;

    ShaderMVP( const std::string &shaderName): ShaderProgram(shaderName)
    { initUniforms(); };
    ShaderMVP( const std::string &vsName, const std::string &fsName): ShaderProgram(vsName, fsName)
    { initUniforms(); };


    void initUniforms()
    {
        model=getUniform("model");
        view=getUniform("view");
        projection=getUniform("projection");
    };
};

class ShaderPBR: public ShaderProgram
{
public:
    GLuint model;
    GLuint view;
    GLuint projection;
    GLuint light;
    GLuint lightSpaceMat;

    ShaderPBR( const std::string &shaderName): ShaderProgram(shaderName)
    { initUniforms(); };
    ShaderPBR( const std::string &vsName, const std::string &fsName): ShaderProgram(vsName, fsName)
    { initUniforms(); };


    void initUniforms()
    {
        model=getUniform("model");
        view=getUniform("view");
        projection=getUniform("projection");
        light=getUniform("light");
        lightSpaceMat=getUniform("lightSpaceMat");
    };
};


