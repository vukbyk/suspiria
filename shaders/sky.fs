#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skyCube;

void main()
{
    float gamma = 2.2;
    // vec3 diffuseColor = pow(texture(diffuse, texCoords).rgb, vec3(gamma));
    // FragColor = pow(texture(skyCube, TexCoords), vec4(gamma)/2);
    FragColor = pow(texture(skyCube, TexCoords), vec4(gamma));
    // FragColor = texture(skyCube, TexCoords);
}
