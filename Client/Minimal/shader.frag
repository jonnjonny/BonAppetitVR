#version 410 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 vertNormal;
in vec4 vertex;
in mat4 Modelview;

uniform int colorScheme;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{

vec3 objectColor = vec3(1,0,0);

//pt light
vec3 lightPos = vec3(0,3,0);
vec3 lightColor = vec3(1,1,1);


 float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

vec3 norm = normalize(vertNormal);
vec4 transfMyVertex = Modelview * vertex;
vec3 FragPos = transfMyVertex.xyz / transfMyVertex.z;
vec3 lightDir = normalize(lightPos - FragPos);  

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;




   if(colorScheme == 0) {
		objectColor = vec3(0.0,0.0,0.0);
	}
    else if(colorScheme == 1) {
		objectColor = vec3(248.0,131.0,121.0)/255.0;
	}
	else {
		objectColor = vec3(1.0,1.0,1.0);
	}


vec3 result = (ambient + diffuse) * objectColor;
fragColor = vec4(result, 1.0);
	 /*

	else if(colorScheme == 3) {
		objectColor = vec4(0.0,0.0,1.0,1.0);
	}
	else if(colorScheme == 4) {
		objectColor = vec4(1.0,1.0,1.0,1.0);
	}
	else if(colorScheme == 5) {
		objectColor = vec4(1.0,1.0,0.0,1.0);
	}
	else if(colorScheme == 6) {
		objectColor = vec4(0.0,1.0,1.0,0.0);
	}
	else {
		objectColor = vec4(1.0,0.0,1.0,1.0);
	}*/
}
