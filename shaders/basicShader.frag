#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 worldNormal;

uniform vec3 AmbientColor= vec3(0.2);
uniform vec3 LightDirection = normalize(vec3(0, 10, 10));
uniform vec3 LightColor = vec3(0.2, 0.2, 0.2);
uniform vec3 DiffuseColor = vec3(1, 1, 1);	
uniform vec3 LightDirection_2 = normalize(vec3(0, 10, 5));
uniform vec3 LightColor_2 = vec3(1.0, 1.0, 0.0);

// You can output many things.
// The first vec4 type output determines the color of the fragment
out vec4 fragColor;


void main() {
    vec3 irradiance = AmbientColor + LightColor * max(0, dot(LightDirection, worldNormal));
    irradiance += LightColor_2 * max(0, dot(LightDirection_2, worldNormal));

	// Diffuse reflectance
	vec3 reflectance = irradiance * DiffuseColor;

	// Gamma correction
	fragColor = vec4(sqrt(reflectance), 1);
}