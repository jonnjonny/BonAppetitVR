#version 410 core

in vec2 TexCoords;
out vec4 fragColor;
uniform sampler2D texFramebuffer;
void main()
{
	fragColor =  texture2D(texFramebuffer, TexCoords );
	//fragColor =  vec4(TexCoords.x, TexCoords.y,1,1);
}