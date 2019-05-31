#version 410 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 vertNormal;

uniform int color;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;




void main()
{
	if(color == 0) {
		fragColor = vec4(1.0,0.0,0.0,1.0);
	}
	else {
		fragColor = vec4(0.0,1.0,0.0,1.0);
	}
}
