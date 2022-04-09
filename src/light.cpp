#include "light.h"
//#include "shaderprogram.h"
#include <glm/gtc/type_ptr.hpp>
#include "components.h"
#include "scene.h"

Light::Light(ShaderProgram *aShaderProgram)
{
    shaderProgram = aShaderProgram;
    lightID = shaderProgram->getUniform( "light");
}

void Light::renderAll()
{
    initializeOpenGLFunctions();
//    glUniformMatrix4fv(lightID, 1, GL_FALSE, glm::value_ptr(transform.getTransformMatrix()) );//&mtm[0][0]);
//    Transform t = transform;
//    glUniformMatrix4fv(lightID, 1, GL_FALSE, glm::value_ptr(t.getTransformMatrix()) );//&mtm[0][0]);

    auto s = dynamic_cast<Scene*>(parentSpacial);
    if(s)
    {
        auto &m = s->world.get<TransformComponent>(entity);
        glUniformMatrix4fv(lightID, 1, GL_FALSE, glm::value_ptr(m.transform.getTransformMatrix()) );//&mtm[0][0]);
    }
}
