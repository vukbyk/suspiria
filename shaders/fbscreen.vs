#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif


layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;



//layout (location = 0) in vec2 pos;
//layout (location = 1) in vec2 uv;


out vec2 TexCoords;

void main()
{
    TexCoords = uv;
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
}
