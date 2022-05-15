#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D shadowMap;

//uniform vec3 camPos;
uniform mat4 projection;
// IBL
uniform samplerCube skyCube;//irradianceMap;
//uniform samplerCube prefilterMap;
//uniform sampler2D brdfLUT;

// lights
//uniform vec3 lightPositions[4];
//uniform vec3 lightColors[4];

//Vuk:
uniform mat4 light;

in vec3 FragPos;

vec3 lightPosition=light[3].xyz;
vec3 camPos=projection[3].xyz;
//float near = 0.1;
//float far = 100.0;
//float LinearizeDepth(float depth)
//{
//    float z = depth * 2.0 - 1.0; // back to NDC
//    return (2.0 * near * far) / (far + near - z * (far - near));
////    return (camPos.z-near) / (far-near);
//}
vec2 CalcParallaxTexCoords(sampler2D dispMap, mat3 tbnMatrix, vec3 directionToEye, vec2 texCoords, float scale, float bias)
{
        return texCoords.xy + (directionToEye * tbnMatrix).xy * (texture2D(dispMap, texCoords.xy).r * scale + bias);
}

float SampleShadowMap(sampler2D shadowMap, vec2 coords, float compare)
{
        return step(compare, texture2D(shadowMap, coords.xy).r);
}

float SampleShadowMapLinear(sampler2D shadowMap, vec2 coords, float compare, vec2 texelSize)
{
        vec2 pixelPos = coords/texelSize + vec2(0.5);
        vec2 fracPart = fract(pixelPos);
        vec2 startTexel = (pixelPos - fracPart) * texelSize;

        float blTexel = SampleShadowMap(shadowMap, startTexel, compare);
        float brTexel = SampleShadowMap(shadowMap, startTexel + vec2(texelSize.x, 0.0), compare);
        float tlTexel = SampleShadowMap(shadowMap, startTexel + vec2(0.0, texelSize.y), compare);
        float trTexel = SampleShadowMap(shadowMap, startTexel + texelSize, compare);

        float mixA = mix(blTexel, tlTexel, fracPart.y);
        float mixB = mix(brTexel, trTexel, fracPart.y);

        return mix(mixA, mixB, fracPart.x);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
//    if(projCoords.z > 1.0)
//        projCoords.z = 0.0;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
//    float bias = 0.005;
    //TODO unite calculation for light Direction
//    vec3 L = normalize(lightPosition/*s[i]*/ - WorldPos);
    float bias = max(0.0005 * (1.0 - dot(Normal, lightDir)), 0.0002);// for glCullFace(GL_FRONT);
//      float bias = max(0.005 * (1.0 - dot(Normal, lightDir)), 0.002);// for glCullFace(GL_BACK);
//    float bias = 0.0005;
//    float shadow = (currentDepth /*- bias*/ > closestDepth + bias)  ? 1.0 : 0.0;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
//////////////

//    shadow /= 25.0;//9.0 // for -1 to +1;
//    for(int x = -3; x <= 3; ++x)
//    {
//        for(int y = -3; y <= 3; ++y)
//        {
//            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
//            shadow += currentDepth /*- bias*/ > pcfDepth + bias ? 1.0 : 0.0;
//        }
//    }
//    shadow /= 49.0;//81.0;//49.0;//25.0;//9.0 // for -1 to +1;
//    if(projCoords.z > 1.0)
//    {
//        shadow = 0.0;
//    }
//    return 1-shadow;
////////////////
//    float SampleShadowMapPCF(sampler2D shadowMap, vec2 coords, float compare, vec2 texelSize)
//    {
    const float NUM_SAMPLES = 3.0f;
    const float SAMPLES_START = (NUM_SAMPLES-1.0f)/2.0f;
    const float NUM_SAMPLES_SQUARED = NUM_SAMPLES*NUM_SAMPLES;
    if(projCoords.y >= 1.0)
    {
        shadow = 0.0;
    }
    for(float y = -SAMPLES_START; y <= SAMPLES_START; y += 1.0f)
    {
        for(float x = -SAMPLES_START; x <= SAMPLES_START; x += 1.0f)
        {
                vec2 coordsOffset = vec2(x,y)*texelSize;
                shadow += SampleShadowMapLinear(shadowMap, projCoords.xy + coordsOffset, currentDepth - bias, texelSize);
        }
    }
//    return currentDepth;

    return shadow/NUM_SAMPLES_SQUARED;
////    }

}

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal
// mapping the usual way for performance anways; I do plan make a note of this
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
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
// ----------------------------------------------------------------------------


void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0)*5.0;
    // material properties
    vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    //Debug trick for projection from Vedran
    //vec3 albedo = pow(texture(albedoMap, FragPosLightSpace.xy).rgb, vec3(2.2));
    float metallic = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;
    float ao = texture(aoMap, TexCoords).r;

    // input lighting data
    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - WorldPos);
    vec3 R = reflect(-V, N);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    //Vuk: quick fix

    float shadow;
//    for(int i = 0; i < 4; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPosition/*s[i]*/ - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPosition/*s[i]*/ - WorldPos);
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
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
        shadow = ShadowCalculation(FragPosLightSpace, L);
    }

    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 I = normalize(WorldPos - camPos );
    vec3 RR  = reflect( I, N);//normal);
    vec3 irradiance  = vec3(texture(skyCube, RR));
//    vec3 irradiance = texture(/*irradianceMap*/skyCube, N).rgb;
    vec3 diffuse      = irradiance * albedo;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
//    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
//    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
//    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse /*+ specular*/) * ao * 0.001;
    vec3 color =  ambient + Lo * (/*1-*/shadow);

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));
    FragColor = vec4(color, 1.0);
//    FragColor = vec4( ambient + Lo , 1.0);

//    FragColor = vec4(irradiance , 1.0);
//    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far to get depth in range [0,1] for visualization purposes
//    FragColor = vec4(vec3(depth), 1.0);
//    FragColor = vec4(shadow, shadow, shadow, 1.0);


}

//void main()
//{
//    vec3 color = texture(albedoMap, TexCoords).rgb;
//    vec3 normal = normalize(Normal);
//    vec3 lightColor = vec3(0.3);
//    // ambient
//    vec3 ambient = 0.3 * lightColor;
//    // diffuse
//    vec3 lightPos=light[3].xyz;
//    vec3 lightDir = normalize(lightPos - FragPos);
//    float diff = max(dot(lightDir, normal), 0.0);
//    vec3 diffuse = diff * lightColor;
//    // specular
//    vec3 viewDir = normalize(camPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = 0.0;
//    vec3 halfwayDir = normalize(lightDir + viewDir);
//    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
//    vec3 specular = spec * lightColor;
//    // calculate shadow
//    float shadow = ShadowCalculation(FragPosLightSpace);
//    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

//    FragColor = vec4(lighting, 1.0);
//}
