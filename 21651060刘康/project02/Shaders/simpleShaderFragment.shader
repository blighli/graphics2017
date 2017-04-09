#version 330 core

uniform sampler2D texture_diffuse1;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

void main()
{
	color = vec4((texture(texture_diffuse1, TexCoords)));
}