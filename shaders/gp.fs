#ifdef GL_ES
precision highp int;
precision highp float;
#endif

out vec4 FragColor;

in vec3 FragPos;
in vec2 uvFrag;
in vec3 TangentLightPos;
in vec3 TangentLightDir;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec3 CamWorldPos;
in mat3 TBN;
in vec4 FragPosLightSpace;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform samplerCube skyCube;
uniform sampler2D brdfLUT;
uniform sampler2D shadowMap;

// === Local Lighting Constants ===
const vec3  lightColor        = vec3(1.0, 0.94, 0.88);
const float lightStrength     = 0.5;
const float ambientStrength   = 1.0;
const float specularStrength  = 0.0;  // set to 0 to disable direct specular
const float shadowBias        = 0.001;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float k = pow(roughness + 1.0, 2.0) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float ggx1 = GeometrySchlickGGX(max(dot(N, V), 0.0), roughness);
    float ggx2 = GeometrySchlickGGX(max(dot(N, L), 0.0), roughness);
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) *
           pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 N, vec3 L) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if (projCoords.z > 1.0) return 0.0;

    float currentDepth = projCoords.z;
    float bias = max(shadowBias * (1.0 - dot(N, L)), shadowBias * 0.1);
    float shadow = 0.0;
    float texelSize = 1.0 / 2048.0;
    int kernelSize = 2;

    for (int x = -kernelSize; x <= kernelSize; ++x) {
        for (int y = -kernelSize; y <= kernelSize; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            float closestDepth = texture(shadowMap, projCoords.xy + offset).r;
            if (currentDepth - bias > closestDepth)
                shadow += 1.0;
        }
    }

    shadow /= float((2 * kernelSize + 1) * (2 * kernelSize + 1));
    return shadow;
}

void main() {
    float gamma = 2.2;

    // Albedo from texture, gamma corrected
    vec3 albedo = pow(texture(albedoMap, uvFrag).rgb, vec3(gamma));

    // Metallic and roughness from textures
    float metallic = texture(metallicMap, uvFrag).r;
    float roughness = texture(roughnessMap, uvFrag).r;

    // Ambient occlusion from texture
    float ao = texture(aoMap, uvFrag).r;

    // Normal from normal map, adjusted for TBN matrix
    vec3 N = normalize(texture(normalMap, uvFrag).rgb * 2.0 - 1.0);

    // View and light vectors in tangent space
    vec3 V = normalize(TangentViewPos - TangentFragPos);
    vec3 L = normalize(TangentLightDir);
    vec3 H = normalize(V + L);

    // F0 for Fresnel term, adjusted by metallic value
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    // Normal distribution function (NDF)
    float NDF = DistributionGGX(N, H, roughness);

    // Geometry function for Smith's method
    float G = GeometrySmith(N, V, L, roughness);

    // Fresnel term using Schlick approximation
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    // Specular energy conservation
    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);

    // Compute specular only if enabled
    vec3 specular = (NDF * G * F) / max(4.0 * max(dot(N, V), 0.0) * NdotL, 0.001);
    specular *= specularStrength;

    // Shadow calculation
    float shadow = ShadowCalculation(FragPosLightSpace, N, L);

    // === Light tint fix for metal objects ===
    vec3 effectiveLightColor = mix(lightColor, vec3(1.0), metallic);

    // === Direct light fix for metal objects ===
    vec3 directLighting = vec3(0.0);

    // Only apply direct lighting if not fully metallic or specular is allowed
    if (metallic < 0.99 || specularStrength > 0.0) {
        vec3 diffuse = kD * albedo / PI;
        directLighting = (diffuse + specular) * effectiveLightColor * lightStrength * NdotL * (1.0 - shadow);
    }

    // === IBL reflection ===
    vec3 Nw = normalize(TBN * N);
    vec3 Vw = normalize(TBN * V);
    vec3 R = reflect(-Vw, Nw);
    vec3 irradiance = pow(textureLod(skyCube, R, roughness * 8.0).rgb, vec3(gamma));
    vec3 Fr = fresnelSchlickRoughness(max(dot(Nw, Vw), 0.0), F0, roughness);
    vec3 ambient = (1.0 - Fr) * irradiance * albedo * ambientStrength * ao;

    // Final color composition
    vec3 finalColor = directLighting + ambient;
    finalColor = pow(finalColor, vec3(1.0 / gamma));

    FragColor = vec4(finalColor, 1.0);
}
