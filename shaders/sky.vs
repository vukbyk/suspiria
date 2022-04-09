#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

layout (location = 0) in vec3 pos;
//layout (location = 1) in vec2 uv;
//layout (location = 2) in vec3 nor;
//layout (location = 3) in vec3 tng;
//layout (location = 4) in vec3 bit;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 light;

//out vec2 uvFrag;
//void main()
//{
////    uvFrag=uv;
//    mat4 staticView = view;
//    staticView[3].xyz = vec3(0.0);
//    gl_Position = projection * staticView /** model*/ * vec4(pos, 1.0);
//}

out vec3 TexCoords;

void main()
{
    TexCoords = pos;
    mat4 staticView = view;
    staticView[3].xyz = vec3(0.0); //can be removed on CPU
    vec4 position = projection * staticView * vec4(pos, 1.0);
    gl_Position = position.xyww;
}



