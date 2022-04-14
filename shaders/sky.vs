#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

layout (location = 0) in vec3 pos;

uniform mat4 view;
uniform mat4 projection;


out vec3 TexCoords;

void main()
{
    TexCoords = pos;
    mat4 staticView = view;
    staticView[3].xyz = vec3(0.0); //can be removed on CPU
    vec4 position = projection * staticView * vec4(pos, 1.0);
    gl_Position = position.xyww;
}



