#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 worldNormal;
in vec3 worldPos;

// You can output many things.
// The first vec4 type output determines the color of the fragment
out vec4 fragColor;

uniform vec3 eyePos;

uniform int lightCount;
uniform vec4 lightPosn[10];  // positions of lights
uniform vec4 lightColorn[10]; // colors of lights

//material parameters.
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emission;
uniform float shininess;

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal,
                    const in vec3 halfvec, const in vec4 mydiffuse,
                    const in vec4 myspecular, const in float myshininess) {
    float nDotL = dot(normal, direction);
    vec4 lambert = mydiffuse * lightcolor * max(nDotL, 0.0);

    float nDotH = dot(normal, halfvec);
    vec4 phong = myspecular * lightcolor * pow(max(nDotH, 0.0), myshininess) ;

    vec4 retval = lambert + phong ;
    return retval ;
}


void main() {
    vec3 eyeDir = normalize(eyePos - worldPos) ;

    vec4 col = vec4(0);
    vec3 direction;
    vec3 halfi;

    vec3 pointPos;
    vec3 pointDir;
    vec3 pointHal;

    for(int i = 0; i < lightCount; ++i) {
        //directional
        if(lightPosn[i].w == 0) {
            direction = normalize(lightPosn[i].xyz);
            halfi = normalize(direction + eyeDir);
            col = col + ComputeLight(direction, lightColorn[i], worldNormal, halfi,
                                    diffuse, specular, shininess);

        }
        // point light
        else {
            pointPos = lightPosn[i].xyz / lightPosn[i].w;
            pointDir = normalize(pointPos - worldPos);
            pointHal = normalize(pointDir + eyeDir);
            col = col + ComputeLight(pointDir, lightColorn[i], worldNormal, pointHal,
                                    diffuse, specular, shininess);
        }
    }

    fragColor = col + ambient + emission;
}
