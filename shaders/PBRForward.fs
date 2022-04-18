#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

out vec4 FragColor;

in vec3 FragPos;
in vec2 uvFrag;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

//in mat3 TBN;
in mat3 inverseTBN;

uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;

//in vec3 lightPosition;

in vec3 BoxPosition;
in vec3 ReflectionVector;

//uniform vec3 viewPosCam;
uniform samplerCube skyCube;

//in vec3 NormalTutorial;
//in vec3 Position;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0)*5.0;

    float dist = length(TangentFragPos - TangentLightPos);
    float attenuation = 1.0 / ( dist * dist );

     // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalTexture, uvFrag).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0); //this normal is in tangent space
                                            //for switching light and loc in tan space

    // Get diffuse color
    vec3 color = texture(albedoTexture, uvFrag).rgb;

    // Diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diffIntensity = max(dot(lightDir, normal), 0.0) ;
    vec3 diffuse = diffIntensity * color * lightColor * attenuation;

    // Specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(.75) * spec * lightColor * attenuation;

    // Works
    vec3 I = normalize(TangentFragPos - TangentViewPos );
    vec3 R  = reflect( I, normal);
    vec3 reflection  = vec3(texture(skyCube, R*inverseTBN));

//    color = mix(reflection, color, .5);
//    const float gamma = 2.2;             //instead of SRGB default 2.2
////    color = pow(color, vec3( gamma));    //instead of SRGB,
//    color = pow(color, vec3(1.0/gamma) );// 1.0/gamma for brigthen up

    // Ambient
    vec3 ambient = color*  reflection *.5; //0.1 * color  ;
//    ambient = mix(ambient, texture(reflectionBoxTexture, reflection).rgb, .5);

//    result = texture(albedoTexture, uvFrag).rgb;
//    vec4 aten = vec4(1.0 / (ambient + diffuse * dist + spec * (dist * dist)), 1.0);
    vec3 lightCombine = ambient + diffuse + specular;
    vec3 withReflection =   mix( reflection , lightCombine, 0.97);
//    FragColor = vec4(withReflection.rgb, 1);
    FragColor = vec4(ambient + diffuse + specular, 1.0);
//    FragColor = vec4(reflection.rgb, 1);

//    vec4 aten = vec4(1.0 / (ambient + diffuse * dist + spec * (dist * dist)), 1.0);
//FragColor = vec4( ambient + diffuse * (theta - cos(cutOff)) * 2 * cos(cutOff) + specular, 1.0);

//    if(theta > cos(cutOff))
//    {
//        // diffuse
//        vec3 lightTangentPosition = normalize(TangentLightPos - TangentFragPos);
//        float diffIntensity = max(dot(lightTangentPosition, normal), 0.0)  * attenuation;
//        vec3 diffuse = diffIntensity * color * lightColor * lightStrength;

//        // specular
//        vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
//        vec3 reflectDir = reflect(-lightTangentPosition, normal);
//        vec3 halfwayDir = normalize(lightTangentPosition + viewDir);
//        float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0) * attenuation;

//        vec3 specular = spec * lightColor * specularStrength;

//        //    vec4 aten = vec4(1.0 / (ambient + diffuse * dist + spec * (dist * dist)), 1.0);
//        FragColor = vec4( ambient + diffuse * (theta - cos(cutOff)) * 2 * cos(cutOff) + specular, 1.0);
//    }
//    else
//        FragColor = vec4(ambient, 1.0);


    //FragColor = vec4(result, 1);
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //vec3 result = vec3(1,1,1)-vec3(depth);// To DELETE
    //result = diffT * colorT * lightColor;
    //result = texture(albedoTexture, uvFrag).rgb;

    //FragColor = vec4(ambient + diffuse , 1.0);
    //    FragColor = vec4(bi , 1) ;
    //    FragColor = vec4(len,0,0,0);
}
