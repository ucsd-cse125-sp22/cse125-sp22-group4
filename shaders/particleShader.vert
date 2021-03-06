#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 vertex;
// Uniform variables
uniform mat4 viewProj;
uniform mat4 model;
uniform vec3 offset;
uniform vec4 color;
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;

out vec2 TexCoords;
out vec4 ParticleColor;

void main() {

    TexCoords = vertex.xy + vec2(0.5, 0.5);
    ParticleColor = color;
    vec3 vertexPosition_worldspace = offset + CameraRight_worldspace * vertex.x + CameraUp_worldspace * vertex.y;
    gl_Position = viewProj * vec4(vertexPosition_worldspace, 1.0f);

    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    //gl_Position = viewProj * model * vec4(position, 1);
}