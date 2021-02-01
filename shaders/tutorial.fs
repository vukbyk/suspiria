#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

out vec4 FragColor;

//in VSO {
//    vec3 FragPos;
//    vec2 uvFrag;
//    vec3 TangentLightPos;
//    vec3 TangentViewPos;
//    vec3 TangentFragPos;
//} fsi;

in vec3 FragPos;
in vec2 uvFrag;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;

in vec3 lightPosition;
in vec3 viewPosition;

in vec3 normalTest;

//float far=100.0;
//float near = .3;
//
//float LinearizeDepth(float depth)
//{
//    float z = depth * 2.0 - 1.0; // back to NDC
//    return (2.0 * near * far) / (far + near - z * (far - near));
//}

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0)*50.0;

//    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
//    vec3 result = vec3(1,1,1)-vec3(depth);// To DELETE

//    //Test: no normals
//    vec3 colorT = texture(albedoTexture, uvFrag).rgb;
//    vec3 ambientt = 0.1 * colorT;
//    vec3 relVecT = lightPosition - FragPos;
//    float distT = length(relVecT);

//    vec3 lightDirT = normalize(relVecT);
//    vec3 nnormalTest = normalize(normalTest);
//    float diffintensT = max(dot(nnormalTest, lightDirT), 0.0);

//    float attenuationT = 1 / (distT);// * distT);
//    float diffT = diffintensT * attenuationT;

//    result = diffT * colorT * lightColor;
//    FragColor = vec4(texture(normalTexture, uvFrag).rgb, 1);//vec4(result, 1);
//    //TEST

    float dist = length(TangentFragPos - TangentLightPos);
    float attenuation = 1.0 / ( dist * dist );

     // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalTexture, uvFrag).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
//    normal*= vec3(1.0, -1.0, 1.0);

    // get diffuse color

    vec3 color = texture(albedoTexture, uvFrag).rgb;
//    const float gamma = 2.2;             //instead of SRGB default 2.2
//    color = pow(color, vec3( gamma));    //instead of SRGB,
//    color = pow(color, vec3(1.0/gamma) );// 1.0/gamma for brigthen up

    // ambient
    vec3 ambient = 0.1 * color;

    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diffIntensity = max(dot(lightDir, normal), 0.0) ;
    vec3 diffuse = diffIntensity * color * lightColor * attenuation;

    // specular
//    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(.75) * spec * lightColor * attenuation;

//    result = texture(albedoTexture, uvFrag).rgb;
    //    vec4 aten = vec4(1.0 / (ambient + diffuse * dist + spec * (dist * dist)), 1.0);
    FragColor = vec4(ambient + diffuse + specular, 1.0);


//    FragColor = vec4(result, 1);

    //FragColor = vec4(ambient + diffuse , 1.0);
    //    FragColor = vec4(bi , 1) ;
    //    FragColor = vec4(len,0,0,0);
}
