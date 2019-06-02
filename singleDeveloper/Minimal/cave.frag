#version 330 core
in vec2 TexCoords;
out vec4 fragColor;
uniform sampler2D texFramebuffer;
uniform float dimmingFactor;
void main()
{
    fragColor = (vec4(1.0f) * dimmingFactor) * texture(texFramebuffer, TexCoords );
	//fragColor = vec4(TexCoords, 1, 1);
}