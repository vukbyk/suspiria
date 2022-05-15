#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
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
//!!!!!!!!!!!!!!!!CheckChange!!!!!!!!!!!!!!
//uniform vec3 camPos;
vec3 camPos=projection[3].xyz;

//uniform mat4 transforms[3];
//uniform uint index;
//out VSO {
//    vec3 outFragPos;
//    vec2 outuvFrag;
//    vec3 outTangentLightPos;
//    vec3 outTangentViewPos;
//    vec3 outTangentFragPos;
//} vso;

//out vec3 FragPos;
out vec2 uvFrag;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;


//out vec3 nrm;
//out vec3 tg;
//out vec3 bi;
//out mat3 TBN;
out mat3 inverseTBN;

out vec3 lightPosition;
out mat4 lightMat;

//out vec3 ReflectionVector;
//out vec3 BoxPosition;

//out vec3 NormalTutorial;
//out vec3 Position;

void main()
{
    uvFrag=uv;
    lightMat = light;
    lightPosition = light[3].xyz;
//    vec3 camPos = vec3(view[3].xyz);
//    viewPosition = vec3(view[3].xyz);//??? maybe have to send real data????
    //Probabli can be used directlycamPos
//    vec3 viewPosition = camPos;

//    FragPos = vec3(model * vec4(pos, 1.0));
    vec3 FragPos= vec3(model * vec4(pos, 1.0));

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * tng);
    vec3 N = normalize(normalMatrix * nor);
    vec3 B = normalize(normalMatrix * bit);//Maybe this insted of calculating who knows why and where from
    T = normalize(T - dot(T, N) * N);
//    vec3 B = cross(N, T);//from tutorial but somtimes you get mirrored nor tng bitng

    B = normalize(B - dot(B, T) * T);

    mat3 TBN = mat3(T, B, N);
    inverseTBN = transpose(TBN); //in special cases you can use transponse
                                      //to inverse (cheaper option)

    TangentLightPos = inverseTBN * lightPosition;
    TangentViewPos  = inverseTBN * camPos;
    TangentFragPos  = inverseTBN * FragPos;

//    ReflectionVector = mat3(transpose(inverse(model))) * nor;//N;
//    BoxPosition = vec3(model * vec4(pos, 1.0));
//    NormalTutorial = /*N.xyz;*/mat3(transpose(inverse(model))) * nor;
//    Position = vec3(model * vec4(pos, 1.0));


    gl_Position = projection * view * model * vec4(pos, 1.0);
}
