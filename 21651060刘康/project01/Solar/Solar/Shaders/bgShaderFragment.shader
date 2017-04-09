#version 330 core

in vec2 TexCoords;
uniform sampler2D diffuse;

out vec4 color;

void main()
{
	color = vec4(texture(diffuse, TexCoords));
}