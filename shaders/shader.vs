#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 v_texcoord;


void main()
{
    // Calculate vertex position in screen space
//    gl_Position = projection * view * vec4(pos, 1.0);
    gl_Position = projection * view * model * vec4(pos, 1.0);

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = uv;
}

//#ifdef GL_ES
//// Set default precision to medium
//precision mediump int;
//precision mediump float;
//#endif

//uniform mat4 mvp_matrix;

//attribute vec4 a_position;
//attribute vec2 a_texcoord;

//varying vec2 v_texcoord;

//void main()
//{
//    // Calculate vertex position in screen space
//    gl_Position = mvp_matrix * a_position;

//    // Pass texture coordinate to fragment shader
//    // Value will be automatically interpolated to fragments inside polygon faces
//    v_texcoord = a_texcoord;
//}

