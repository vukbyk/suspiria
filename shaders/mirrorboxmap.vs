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
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPosCam;

out vec3 ReflectionVector;
//out vec3 RefractionVector;
out vec3 Position;

void main()
{
    ReflectionVector = mat3(transpose(inverse(model))) * nor;
    Position = vec3(model * vec4(pos, 1.0));
//    gl_Position = projection * view * vec4(pos, 1.0);
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
