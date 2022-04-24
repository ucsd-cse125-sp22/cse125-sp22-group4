#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;

out vec3 TexCoords;

// Uniform variables
uniform mat4 viewProj;

void main() {
    TexCoords = position;
    vec4 pos = viewProj * vec4(position, 1.0);
    gl_Position = pos.xyww;
}