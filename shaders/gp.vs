#ifdef GL_ES
precision highp int;
precision highp float;
#endif

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 nor;
layout(location = 3) in vec3 tng;
layout(location = 4) in vec3 bit;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light;
uniform mat4 lightSpaceMat;

out vec2 uvFrag;
out vec3 TangentLightPos;
out vec3 TangentLightDir;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec3 CamWorldPos;
out mat3 TBN;
out vec3 FragPos;
out vec4 FragPosLightSpace;

void main()
{
    vec3 FragWorldPos = vec3(model * vec4(pos, 1.0));
    FragPos = FragWorldPos;
    uvFrag = uv;

    CamWorldPos = vec3(inverse(view)[3]);

    // TBN matrix
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * tng);
    vec3 N = normalize(normalMatrix * nor);
    vec3 B = normalize(normalMatrix * bit);
    T = normalize(T - dot(T, N) * N);
    B = normalize(B - dot(B, T) * T);
    TBN = mat3(T, B, N);

    // Inverse TBN
    mat3 invTBN = transpose(TBN);
    vec3 lightDir = normalize(light[2].xyz);
    vec3 lightPos = light[3].xyz;

    TangentLightDir = invTBN * lightDir;
    TangentLightPos = invTBN * lightPos;
    TangentViewPos  = invTBN * CamWorldPos;
    TangentFragPos  = invTBN * FragWorldPos;

    FragPosLightSpace = lightSpaceMat * vec4(FragWorldPos, 1.0);
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
