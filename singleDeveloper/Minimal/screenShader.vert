#version 330 core
layout (location = 0) in vec3 position;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 modelview;

void main()
{
    gl_Position =   projection * modelview * vec4(position, 1.0);  
    TexCoords = vec2(position);
}
