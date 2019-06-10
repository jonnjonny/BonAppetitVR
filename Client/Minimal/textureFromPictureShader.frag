#version 410 core

in vec3 vertNormal;
in vec4 vertex;
in mat4 Modelview;

in vec2 TexCoords;
out vec4 fragColor;
uniform sampler2D texFramebuffer;


void main()
{


vec4 objectColor = texture2D(texFramebuffer, TexCoords );

//pt light
vec3 lightPos = vec3(0,3,0);
vec4 lightColor = vec4(1,1,1,1);


 float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * lightColor;

vec3 norm = normalize(vertNormal);
vec4 transfMyVertex = Modelview * vertex;
vec3 FragPos = transfMyVertex.xyz / transfMyVertex.z;
vec3 lightDir = normalize(lightPos - FragPos);  

float diff = max(dot(norm, lightDir), 0.0);
vec4 diffuse = diff * lightColor;

vec4 result = (ambient + diffuse) * objectColor;

	fragColor = result;

}