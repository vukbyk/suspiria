#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

in layout (location = 0) vec3 pos;
in layout (location = 1) vec2 uv;
in layout (location = 2) vec3 nor;
in layout (location = 3) vec3 tng;
in layout (location = 4) vec3 bit;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light;
//uniform vec3 viewPos;


//uniform mat4 transforms[3];
//uniform uint index;
out VSO {
    vec3 FragPos;
    vec2 uvFrag;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vso;

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
    viewPosition = -1*vec3(view[3].xyz);
//    vec3 viewPosition = vec3(view[3].xyz);//??? maybe have to send real data????

    vso.FragPos = vec3(model * vec4(pos, 1.0));
    vso.uvFrag=uv;
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

    vso.TangentLightPos = TBN * lightPosition;
    vso.TangentViewPos  = TBN * viewPosition;
    vso.TangentFragPos  = TBN * vso.FragPos;

    gl_Position = projection * view * model * vec4(pos, 1.0);
}
