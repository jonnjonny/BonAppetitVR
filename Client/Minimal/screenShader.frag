#version 330 core
in vec2 TexCoords;
out vec4 fragColor;
uniform sampler2D texFb;
void main()
{
    fragColor = texture(texFb, TexCoords );
	//fragColor = vec4(TexCoords, 1, 1);
}