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
vec3 lightPos = vec3(2,1.5,-3);
vec4 lightColor = vec4(1,1,1,1);
vec4 myDiffuse = vec4(0.8,0.8,0.8,1);
vec4 mySpecular = vec4(0.2,0.2,0.2,1);
float myShininess = 20;

 float ambientStrength = 0.8;
    vec4 ambient = vec4( ambientStrength * vec3(lightColor.x,lightColor.y,lightColor.z),1.0);

vec3 norm = normalize(vertNormal);
vec4 transfMyVertex = Modelview * vertex;



vec3 FragPos = transfMyVertex.xyz / transfMyVertex.z;
vec3 lightDir = normalize(lightPos - FragPos);  


vec4 lambert = myDiffuse* max(dot(norm, lightDir), 0.0) * lightColor;

vec3 halfvec = normalize(lightDir + vec3(1,0,0));
float nDotH = dot(norm, halfvec);
vec4 phong = mySpecular * lightColor;// * pow(max(nDotH, 0.0), myShininess);



float diff = max(dot(norm, lightDir), 0.0);
vec4 diffuse = diff * lightColor;

vec4 
 result = (ambient + diffuse ) * objectColor;


	fragColor = result;

}