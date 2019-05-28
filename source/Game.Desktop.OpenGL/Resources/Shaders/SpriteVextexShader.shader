#version 150 core
in vec3 position;
in vec4 color;
in vec2 texcoord;
out vec4 Color;
out vec2 Texcoord;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

void main()
{
	Color = color;
	Texcoord = texcoord;
	gl_Position = proj * view * model* vec4(position, 1.0);
}