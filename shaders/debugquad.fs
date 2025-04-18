#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
//uniform float near_plane;
//uniform float far_plane;

float near_plane =1.0f;
float far_plane = 50.0f;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
    float depthValue = texture(depthMap, TexCoords).r;
    // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}

