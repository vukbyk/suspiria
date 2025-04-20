#ifdef GL_ES
// Set default precision to medium for OpenGL ES
precision highp int;
precision highp float;
#endif

out vec4 FragColor;

// Inputs from vertex shader
in vec3 FragPos;
in vec2 uvFrag;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec3 CamWorldPos;

in mat3 TBN;

// Material texture maps
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// Optional IBL maps (commented out for now)
// uniform samplerCube irradianceMap;
// uniform samplerCube prefilterMap;

// Sky reflection cubemap
uniform samplerCube skyCube;
uniform sampler2D brdfLUT; // BRDF lookup texture for specular IBL

const float PI = 3.14159265359;

// ----------------------------------------------------------------------------
// Trowbridge-Reitz GGX Normal Distribution Function (NDF)
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

// ----------------------------------------------------------------------------
// Schlick-GGX geometry term (single direction)
float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

// ----------------------------------------------------------------------------
// Smith's method for geometric attenuation (combined view and light directions)
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

// ----------------------------------------------------------------------------
// Fresnel-Schlick approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// ----------------------------------------------------------------------------
// Fresnel-Schlick with roughness (for ambient specular)
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) *
                pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
    float gamma = 2.2;
    float ambientStrength=0.5;
    // Sample material textures
    vec3 albedo     = pow(texture(albedoMap, uvFrag).rgb, vec3(gamma) ); // linearize
    float metallic  = texture(metallicMap, uvFrag).r;
    float roughness = texture(roughnessMap, uvFrag).r;
    float ao        = texture(aoMap, uvFrag).r;

    // Hardcoded bright white light
    vec3 lightColor = vec3(1.0, 1.0, 1.0) * 1.0;

    // Compute view and light direction vectors in tangent space
    float dist      = length(TangentFragPos - TangentLightPos);

    // Compute normal from normal map in world space
    vec3 normalMapValue = texture(normalMap, uvFrag).rgb;
    vec3 N = normalize(TBN * (normalMapValue * 2.0 - 1.0));

    // View and light directions in world space
    vec3 V = normalize(CamWorldPos - FragPos);
    vec3 L = normalize(TBN * normalize(TangentLightPos - TangentFragPos));


    // Base reflectivity at normal incidence (plastic = 0.04, metal = albedo)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // Initialize final lighting accumulator
    vec3 Lo = vec3(0.0);

    {
        // Compute halfway vector
        // vec3 L = lightDir;
        vec3 H = normalize(V + L);

        // float attenuation = 1.0 / (dist * dist);
        float attenuation = 1.0 / (dist * dist);
        vec3 radiance = lightColor * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;

        // Fresnel term also determines diffuse strength (kD)
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);

        // Add lighting contribution
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    // === Ambient lighting (IBL) ===

    vec3 Frgh = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kD = 1.0 - Frgh;
    // vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
    // vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    // V = normalize(CamWorldPos - FragPos);
    vec3 R = reflect(-V, N);

    // vec3 irradiance = pow(texture(skyCube, R).rgb, vec3(gamma));
    vec3 irradiance = pow(textureLod(skyCube, R, roughness * 8.0).rgb, vec3(gamma));
    vec3 diffuse = irradiance * albedo;

    // === Optional Specular IBL (Split-Sum Approximation) ===

    // const float MAX_REFLECTION_LOD = 4.0;
    // vec3 prefilteredColor = textureLod(/*prefilterMap*/skyCube, R, roughness * MAX_REFLECTION_LOD).rgb;
    // vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    // vec3 specular = prefilteredColor * (Frgh * 1.0); // simplified; normally * (brdf.x + brdf.y)

    // Final ambient contribution (diffuse only for now)
    // vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 ambient = (kD * diffuse) * ao;

    // Combine all
    vec3 color = ambient * ambientStrength + Lo;
    // HDR tonemapping
    color = color / (color + vec3(1.0)); // Reinhard

    // float saturation = 1.15;
    // float gray = dot(color, vec3(0.2126, 0.7152, 0.0722)); // luminance
    // color = mix(vec3(gray), color, saturation);
    // Gamma correction
    color = pow(color, vec3(1.0 / gamma));

    FragColor = vec4(color, 1.0);
    // This will let you see the reflected direction as RGB color — helpful to verify it's covering the whole 3D sphere.
    // FragColor = vec4(normalize(R) * 0.5 + 0.5, 1.0);

}
