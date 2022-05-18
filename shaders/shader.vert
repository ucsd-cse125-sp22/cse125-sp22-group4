#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 biTangent;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform int hasBones;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something.
out vec3 worldNormal;
out vec3 worldPos;
out vec2 texCoords;
out mat3 TBN;

void main() {
    vec4 totalPosition = vec4(0.0f);
    // ??
    vec3 totalNormal = vec3(0.0f);

    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
        if(boneIds[i] == -1) {
            continue;
        }
        if(boneIds[i] >= MAX_BONES) 
        {
            totalPosition = vec4(position,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(position,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal;
        totalNormal += localNormal * weights[i];
    }

    mat4 BoneTransform  = finalBonesMatrices[boneIds[0]] * weights[0];
    BoneTransform += finalBonesMatrices[boneIds[1]] * weights[1];
    BoneTransform += finalBonesMatrices[boneIds[2]] * weights[2];
    BoneTransform += finalBonesMatrices[boneIds[3]] * weights[3];

    if (hasBones == 0) {
        vec3 T = normalize(vec3(model * vec4(tangent, 0)));
        vec3 B = normalize(vec3(model * vec4(biTangent, 0)));
        vec3 N = normalize(vec3(model * vec4(normal, 0)));
        TBN = mat3(T, B, N);

        worldNormal = normalize(vec3(inverse(transpose(model)) * vec4(normal, 0)));
        worldPos = vec3(model * vec4(position, 1));
        texCoords = aTexCoords; 
        gl_Position = viewProj * model * vec4(position, 1.0f);
    } else {
        mat3 normalMatrix = transpose(inverse(mat3(BoneTransform)));
        vec3 T = normalize(normalMatrix * vec3(model * vec4(tangent, 0)));
        vec3 B = normalize(normalMatrix * vec3(model * vec4(biTangent, 0)));
        vec3 N = normalize(normalMatrix * vec3(model * vec4(normal, 0)));
        TBN = mat3(T, B, N);

        worldNormal = normalize(vec3(inverse(transpose(model)) * vec4(N, 0)));
        worldPos = vec3(model * totalPosition);
        texCoords = aTexCoords; 
        gl_Position = viewProj * model * totalPosition;
    }
}
