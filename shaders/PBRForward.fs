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

in mat3 TBN;
in mat3 inverseTBN;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

//in vec3 BoxPosition;
//in vec3 ReflectionVector;

//uniform vec3 camPos;
uniform samplerCube skyCube;

// IBL
//uniform samplerCube irradianceMap;
//uniform samplerCube prefilterMap;

const float PI = 3.14159265359;

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
//    vec3 lightColor = vec3(1.0, 1.0, 1.0)*5.0;

//    float dist = length(TangentFragPos - TangentLightPos);
//    float attenuation = 1.0 / ( dist * dist );

     // obtain normal from normal map in range [0,1]
//    vec3 normal = texture(normalMap, uvFrag).rgb;
    // transform normal vector to range [-1,1]
//    normal = normalize(normal * 2.0 - 1.0); //this normal is in tangent space
//                                            //for switching light and loc in tan space

//    // Get diffuse color
//    vec3 color = texture(albedoMap, uvFrag).rgb;

    // Diffuse
//    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
//    float diffIntensity = max(dot(lightDir, normal), 0.0) ;
//    vec3 diffuse = diffIntensity * color * lightColor * attenuation;

//    // Specular
//    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
//    vec3 reflectDir = reflect(-lightDir, normal);
//    vec3 halfwayDir = normalize(lightDir + viewDir);
//    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
//    vec3 specular = vec3(.75) * spec * lightColor * attenuation;

//    // Works
//    vec3 I = normalize(TangentFragPos - TangentViewPos );
//    vec3 R  = reflect( I, normal);
//    vec3 reflection  = vec3(texture(skyCube, R*inverseTBN));

//    color = mix(reflection, color, .5);
//    const float gamma = 2.2;             //instead of SRGB default 2.2
////    color = pow(color, vec3( gamma));    //instead of SRGB,
//    color = pow(color, vec3(1.0/gamma) );// 1.0/gamma for brigthen up

    // Ambient
//    vec3 ambient = color*  reflection *.5; //0.1 * color  ;
//    ambient = mix(ambient, texture(reflectionBoxTexture, reflection).rgb, .5);




    vec3 albedo     = pow(texture(albedoMap, uvFrag).rgb, vec3(2.2));
    float metallic  = texture(metallicMap, uvFrag).r;
    float roughness = texture(roughnessMap, uvFrag).r;
    float ao        = texture(aoMap, uvFrag).r;

    vec3 lightColor = vec3(1.0, 1.0, 1.0)*5.0;
    float dist = length(TangentFragPos - TangentLightPos);
    vec3 normal = texture(normalMap, uvFrag).rgb;
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    normal = normalize(normal * 2.0 - 1.0);

    vec3 N = normal;
    vec3 V = viewDir;

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
//   for(int i = 0; i < 4; ++i)
   {
       // calculate per-light radiance
       vec3 L = lightDir; //(lightPositions[i] - WorldPos);
       vec3 H = normalize(V + L);
       float distance = dist;//length(lightPositions[i] - WorldPos);
       float attenuation = 1.0 / (distance * distance);
       vec3 radiance = lightColor/*s[i]*/ * attenuation;

       // Cook-Torrance BRDF
       float NDF = DistributionGGX(N, H, roughness);
       float G   = GeometrySmith(N, V, L, roughness);
       vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

       vec3 numerator    = NDF * G * F;
       float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
       vec3 specular = numerator / denominator;

       // kS is equal to Fresnel
       vec3 kS = F;
       // for energy conservation, the diffuse and specular light can't
       // be above 1.0 (unless the surface emits light); to preserve this
       // relationship the diffuse component (kD) should equal 1.0 - kS.
       vec3 kD = vec3(1.0) - kS;
       // multiply kD by the inverse metalness such that only non-metals
       // have diffuse lighting, or a linear blend if partly metal (pure metals
       // have no diffuse light).
       kD *= 1.0 - metallic;

       // scale light by NdotL
       float NdotL = max(dot(N, L), 0.0);

       // add to outgoing radiance Lo
       Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
   }

   // ambient lighting (note that the next IBL tutorial will replace
   // this ambient lighting with environment lighting).
   // ambient lighting (we now use IBL as the ambient term)
   vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

   vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
   vec3 kD= 1.0 - kS;
   kD *= 1.0 - metallic;

//       vec3 irradiance = texture(BoxPosition, N).rgb;
   //to TBN
   vec3 I = normalize(TangentFragPos - TangentViewPos );
   vec3 R  = reflect( I, N);//normal);
   vec3 R_world = TBN * R;
   vec3 irradiance  = vec3(texture(skyCube, R*inverseTBN));

   vec3 diffuse      = irradiance * albedo ;

   // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
//   const float MAX_REFLECTION_LOD = 4.0;
//   vec3 prefilteredColor = textureLod(/*prefilterMap*/skyCube, R,  roughness * MAX_REFLECTION_LOD).rgb;
//   vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
//   vec3 specular = prefilteredColor * (F * 1);//brdf.x + brdf.y);

//   vec3 ambient = (kD * diffuse + specular) * ao;
   vec3 ambient = (kD * diffuse) * ao;
//   vec3 ambient = vec3(0.03) * albedo * ao;
   // vec3 ambient = vec3(0.002);

   vec3 color = ambient + Lo;

   // HDR tonemapping
   color = color / (color + vec3(1.0));
   // gamma correct
   color = pow(color, vec3(1.0/2.2));

   FragColor = vec4(color, 1.0);

}
