#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif

//out vec4 FragColor;
//float near = 0.001;
//float far = 10.0;
//float LinearizeDepth(float depth)
//{
//    float z = depth * 2.0 - 1.0; // back to NDC
//    return (2.0 * near * far) / (far + near - z * (far - near));
////    return (camPos.z-near) / (far-near);
//}

void main()
{
//From tutorial is under comment?
    // gl_FragDepth = gl_FragCoord.z;
//Vuk:
//    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far to get depth in range [0,1] for visualization purposes
//    FragColor = vec4(vec3(depth), 1.0);
}
