#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
uniform mat4 model;

void main()
{
	gl_Position = model * vec4(position, 1.0f);
	TexCoords = texCoords;
	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = normalize(mat3(transpose(inverse(model))) * normal);
}