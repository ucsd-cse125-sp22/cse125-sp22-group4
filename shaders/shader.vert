#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 biTangent;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something.
out vec3 worldNormal;
out vec3 worldPos;
out vec2 texCoords;
out mat3 TBN;

void main() {
    gl_Position = viewProj * model * vec4(position, 1.0f);

    // for shading
    worldNormal = normalize(vec3(inverse(transpose(model)) * vec4(normal, 0)));
    worldPos = vec3(model * vec4(position, 1));
    texCoords = aTexCoords;

    vec3 T = normalize(vec3(model * vec4(tangent, 0)));
    vec3 B = normalize(vec3(model * vec4(biTangent, 0)));
    vec3 N = normalize(vec3(model * vec4(normal, 0)));
    TBN = mat3(T, B, N);
}
