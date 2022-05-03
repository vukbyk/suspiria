#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

layout (location = 0) in vec3 pos;//aPos;
//layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoords;
layout (location = 1) in vec2 uv;//aTexCoords;
layout (location = 2) in vec3 nor;//aNormal;
layout (location = 3) in vec3 tng;//?
layout (location = 4) in vec3 bit;//?

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

//vuk: to remove?
uniform vec3 camPos;

void main()
{
    TexCoords = uv;
    WorldPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(model) * nor;

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}
