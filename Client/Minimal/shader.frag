#version 410 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 vertNormal;

uniform int colorScheme;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    if(colorScheme == 0) {
		fragColor = vec4(0.0,0.0,0.0,1.0);
	}
    else if(colorScheme == 1) {
		fragColor = vec4(248.0,131.0,121.0,255.0)/255.0;
	}
	else {
		fragColor = vec4(1.0,1.0,1.0,1.0);
	}

	/*else if(colorScheme == 3) {
		fragColor = vec4(0.0,0.0,1.0,1.0);
	}
	else if(colorScheme == 4) {
		fragColor = vec4(1.0,1.0,1.0,1.0);
	}
	else if(colorScheme == 5) {
		fragColor = vec4(1.0,1.0,0.0,1.0);
	}
	else if(colorScheme == 6) {
		fragColor = vec4(0.0,1.0,1.0,0.0);
	}
	else {
		fragColor = vec4(1.0,0.0,1.0,1.0);
	}*/
}
