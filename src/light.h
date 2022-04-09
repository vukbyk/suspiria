#pragma once

#include "spacial.h"
#include "shaderprogram.h"

class Light: public Spacial
{
    class ShaderProgram *shaderProgram = nullptr;
    GLint lightID;
public:
    Light(ShaderProgram *aShaderProgram);
    void renderAll();
};

