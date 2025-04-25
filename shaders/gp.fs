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

const float lightStrength   = 0.5;
const float ambientStrength = 0.5;
const float shadowBias      = -0.005;
const float specularStrength = 0.1;

const vec3 lightColor = vec3(1.0, 0.94, 0.88);
const float PI = 3.14159265359;

// === PBR Functions ===
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
    return GeometrySchlickGGX(max(dot(N, V), 0.0), roughness) *
           GeometrySchlickGGX(max(dot(N, L), 0.0), roughness);
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

    float bias = max(shadowBias * (1.0 - dot(N, L)), shadowBias * 0.1);
    float shadow = 0.0;
    float texelSize = 1.0 / 2048.0;
    int kernelSize = 2;

    for (int x = -kernelSize; x <= kernelSize; ++x) {
        for (int y = -kernelSize; y <= kernelSize; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            float closestDepth = texture(shadowMap, projCoords.xy + offset).r;
            if (projCoords.z - bias > closestDepth)
                shadow += 1.0;
        }
    }

    shadow /= float((2 * kernelSize + 1) * (2 * kernelSize + 1));
    return shadow;
}

void main() {
    float gamma = 2.2;

    vec3 albedo = pow(texture(albedoMap, uvFrag).rgb, vec3(gamma*gamma));
    float metallic = texture(metallicMap, uvFrag).r;
    float roughness = texture(roughnessMap, uvFrag).r;
    float ao = texture(aoMap, uvFrag).r;

    vec3 N = normalize(texture(normalMap, uvFrag).rgb * 2.0 - 1.0);
    vec3 V = normalize(TangentViewPos - TangentFragPos);
    vec3 L = normalize(TangentLightDir);
    vec3 H = normalize(V + L);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);
    float NdotL = max(dot(N, L), 0.0);

    vec3 specular = (NDF * G * F) / max(4.0 * max(dot(N, V), 0.0) * NdotL, 0.001);
    specular *= specularStrength;

    float shadow = ShadowCalculation(FragPosLightSpace, N, L);

    vec3 lightRadiance = lightColor * lightStrength;
    vec3 diffuse = kD * albedo / PI;
    vec3 direct = (diffuse + specular) * lightRadiance * NdotL * (1.0 - shadow);

    // IBL
    vec3 Nw = normalize(TBN * N);
    vec3 Vw = normalize(TBN * V);
    vec3 R = reflect(-Vw, Nw);
    vec3 env = textureLod(skyCube, R, roughness * 1.0).rgb;
    env = pow( env, vec3(gamma) );
    vec3 Fr = fresnelSchlickRoughness(max(dot(Nw, Vw), 0.0), F0, roughness);
    vec3 ibl = (1.0 - Fr) * env * albedo * ao * ambientStrength;

    vec3 color = direct + ibl;

    // Gamma correction + tone mapping
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / gamma));

    FragColor = vec4(color, 1.0);

}
