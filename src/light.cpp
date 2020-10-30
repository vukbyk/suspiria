#include "light.h"
//#include "shaderprogram.h"
#include <glm/gtc/type_ptr.hpp>

Light::Light(ShaderProgram *aShaderProgram)
{
    shaderProgram = aShaderProgram;
}

void Light::renderAll()
{
    initializeOpenGLFunctions();
    GLint lightID = glGetUniformLocation(shaderProgram->programId(), "light");
    glUniformMatrix4fv(lightID, 1, GL_FALSE, glm::value_ptr(transform.getTransformMatrix()) );//&mtm[0][0]);
}
