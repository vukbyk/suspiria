#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 nor;
layout (location = 3) in vec3 tng;
layout (location = 4) in vec3 bit;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 light;
uniform mat4 view;//lightSpaceMatrix;??


void main()
{
//    gl_Position = /*lightSpaceMatrix*/view * model * vec4(pos, 1.0);
//    TexCoords = uv;
//    WorldPos = vec3(model * vec4(pos, 1.0));
//    Normal = mat3(model) * nor;
    gl_Position =  projection * view * model * vec4(pos, 1.0);
}
