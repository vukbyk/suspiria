#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

layout (location = 0) in vec3 pos;

out vec3 WorldPos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    WorldPos = pos;
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}
