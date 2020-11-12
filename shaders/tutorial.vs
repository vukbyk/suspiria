#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 nor;
layout (location = 3) in vec3 tng;
layout (location = 4) in vec3 bit;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light;
//uniform vec3 viewPos;


//uniform mat4 transforms[3];
//uniform uint index;
//out VSO {
//    vec3 outFragPos;
//    vec2 outuvFrag;
//    vec3 outTangentLightPos;
//    vec3 outTangentViewPos;
//    vec3 outTangentFragPos;
//} vso;

out vec3 FragPos;
out vec2 uvFrag;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

//out vec3 FragPos;
//out vec2 uvFrag;

out vec3 nrm;
out vec3 tg;
out vec3 bi;

out vec3 lightPosition;
out vec3 viewPosition;
out vec3 normalTest;

void main()
{
    normalTest = nor; //To DELETE


    lightPosition = vec3(light[3].xyz);
    viewPosition = -1.0 * vec3(view[3].xyz);
//    vec3 viewPosition = vec3(view[3].xyz);//??? maybe have to send real data????

    FragPos = vec3(model * vec4(pos, 1.0));
    uvFrag=uv;
//    nrm=nor;
//    tg=tng;
//    bi=bit;

    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 T = normalize(normalMatrix * tng);
    vec3 N = normalize(normalMatrix * nor);
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(normalMatrix * bit);

//    B = normalize(B - dot(B, T) * T);
//    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));

    TangentLightPos = TBN * lightPosition;
    TangentViewPos  = TBN * viewPosition;
    TangentFragPos  = TBN * FragPos;

    gl_Position = projection * view * model * vec4(pos, 1.0);
}
