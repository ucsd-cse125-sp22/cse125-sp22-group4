#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 worldNormal;
in vec3 worldPos;

uniform mat4 model;

// You can output many things.
// The first vec4 type output determines the color of the fragment
out vec4 fragColor;

uniform vec4 lightposn[2] = { { 0, 5, -10, 1 }, { 0, 5, 10, 1 } };  // positions of lights 
uniform vec4 lightcolor[2] = { { 0.6, 0.3, 0.0, 1  } , { 0, 0.3, 0.6, 1 } }; // colors of lights

//material parameters.
uniform vec4 ambient = { 0.4, 0.4, 0.4, 1 };
uniform vec4 diffuse = { 0.6, 0.6, 0.6, 1 };
uniform vec4 specular = { 1, 1, 1, 1 };
uniform vec4 emission = { 0.02, 0.02, 0.02, 1 };
uniform float shininess = 100;

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, 
					const in vec3 halfvec, const in vec4 mydiffuse, 
					const in vec4 myspecular, const in float myshininess)
{
        float nDotL = dot(normal, direction)  ;         
        vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;  

        float nDotH = dot(normal, halfvec) ; 
        vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ; 

        vec4 retval = lambert + phong ; 
        return retval ;            
}       


void main() {

	const vec3 eyepos = vec3(0,0,0);
	vec4 tempPos = model * vec4 (worldPos, 1.0);
	vec3 mypos = tempPos.xyz / tempPos.w ; // Dehomogenize current location 
    vec3 eyedirn = normalize(eyepos - mypos) ; 

	vec3 normal = worldNormal;

	vec4 col = vec4(0);
	vec3 direction;
	vec3 halfi;

	vec3 pointPos;
	vec3 pointDir;
	vec3 pointHal;

	for(int i = 0; i < 2; i ++)
	{
		//directional
		if(lightposn[i].w == 0)
		{
			direction = normalize(lightposn[i].xyz);
			halfi	  = normalize(direction + eyedirn);
			col		  = col + ComputeLight(direction, lightcolor[i], normal, halfi,
										diffuse, specular, shininess);

		}
		else	//point
		{
			pointPos = lightposn[i].xyz / lightposn[i].w;
			pointDir = normalize(pointPos - mypos);
			pointHal = normalize(pointDir + eyedirn);
			col = col + ComputeLight(pointDir, lightcolor[i], normal, pointHal, 
									diffuse, specular, shininess);
		}
	}

	fragColor = col + ambient + emission;
	//fragColor = vec4((normal + vec3(1))/2, 1);
}
