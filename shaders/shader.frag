#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 worldNormal;
in vec3 worldPos;

// You can output many things.
// The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main() {
    fragColor = vec4((worldNormal + vec3(1))/2, 1);
}
