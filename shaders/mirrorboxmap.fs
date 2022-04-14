#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

out vec4 FragColor;

in vec3 ReflectionVector;
//in vec3 RefractionVector;
in vec3 Position;

//uniform vec3 cameraPos;
uniform vec3 viewPosCam;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(Position - viewPosCam);
    vec3 R = reflect(I, normalize(ReflectionVector));
    vec3 refraction =  refract(I, normalize(ReflectionVector), 1.0/1.332);
    vec3 refref = normalize(mix(R, refraction, 0.0));
    vec3 color = vec3(.1,1,.3);
//    vec3 frag =(texture(skybox, refraction).rgb)*0.66 + color + (texture(skybox, R).rgb)*0.23;
    vec3 fragRR =mix((texture(skybox, refraction).rgb),(texture(skybox, R).rgb),0.23);
    vec3 frag = mix(fragRR, color, 0.1);
    FragColor = vec4(frag.rgb, 1.0);
}
