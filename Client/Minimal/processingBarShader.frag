#version 410 core

in vec2 TexCoords;
out vec4 fragColor;
uniform float percentDone;


void main()
{
	if( TexCoords.x <= percentDone) {
		fragColor =  vec4( 1,1, 0, 1 );
	}
	else { 
		fragColor =  vec4( 1,1,1,  1 );
	}
	//fragColor =  vec4(TexCoords.x, TexCoords.y,1,1);
}