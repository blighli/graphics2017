#version 430 core

const int SUN = 0;
const int PLANET = 1;
const int SKY = 2;
const int CONE = 3;
const int MARS = 4;

in vec4 frontAmbDiffExport, frontSpecExport;
in vec2 texCoordsExport;

uniform sampler2D sunTex;
uniform sampler2D earthTex;
uniform sampler2D skyTex;
uniform sampler2D hatTex;
uniform sampler2D marsTex;
uniform uint object;

out vec4 colorsOut;

vec4 texColor;

void main(void)
{
    if (object == SUN) texColor = texture(sunTex, texCoordsExport);
    if (object == PLANET) texColor = texture(earthTex, texCoordsExport);
	if (object == SKY) texColor = texture(skyTex, texCoordsExport);
	if (object == CONE) texColor = texture(hatTex, texCoordsExport);
	if (object == MARS) texColor = texture(marsTex, texCoordsExport);

	if (object == SKY) colorsOut = texColor;
	if (object == SUN) colorsOut = (frontAmbDiffExport * texColor + frontSpecExport);
	if (object == PLANET) colorsOut = (frontAmbDiffExport * texColor);
    if (object == CONE) colorsOut = (frontAmbDiffExport * texColor + frontSpecExport);
	if (object == MARS) colorsOut = (frontAmbDiffExport * texColor);
}
