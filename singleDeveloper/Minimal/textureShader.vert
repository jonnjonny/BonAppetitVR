#version 410 core
// This is a sample fragment shader.


// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
uniform mat4 projection;
uniform mat4 modelview;

in vec3 position;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec2 textureCoord;

void main()
{
    gl_Position = projection * modelview * vec4(position, 1.0);
    textureCoord = vec2((atan(position.y, position.x) / 3.1415926 + 1.0) * 0.5,
    (asin(position.z) / 3.1415926 + 0.5));
}
