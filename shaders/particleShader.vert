#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec4 vertex;
// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;
uniform vec3 offset;
uniform vec4 color;

out vec2 TexCoords;
out vec4 ParticleColor;

void main() {


    float scale = 1.0f;
    TexCoords = vertex.zw;
    ParticleColor = color;
    gl_Position = viewProj * vec4(vec3((vertex.xy * scale), 0.0) + offset, 1.0);

    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    //gl_Position = viewProj * model * vec4(position, 1);
}