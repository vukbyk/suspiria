#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(pos, 1.0);
}
