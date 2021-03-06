#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 worldNormal;
in vec3 worldPos;
in vec2 texCoords;
in mat3 TBN;

// You can output many things.
// The first vec4 type output determines the color of the fragment
out vec4 fragColor;

uniform vec3 eyePos;
uniform int lightCount;
uniform vec4 lightPosn[10];  // positions of lights
uniform vec4 lightColorn[10]; // colors of lights

uniform mat4 model;

//material parameters.
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emission;
uniform float shininess;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_ambient1;
uniform sampler2D texture_shininess1;

uniform int diffuseMode;
uniform int specularMode;
uniform int normalMode;
uniform int ambientMode;
uniform int shininessMode;

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal,
                    const in vec3 halfvec, const in vec4 mydiffuse,
                    const in vec4 myspecular, const in float myshininess) {
    float nDotL = dot(normal, direction);
    vec4 lambert = mydiffuse * lightcolor * max(nDotL, 0.0);

    float nDotH = dot(normal, halfvec);
    vec4 phong = myspecular * lightcolor * pow(max(nDotH, 0.0), myshininess) ;

    vec4 retval = lambert + phong;
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

    vec3 realNormal = vec3(0);
    vec4 realAmbient = vec4(0);
    vec4 realDiffuse = vec4(0);
    vec4 realSpecular = vec4(0);
    float realShininess = 0;
    
    vec4 ambientLight = vec4(0.1);
    vec4 normalTexture = texture(texture_normal1, texCoords);
    vec4 roughnessTexture = texture(texture_shininess1, texCoords);
    float roughnessGrey = (0.299 * roughnessTexture[0] + 0.587 * roughnessTexture[1] + 0.114 * 0.587 * roughnessTexture[2]);

    realNormal = normalMode == 1 ? normalize(TBN * (vec3(normalTexture) * 2.0 - 1.0)) : worldNormal;
    realAmbient = ambientMode == 1 ? texture(texture_ambient1, texCoords) * ambientLight : ambient * ambientLight;
    realDiffuse = diffuseMode == 1 ? texture(texture_diffuse1, texCoords) * diffuse : diffuse;
    realSpecular = specularMode == 1 ? texture(texture_specular1, texCoords) * specular : specular;
    realShininess = shininessMode == 1 ? roughnessGrey * shininess : shininess;

    for(int i = 0; i < lightCount; ++i) {
        //directional
        if(lightPosn[i].w == 0) {
            direction = normalize(lightPosn[i].xyz);
            halfi = normalize(direction + eyeDir);
            col = col + ComputeLight(direction, lightColorn[i], realNormal, halfi,
                                    realDiffuse, realSpecular, realShininess);

        }
        // point light
        else {
            pointPos = lightPosn[i].xyz / lightPosn[i].w;
            pointDir = normalize(pointPos - worldPos);
            pointHal = normalize(pointDir + eyeDir);
            col = col + ComputeLight(pointDir, lightColorn[i], realNormal, pointHal,
                                    realDiffuse, realSpecular, realShininess);
        }
    }

    fragColor = col + realAmbient + emission;
    //fragColor = vec4(realNormal + vec3(1) / 2, 1);
}
