#pragma once

#include "spacial.h"
#include "shaderprogram.h"

class Light: public Spacial
{
    class ShaderProgram *shaderProgram = nullptr;
public:
    Light(ShaderProgram *aShaderProgram);
    void renderAll();
};

