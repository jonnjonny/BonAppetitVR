#version 410 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 vertNormal;
in vec4 vertex;
in mat4 Modelview;

uniform int colorScheme;
uniform mat4 view;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{

	vec4 objectColor = vec4(1,0,0,1);
	if(colorScheme == 0) {
		objectColor = vec4(0.0,0.0,0.0,1.0);
	}
	else if(colorScheme == 1) {
		objectColor = vec4(248.0,131.0,121.0,255.0)/255.0;
	}
	else {
		objectColor = vec4(1.0,1.0,1.0,1.0);
	}

	vec4 lightPos_h = vec4(0,5,0,1);
	vec4 trans_light_h = view * lightPos_h;
	vec3 trans_light = trans_light_h.xyz/trans_light_h.w;
	vec4 lightColor = vec4(1,1,1,1);
	vec4 myDiffuse = vec4(0.8,0.8,0.8,1);
	vec4 mySpecular = vec4(0.5,0.5,0.5,1);
	float myShininess = 20;

	 float ambientStrength = 0.3;
		vec4 ambient = vec4( ambientStrength * vec3(lightColor.x,lightColor.y,lightColor.z),1.0);

	vec3 trans_norm = normalize(mat3(transpose(inverse(Modelview)))*vertNormal);
	vec4 transfMyVertex = Modelview * vertex;


	vec3 eyePosition = vec3(0,0,0);
	vec3 vertex_trans = transfMyVertex.xyz / transfMyVertex.w;
	vec3 eyeDirection = normalize(eyePosition - vertex_trans); 
	vec3 lightDirection = normalize(trans_light-vertex_trans);

	float nDotL = dot(trans_norm,lightDirection);
	vec4 lambert = myDiffuse* max(nDotL, 0.0) * lightColor;

	vec3 halfvec = normalize(lightDirection + eyeDirection);

	float nDotH = dot(trans_norm, halfvec);
	vec4 phong = mySpecular * lightColor* pow(max(nDotH, 0.0), myShininess);


	vec4 result = (ambient + lambert + phong)*objectColor;


	fragColor = result;

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
