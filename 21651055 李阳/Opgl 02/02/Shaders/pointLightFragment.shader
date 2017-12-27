#version 330 core

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D texture_diffuse1;

vec3 LightPos = vec3(0.0f, 0.0f, 0.0f);
float linear = 0.009f;
float quadratic = 0.0032f;

out vec4 color;

void main(){
	// 环境光
	float ambient = 0.1f;
	// 漫反射光
	vec3 lightDir = normalize(LightPos - FragPos);
	float diffuse = max(dot(lightDir, Normal), 0.0f);
	// 衰减
	float dis = distance(LightPos, FragPos);
	float attenuation = 1.0 / (1.0f + dis * linear + dis * dis * quadratic);

	vec4 tColor = texture(texture_diffuse1, TexCoords);
	tColor *= ((ambient + diffuse) * attenuation);

	color = tColor;
}