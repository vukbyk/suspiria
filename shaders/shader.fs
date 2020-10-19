#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D albedoTexture;

in vec2 v_texcoord;

out vec4 FragColor;

void main()
{
    // Set fragment color from texture
    FragColor = texture(albedoTexture, vec2(v_texcoord.x, v_texcoord.y));
//    FragColor = vec4(.3,.1,0.5,0);
}

//#ifdef GL_ES
//// Set default precision to medium
//precision mediump int;
//precision mediump float;
//#endif

//uniform sampler2D texture;

//varying vec2 v_texcoord;

//void main()
//{
//    // Set fragment color from texture
//    gl_FragColor = texture2D(texture, v_texcoord);
//}
