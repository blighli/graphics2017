#version 430 core

const int SUN = 0;
const int PLANET = 1;
const int SKY = 2;
const int CONE = 3;
const int MARS = 4;

layout(location=0) in vec4 sunCoords;
layout(location=1) in vec3 sunNormal;
layout(location=2) in vec2 sunTexCoords;
layout(location=3) in vec4 earthCoords;
layout(location=4) in vec3 earthNormal;
layout(location=5) in vec2 earthTexCoords;
layout(location=6) in vec4 skyCoords;
layout(location=7) in vec3 skyNormal;
layout(location=8) in vec2 skyTexCoords;
layout(location=9) in vec4 coneCoords;
layout(location=10) in vec3 coneNormal;
layout(location=11) in vec2 coneTexCoords;
layout(location=12) in vec4 marsCoords;
layout(location=13) in vec3 marsNormal;
layout(location=14) in vec2 marsTexCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform mat3 normalMat;
uniform uint object;

out vec4 frontAmbDiffExport, frontSpecExport;
out vec2 texCoordsExport;

struct Light
{
    vec4 lightAmbience;
    vec4 lightDiffuse;
    vec4 lightSpecular;
    vec4 coords;
};
uniform Light light;

uniform vec4 globAmb;

struct Material
{
    vec4 matAmbience;
    vec4 matDiffuse;
    vec4 matSpecular;
    vec4 matEmittance;
    float matShininess;
};
uniform Material sunMaterial;
uniform Material planetMaterial;

Material mat;

vec3 normal, lightDirection, eyeDirection, halfway;
vec4 emittance, ambience, diffuse, specular;
vec4 coords;

void main(void)
{
    if (object == SUN)
    {
        coords = sunCoords;
        normal = sunNormal;
        texCoordsExport = sunTexCoords;
        mat.matAmbience = sunMaterial.matAmbience;
        mat.matDiffuse = sunMaterial.matDiffuse;
        mat.matSpecular = sunMaterial.matSpecular;
        mat.matEmittance = sunMaterial.matEmittance;
        mat.matShininess = sunMaterial.matShininess;
    }
    if (object == PLANET)
    {
        coords = earthCoords;
        normal = earthNormal;
        texCoordsExport = earthTexCoords;
        mat.matAmbience = planetMaterial.matAmbience;
        mat.matDiffuse = planetMaterial.matDiffuse;
        mat.matSpecular = planetMaterial.matSpecular;
        mat.matEmittance = planetMaterial.matEmittance;
        mat.matShininess = planetMaterial.matShininess;
    }

	if (object == SKY)
	{
		coords = skyCoords;
		normal = skyNormal;
		texCoordsExport = skyTexCoords;
		mat.matAmbience = sunMaterial.matAmbience;
        mat.matDiffuse = sunMaterial.matDiffuse;
        mat.matSpecular = sunMaterial.matSpecular;
        mat.matEmittance = sunMaterial.matEmittance;
        mat.matShininess = sunMaterial.matShininess;
	}

    if (object == CONE)
	{
		coords = coneCoords;
		normal = coneNormal;
		texCoordsExport = coneTexCoords;
		mat.matAmbience = planetMaterial.matAmbience;
        mat.matDiffuse = planetMaterial.matDiffuse;
        mat.matSpecular = planetMaterial.matSpecular;
        mat.matEmittance = planetMaterial.matEmittance;
        mat.matShininess = planetMaterial.matShininess;
	}
	
	if (object == MARS)
	{
		coords = marsCoords;
		normal = marsNormal;
		texCoordsExport = marsTexCoords;
		mat.matAmbience = planetMaterial.matAmbience;
        mat.matDiffuse = planetMaterial.matDiffuse;
        mat.matSpecular = planetMaterial.matSpecular;
        mat.matEmittance = planetMaterial.matEmittance;
        mat.matShininess = planetMaterial.matShininess;
	}

    normal = normalize(normalMat * normal);
    lightDirection = normalize(vec3(modelViewMat * light.coords - modelViewMat * coords));
    eyeDirection = -1.0f * normalize(vec3(modelViewMat * coords));
    halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection)/length(lightDirection + eyeDirection);

    emittance = mat.matEmittance;
    ambience = light.lightAmbience * mat.matAmbience;
    diffuse = max(dot(normal, lightDirection), 0.0f) * (light.lightDiffuse * mat.matDiffuse);
    specular = pow(max(dot(normal, halfway), 0.0f), mat.matShininess) * (light.lightSpecular * mat.matSpecular);
    frontAmbDiffExport =  vec4(vec3(min(emittance + ambience + diffuse, vec4(1.0))), 1.0);
    frontSpecExport =  vec4(vec3(min(specular, vec4(1.0))), 1.0);

    normal = -1.0f * normal;

    gl_Position = projMat * modelViewMat * coords;
}
