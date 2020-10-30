#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

out vec4 FragColor;

in VSO {
    vec3 FragPos;
    vec2 uvFrag;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fsi;

uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;

in vec3 lightPosition;
in vec3 viewPosition;

in vec3 normalTest;

float far=100;
float near = .3;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    vec3 lightColor = vec3(2.0, 1.8, 1.4);

//    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
//    vec3 result = vec3(1,1,1)-vec3(depth);// To DELETE

//    //Test: no normals
//    vec3 colorT = texture(albedoTexture, fsi.uvFrag).rgb;
//    vec3 ambientt = 0.1 * colorT;
//    vec3 relVecT = lightPosition - fsi.FragPos;
//    float distT = length(relVecT);

//    vec3 lightDirT = normalize(relVecT);
//    vec3 nnormalTest = normalize(normalTest);
//    float diffintensT = max(dot(nnormalTest, lightDirT), 0.0);

//    float attenuationT = 1 / (distT);// * distT);
//    float diffT = diffintensT * attenuationT;

//    result = diffT * colorT * lightColor;
//    FragColor = vec4(texture(normalTexture, fsi.uvFrag).rgb, 1);//vec4(result, 1);
//    //TEST

    float dist = length(fsi.TangentFragPos - fsi.TangentLightPos);
    float attenuation = 1.0 / (dist);// * dist );

     // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalTexture, fsi.uvFrag).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
//    normal*= vec3(1.0, -1.0, 1.0);

    // get diffuse color
    vec3 color = texture(albedoTexture, fsi.uvFrag).rgb;

    // ambient
    vec3 ambient = 0.2 * color;

    // diffuse
    vec3 lightDir = normalize(fsi.TangentLightPos - fsi.TangentFragPos);
    float diffIntensity = max(dot(lightDir, normal), 0.0) ;
    vec3 diffuse = diffIntensity * color * lightColor * attenuation;

    // specular
//    vec3 viewDir = normalize(fsi.TangentViewPos - fsi.TangentFragPos);
    vec3 viewDir = normalize(fsi.TangentViewPos - fsi.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.1) * spec * lightColor;

//    result = texture(albedoTexture, fsi.uvFrag).rgb;
    //    vec4 aten = vec4(1.0 / (ambient + diffuse * dist + spec * (dist * dist)), 1.0);
    FragColor = vec4(ambient + diffuse + specular, 1.0);
//    FragColor = vec4(result, 1);

    //FragColor = vec4(ambient + diffuse , 1.0);
    //    FragColor = vec4(bi , 1) ;
    //    FragColor = vec4(len,0,0,0);
}

////in vec3 FragPos;
////in vec2 uvFrag;
//in vec3 nrm;
//in vec3 tg;
//in vec3 bi;
//in vec3 lightPosition;
//in mat3 TBN;

//uniform float unif;
//uniform vec4 ourColor;

//void main()
//{
//    float ambientStrength = 0.2;
//    vec3 lightColor = vec3(1.0,1.0,1.0);
//    vec3 ambient = lightColor * ambientStrength;
////    vec3 result = ambient * albedoTexture;
////    FragColor = vec4(ambient, 1.0);
////    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
////    vec4 fc=texture(albedoTexture, fuv) * ambientStrength;
////    FragColor = fc * vec4(1.0, 1.0, 1.0, 1.0)


////    vec3 lightPos = vec3(0.3, 0.3, -4.47);
//    vec3 lightPos = lightPosition;
//    vec3 nnrm = normalize(nrm);

//    // obtain normal from normal map in range [0,1]
//    vec3 normal = texture(normalTexture, fsi.uvFrag).rgb;
//    // transform normal vector to range [-1,1]
//    normal = normalize(normal * 2.0 - 1.0);
//    normal*=vec3(1.0, -1.0, 1.0);

//    vec3 toLightDir = normalize(lightPos - fsi.FragPos);

//    vec3 lightOrientation = normalize(vec3(0.0, .5, 3.5));
////    nnrm = TBN * normal;
//    nnrm=normalize(nrm);

//    float diff = max(dot(nnrm, toLightDir), 0.0);
//    if (dot(lightOrientation, toLightDir) < -1)//-1 for pointlight ~0.9 spotlight
//        diff = 0.0;

//    vec3 diffuse = diff * lightColor;

//    vec3 result = (ambientStrength + diffuse);
////    FragColor = vec4(normal,1.0);
////    FragColor = vec4(0.66, 0.66, 0.66, 1.0) * vec4(result, 1.0);
//    FragColor = texture(albedoTexture, fsi.uvFrag) * vec4(result, 1.0);
////    FragColor = texture(/*albedoTexture*/normalTexture, uvFrag) * vec4(result, 1.0);
////    FragColor = mix(texture(albedoTexture, uvFrag), texture(normalTexture, uvFrag), 0.2);
////    FragColor = texture(albedoTexture, uvFrag) * ambientStrength;
////    FragColor = vec4(nnrm , 1) ; //show normal
////    FragColor = vec4(tg , 1) ; //show tangent
////    FragColor = vec4(TBN. , 1) ;
////     FragColor = vec4(fuv.xyx, 1.0);
////     FragColor = vec4(1.0, 1.0, 1.0, 1.0);
////     FragColor = ourColor;
//}
