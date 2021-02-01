#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

in vec2 uvFrag;

out vec4 FragColor;

uniform sampler2D albedoTexture;

void main()
{
    vec3 color = texture(albedoTexture, uvFrag).rgb;
    FragColor = vec4(color, 1.0);
}
