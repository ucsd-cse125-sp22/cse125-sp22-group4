#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout(location = 2) in vec2 aTexCoords;

// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;
uniform vec3 offset;
uniform vec4 color;

out vec2 TexCoords;
out vec4 ParticleColor;

void main() {


    float scale = 10.0f;
    TexCoords = aTexCoords;
    ParticleColor = color;
    gl_Position = viewProj * model * vec4((position * scale) + offset, 1.0);

    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    //gl_Position = viewProj * model * vec4(position, 1);
}