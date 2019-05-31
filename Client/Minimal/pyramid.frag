#version 410 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

uniform int linecolor;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec3 color;


void main()
{
	if(linecolor == 0) {
		
		color = vec3(1.0,0.0,0.0);
	
	}
	else {
		color = vec3(0.0,1.0,0.0);
	}

}
