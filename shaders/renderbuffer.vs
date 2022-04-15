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
uniform vec3 viewPosCam;

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


out vec3 nrm;
out vec3 tg;
out vec3 bi;
out mat3 TBN;

out vec3 lightPosition;
out mat4 lightMat;

out vec3 ReflectionVector;
out vec3 BoxPosition;

void main()
{
    lightMat = light;
    lightPosition = light[3].xyz;
//    vec3 viewPosCam = vec3(view[3].xyz);
//    viewPosition = vec3(view[3].xyz);//??? maybe have to send real data????

    //Probabli can be used directlyviewPosCam
//    vec3 viewPosition = viewPosCam;

    FragPos = vec3(model * vec4(pos, 1.0));
    uvFrag=uv;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * tng);
    vec3 N = normalize(normalMatrix * nor);
    T = normalize(T - dot(T, N) * N);
//    vec3 B = normalize(normalMatrix * bit);//who knows why and where from
    vec3 B = cross(N, T);//from tutorial

//    B = normalize(B - dot(B, T) * T);
//    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);
    mat3 inverseTBN = transpose(TBN); //in special cases you can use transponse
                                      //to inverse (cheaper option)

    TangentLightPos = inverseTBN * lightPosition;
    TangentViewPos  = inverseTBN * viewPosCam;
    TangentFragPos  = inverseTBN * FragPos;

    ReflectionVector = N;//mat3(transpose(inverse(model))) * nor;
    BoxPosition = vec3(model * vec4(pos, 1.0));


    gl_Position = projection * view * model * vec4(pos, 1.0);
}


//void main()
//{
//    vec3 lightPos = light[3].xyz;//vec3(0,0,5);//
//    gl_Position = projection * view * model * vec4(pos, 1.0);
//    FragPos = vec3(view * model * vec4(pos, 1.0));
//    normalTest = mat3(transpose(inverse(view * model))) * nor;
//    lightPosition = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
//}
