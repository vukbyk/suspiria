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
//    const float gamma = 2.2;             //instead of SRGB default 2.2
////    color = pow(color, vec3( gamma));    //instead of SRGB,
//    color = pow(color, vec3(1.0/gamma) );// 1.0/gamma for brigthen up
    FragColor = vec4(color, 0.5);
}
