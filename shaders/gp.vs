#ifdef GL_ES
// Set default precision for OpenGL ES
precision highp int;
precision highp float;
#endif

// Vertex attributes
layout (location = 0) in vec3 pos;   // Vertex position
layout (location = 1) in vec2 uv;    // Texture coordinates
layout (location = 2) in vec3 nor;   // Normal vector
layout (location = 3) in vec3 tng;   // Tangent vector
layout (location = 4) in vec3 bit;   // Bitangent vector

// Uniform matrices
uniform mat4 model;       // Model matrix
uniform mat4 view;        // View matrix
uniform mat4 projection;  // Projection matrix
uniform mat4 light;       // Light transformation matrix

// Derive camera position from the projection matrix's fourth column
// Note: This approach is unconventional; typically, the camera position is passed as a separate uniform
// vec3 camPos = projection[3].xyz;
vec3 camPos = vec3(inverse(view)[3]);

// Outputs to the fragment shader
out vec2 uvFrag;               // Interpolated texture coordinates
out vec3 TangentLightPos;      // Light position in tangent space
out vec3 TangentViewPos;       // View (camera) position in tangent space
out vec3 TangentFragPos;       // Fragment position in tangent space
out vec3 lightPosition;        // Light position in world space
out mat4 lightMat;             // Light transformation matrix
out mat3 TBN;                  // TBN matrix for transforming vectors from tangent to world space
out vec3 FragPos;              // World-space fragment position for IBL reflection
out vec3 CamWorldPos;


void main()
{
    CamWorldPos = camPos;
    // Pass through texture coordinates
    uvFrag = uv;

    // Store light matrix and extract light position from its fourth column
    lightMat = light;
    lightPosition = light[3].xyz;

    // Compute fragment position in world space
    vec3 FragWorldPos = vec3(model * vec4(pos, 1.0));
    FragPos = FragWorldPos;

    // Compute normal matrix (transpose of the inverse of the model matrix)
    mat3 normalMatrix = transpose(inverse(mat3(model)));

    // Transform tangent, normal, and bitangent vectors to world space
    vec3 T = normalize(normalMatrix * tng);
    vec3 N = normalize(normalMatrix * nor);
    vec3 B = normalize(normalMatrix * bit);

    // Orthogonalize tangent and bitangent vectors
    T = normalize(T - dot(T, N) * N);
    B = normalize(B - dot(B, T) * T);

    // Construct TBN matrix
    TBN = mat3(T, B, N);

    // Compute inverse TBN matrix (transpose of TBN since it's orthonormal)
    mat3 inverseTBN = transpose(TBN);

    // Transform light position, view position, and fragment position to tangent space
    TangentLightPos = inverseTBN * lightPosition;
    TangentViewPos  = inverseTBN * camPos;
    TangentFragPos  = inverseTBN * FragWorldPos;

    // Compute final vertex position in clip space
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
