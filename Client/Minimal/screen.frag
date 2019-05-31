#version 410 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec2 TexCoords;

uniform sampler2D screenTexture;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec3 color;


void main()
{
	// color = vec3(TexCoords.x, TexCoords.y, 1); // texture(screenTexture, TexCoords).xyz;
	color = texture(screenTexture, TexCoords).xyz;

}
