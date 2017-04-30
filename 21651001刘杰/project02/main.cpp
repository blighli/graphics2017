///////////////////////////////////////////////////////////////////////
// Tony Wang and Luke Johnson
//
// CPSC 425 Final Project
//
// main.cpp
//
// This draws the Sun Earth, Mars, and gives the Earth a Christmas Hat.
///////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "shader.h"
#include "sphere.h"
#include "light.h"
#include "material.h"
#include "getbmp.h"
#include "vertex.h"
#include "myCone.h"

using namespace std;
using namespace glm;

/**
* VAO ids
*/
enum object {SUN, EARTH, SKY, CONE, MARS};

/**
* VBO ids
*/
enum buffer {SUN_VERTICES, SUN_INDICES, EARTH_VERTICES, EARTH_INDICES, SKY_VERTICES, CONE_VERTICES, MARS_VERTICES, MARS_INDICES};

static float viewAngleX = 0.1, viewAngleY = 0.05, viewAngleZ = 0.0;
static float sunAngleX = 0.0, sunAngleY = 0.0, sunAngleZ = 0.0;
static float earthOrbitX = 0.0, earthOrbitY = 0.0, earthOrbitZ = 0.0;
static float earthRotateX = -82.845, earthRotateY = 0.0, earthRotateZ = 0.0;
static float marsOrbitX = 0.0, marsOrbitY = 0.0, marsOrbitZ = 0.0;
static float marsRotateX = -90.0, marsRotateY = 0.0, marsRotateZ = 0.0;

/**
* time between frames in milliseconds
*/
static int animationPeriod = 30;
bool isAnimate = false;

/**
* Light properties matrix for our light
*/
static const Light light =
{
    vec4(0.2, 0.2, 0.2, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(0.0, 0.0, 0.0, 0.0)
};

/**
* Material properties matrix for the sun
*/
static const Material sunMaterial =
{
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(0.0, 0.0, 0.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    1000.0f
};

/**
* Material properties matrix for planets
*/
static const Material planetMaterial =
{
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(0.0, 0.0, 0.0, 1.0),
    20.0f
};

/**
* Vertices, normals, and texture mapping for the sky
*/
static Vertex skyVertices[4] =
{
    {vec4(200.0, -200.0, -100.0, 1.0), vec3(1.0), vec2(1.0, 0.0)},
    {vec4(200.0, 200.0, -100.0, 1.0), vec3(1.0), vec2(1.0, 1.0)},
    {vec4(-200.0, -200.0, -100.0, 1.0), vec3(1.0), vec2(0.0, 0.0)},
    {vec4(-200.0, 200.0, -100.0, 1.0), vec3(1.0), vec2(0.0, 1.0)}
};

/**
* Initialize modelView, projection, and normal matrices
*/
static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);
static mat4 normalMat = mat4(1.0);

/**
* Setup configuration for sun
*/
static Vertex sunVertices[(SPHERE_LONGS + 1) * (SPHERE_LATS + 1)];
static unsigned int sunIndices[SPHERE_LATS][2 * (SPHERE_LONGS + 1)];
static int sunCounts[SPHERE_LATS];
static void* sunOffsets[SPHERE_LATS];

/**
* Initial configuration for earth
*/
static Vertex earthVertices[(SPHERE_LONGS + 1) * (SPHERE_LATS + 1)];
static unsigned int earthIndices[SPHERE_LATS][2 * (SPHERE_LONGS + 1)];
static int earthCounts[SPHERE_LATS];
static void* earthOffsets[SPHERE_LATS];

/**
* Initial configuration for mars
*/
static Vertex marsVertices[(SPHERE_LONGS + 1) * (SPHERE_LATS + 1)];
static unsigned int marsIndices[SPHERE_LATS][2 * (SPHERE_LONGS + 1)];
static int marsCounts[SPHERE_LATS];
static void* marsOffsets[SPHERE_LATS];

/**
* Constructing and initializing a hat
*/
MyCone earthHat = MyCone(0.2, 1.0);

/**
 * Initial configuration for the shader
 */
static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
normalMatLoc,
projMatLoc,
sunTexLoc,
earthTexLoc,
marsTexLoc,
skyTexLoc,
hatTexLoc,
objectLoc,
buffer[8],
vao[5],
texture[5],
width,
height;

/**
* Texture bitmaps
*/
static BitMapFile *image[4];

/**
 * Setup configuration for view rotation
 */
vec4 eyeStart = vec4(0.0, 0.0, 30.0, 1.0);
vec4 eye = eyeStart; // camera location
mat4 viewRotation;  // rotational part of matrix that transforms between World and Camera coordinates
vec4 VPN(0,.5,1,0);  // used as starting value for setting uvn
vec4 VUP(0,1,0,1);  // used as starting value for setting uvn

vec4 centerStart = vec4(0.0, 0.0, 0.0, 1.0);
vec4 center = centerStart;

void calcUVN(vec4 VPN, vec4 VUP)
{
    vec4 n = normalize(VPN);
    vec4 u = vec4(cross(vec3(VUP),vec3(n)),0);
    u = normalize(u);
    vec4 v = vec4(cross(vec3(n),vec3(u)),0);
    viewRotation = mat4(u,v,n,vec4(0,0,0,1));
}

void createVaoSun()
{
    glBindVertexArray(vao[SUN]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[SUN_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sunVertices), sunVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SUN_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sunIndices), sunIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sunVertices[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(sunVertices[0]), (void*)sizeof(sunVertices[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sunVertices[0]),
                          (void*)(sizeof(sunVertices[0].coords) + sizeof(sunVertices[0].normal)));
    glEnableVertexAttribArray(2);
}

void createVaoEarth()
{
    glBindVertexArray(vao[EARTH]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[EARTH_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(earthVertices), earthVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[EARTH_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(earthIndices), earthIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(earthVertices[0]), 0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(earthVertices[0]), (void*)sizeof(earthVertices[0].coords));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(earthVertices[0]),
                          (void*)(sizeof(earthVertices[0].coords) + sizeof(earthVertices[0].normal)));
    glEnableVertexAttribArray(5);
}

void createVaoSky()
{
    glBindVertexArray(vao[SKY]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[SKY_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), 0);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), (void*)sizeof(skyVertices[0].coords));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), (void*)(sizeof(skyVertices[0].coords) + sizeof(skyVertices[0].normal)));
    glEnableVertexAttribArray(8);
}

void createVaoCone()
{
    glBindVertexArray(vao[CONE]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[CONE_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(earthHat.vertices), earthHat.vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(earthHat.vertices[0]), 0);
    glEnableVertexAttribArray(9);
    glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, sizeof(earthHat.vertices[0]), (void*)sizeof(earthHat.vertices[0].coords));
    glEnableVertexAttribArray(10);
    glVertexAttribPointer(11, 2, GL_FLOAT, GL_FALSE, sizeof(earthHat.vertices[0]), (void*)(sizeof(earthHat.vertices[0].coords) + sizeof(earthHat.vertices[0].normal)));
    glEnableVertexAttribArray(11);
}

void createVaoMars()
{
    glBindVertexArray(vao[MARS]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[MARS_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(marsVertices), marsVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[MARS_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(marsIndices), marsIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(marsVertices[0]), 0);
    glEnableVertexAttribArray(12);
    glVertexAttribPointer(13, 3, GL_FLOAT, GL_FALSE, sizeof(marsVertices[0]), (void*)sizeof(marsVertices[0].coords));
    glEnableVertexAttribArray(13);
    glVertexAttribPointer(14, 2, GL_FLOAT, GL_FALSE, sizeof(marsVertices[0]),
                          (void*)(sizeof(marsVertices[0].coords) + sizeof(marsVertices[0].normal)));
    glEnableVertexAttribArray(14);
}

void createAndLinkShader()
{
    vertexShaderId = setShader("vertex", "vertexShader.glsl");
    fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    glUseProgram(programId);
}

void setShaderConfiguration()
{
    /// Obtain modelView matrix, projection matrix, normal matrix and object uniform locations.
    modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
    projMatLoc = glGetUniformLocation(programId, "projMat");
    normalMatLoc = glGetUniformLocation(programId, "normalMat");
    objectLoc = glGetUniformLocation(programId, "object");

    /// Obtain light properties matrix.
    glUniform4fv(glGetUniformLocation(programId, "light.lightAmbience"), 1, &light.ambCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light.lightDiffuse"), 1, &light.difCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light.lightSpecular"), 1, &light.specCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light.coords"), 1, &light.coords[0]);

    /// Obtain material properties matrix for sun.
    glUniform4fv(glGetUniformLocation(programId, "sunMaterial.matAmbience"), 1, &sunMaterial.ambRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "sunMaterial.matDiffuse"), 1, &sunMaterial.difRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "sunMaterial.matSpecular"), 1, &sunMaterial.specRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "sunMaterial.matEmittance"), 1, &sunMaterial.emitCols[0]);
    glUniform1f(glGetUniformLocation(programId, "sunMaterial.matShininess"), sunMaterial.shininess);

    /// Obtain material properties matrix for planets.
    glUniform4fv(glGetUniformLocation(programId, "planetMaterial.matAmbience"), 1, &planetMaterial.ambRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "planetMaterial.matDiffuse"), 1, &planetMaterial.difRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "planetMaterial.matSpecular"), 1, &planetMaterial.specRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "planetMaterial.matEmittance"), 1, &planetMaterial.emitCols[0]);
    glUniform1f(glGetUniformLocation(programId, "planetMaterial.matShininess"), planetMaterial.shininess);
}

void bindSunTexture()
{
    image[0] = getbmp("sun_texture.bmp");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    sunTexLoc = glGetUniformLocation(programId, "sunTex");
    glUniform1i(sunTexLoc, 0);
}

void bindEarthTexture()
{
    image[1] = getbmp("earth_texture.bmp");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    earthTexLoc = glGetUniformLocation(programId, "earthTex");
    glUniform1i(earthTexLoc, 1);
}

void bindSkyTexture()
{
    image[2] = getbmp("sky_texture.bmp");
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    skyTexLoc = glGetUniformLocation(programId, "skyTex");
    glUniform1i(skyTexLoc, 2);
}

void bindEarthHat()
{
    image[3] = getbmp("hat_texture.bmp");
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->sizeX, image[3]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    hatTexLoc = glGetUniformLocation(programId, "hatTex");
    glUniform1i(hatTexLoc, 3);
}

void bindMarsTexture()
{
    image[4] = getbmp("mars_texture.bmp");
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[4]->sizeX, image[4]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[4]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    marsTexLoc = glGetUniformLocation(programId, "marsTex");
    glUniform1i(marsTexLoc, 4);
}

void updateNormals(mat4 mvmSave)
{
    normalMat = transpose(inverse(mat3(modelViewMat)));
    glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, value_ptr(normalMat));
    modelViewMat = mvmSave;
}

// Initialization routine.
void setup(void)
{
    calcUVN(VPN, VUP);

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    createAndLinkShader();
    setShaderConfiguration();

    /// Fill vertices, normals, and textures of VAOs
    fillSphere(sunVertices, sunIndices, sunCounts, sunOffsets);
    fillSphere(earthVertices, earthIndices, earthCounts, earthOffsets);
    fillSphere(marsVertices, marsIndices, marsCounts, marsOffsets);

    /// Create VAO's
    glGenVertexArrays(5, vao);
    glGenBuffers(8, buffer);
    createVaoSun();
    createVaoEarth();
    createVaoSky();
    createVaoCone();
    createVaoMars();

    /// Create texture ids.
    glGenTextures(5, texture);

    /// Bind textures
    bindSunTexture();
    bindEarthTexture();
    bindSkyTexture();
    bindEarthHat();
    bindMarsTexture();
}

void initView()
{
    /// Calculate and update projection matrix.
    projMat = frustum(-1.0, 1.0, -1.0, 1.0, 1.0, 300.0);
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

    /// Calculate and update modelview matrix.
    modelViewMat = lookAt(vec3(eye), vec3(center), vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
}

void drawSky()
{
    glUniform1ui(objectLoc, SKY);
    glBindVertexArray(vao[SKY]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void drawSkybox(mat4 mvmSave)
{
    drawSky();
    modelViewMat = rotate(modelViewMat, 90.0f, vec3(1.0, 0.0, 0.0));
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
    drawSky();
    modelViewMat = mvmSave;
    modelViewMat = rotate(modelViewMat, -90.0f, vec3(1.0, 0.0, 0.0));
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
    drawSky();
    modelViewMat = mvmSave;
    modelViewMat = rotate(modelViewMat, 90.0f, vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
    drawSky();
    modelViewMat = mvmSave;
    modelViewMat = rotate(modelViewMat, -90.0f, vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
    drawSky();
    modelViewMat = mvmSave;
    modelViewMat = rotate(modelViewMat, 180.0f, vec3(0.0, 0.0, 0.0));
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
    drawSky();
}

void rotateScene()
{
    modelViewMat = rotate(modelViewMat, viewAngleZ, vec3(0.0, 0.0, 1.0));
    modelViewMat = rotate(modelViewMat, viewAngleY, vec3(0.0, 1.0, 0.0));
    modelViewMat = rotate(modelViewMat, viewAngleX, vec3(1.0, 0.0, 0.0));
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
}

void drawSun()
{
    /// Handle view rotations
    modelViewMat = rotate(modelViewMat, sunAngleZ, vec3(0.0, 0.0, 1.0));
    modelViewMat = rotate(modelViewMat, sunAngleY, vec3(0.0, 1.0, 0.0));
    modelViewMat = rotate(modelViewMat, sunAngleX, vec3(1.0, 0.0, 0.0));
    modelViewMat = scale(modelViewMat, vec3(6.04, 6.04, 6.04));
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
    /// Draw sun.
    glUniform1ui(objectLoc, SUN);
    glBindVertexArray(vao[SUN]);
    glMultiDrawElements(GL_TRIANGLE_STRIP, sunCounts, GL_UNSIGNED_INT, (const void **)sunOffsets, SPHERE_LATS);
}

void drawEarth()
{
    modelViewMat = rotate(modelViewMat, earthOrbitX, vec3(0.0, 0.0, 1.0));
    modelViewMat = rotate(modelViewMat, earthOrbitY, vec3(0.0, 1.0, 0.0));
    modelViewMat = rotate(modelViewMat, earthOrbitZ, vec3(1.0, 0.0, 0.0));
    modelViewMat = translate(modelViewMat, vec3(-15.0, 0.0, 0.0));
    modelViewMat = scale(modelViewMat, vec3(2.0, 2.0, 2.0));
    modelViewMat = rotate(modelViewMat, earthRotateX, vec3(0.0, 0.0, 1.0));
    modelViewMat = rotate(modelViewMat, earthRotateY, vec3(0.0, 1.0, 0.0));
    modelViewMat = rotate(modelViewMat, earthRotateZ, vec3(1.0, 0.0, 0.0));
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

    glUniform1ui(objectLoc, EARTH);
    glBindVertexArray(vao[EARTH]);
    glMultiDrawElements(GL_TRIANGLE_STRIP, earthCounts, GL_UNSIGNED_INT, (const void **)earthOffsets, SPHERE_LATS);
}

drawPartyhat()
{
    modelViewMat = rotate(modelViewMat, earthOrbitX, vec3(0.0, 0.0, 1.0));
    modelViewMat = rotate(modelViewMat, earthOrbitY, vec3(0.0, 1.0, 0.0));
    modelViewMat = rotate(modelViewMat, earthOrbitZ, vec3(1.0, 0.0, 0.0));
    modelViewMat = translate(modelViewMat, vec3(-15.0, 1.8, 0.0));
    modelViewMat = rotate(modelViewMat, 0.0f, vec3(1.0, 0.0, 0.0));
    modelViewMat = scale(modelViewMat, vec3(2.0, 2.0, 2.0));
    modelViewMat = rotate(modelViewMat, earthRotateX - earthRotateX, vec3(0.0, 0.0, 1.0));
    modelViewMat = rotate(modelViewMat, earthRotateY, vec3(0.0, 1.0, 0.0));
    modelViewMat = rotate(modelViewMat, earthRotateZ, vec3(1.0, 0.0, 0.0));
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

    glUniform1ui(objectLoc, CONE);
    glBindVertexArray(vao[CONE]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices + 2);
}

void drawMars()
{
    modelViewMat = rotate(modelViewMat, marsOrbitX, vec3(0.0, 0.0, 1.0));
    modelViewMat = rotate(modelViewMat, marsOrbitY, vec3(0.0, 1.0, 0.0));
    modelViewMat = rotate(modelViewMat, marsOrbitZ, vec3(1.0, 0.0, 0.0));
    modelViewMat = translate(modelViewMat, vec3(-20.0, 0.0, 0.0));
    modelViewMat = scale(modelViewMat, vec3(1.22, 1.22, 1.22));
    modelViewMat = rotate(modelViewMat, marsRotateX, vec3(0.0, 0.0, 1.0));
    modelViewMat = rotate(modelViewMat, marsRotateY, vec3(0.0, 1.0, 0.0));
    modelViewMat = rotate(modelViewMat, marsRotateZ, vec3(1.0, 0.0, 0.0));
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

    glUniform1ui(objectLoc, MARS);
    glBindVertexArray(vao[MARS]);
    glMultiDrawElements(GL_TRIANGLE_STRIP, marsCounts, GL_UNSIGNED_INT, (const void **)marsOffsets, SPHERE_LATS);
}

/**
* Drawing routine
*/
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    initView();
    mat4 mvmSave = modelViewMat;
    drawSkybox(mvmSave);
    updateNormals(mvmSave);
    rotateScene();
    mvmSave = modelViewMat;
    drawSun();
    updateNormals(mvmSave);
    drawEarth();
    updateNormals(mvmSave);
    drawPartyhat();
    updateNormals(mvmSave);
    drawMars();
    updateNormals(mvmSave);
    glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}

void animate(int value)
{
    if (isAnimate)
    {
        sunAngleY -= 0.0011;
        if (sunAngleY > 360.0) sunAngleY -= 360.0;
        earthOrbitY -= 0.01;
        if (earthOrbitY > 360.0) earthOrbitY -= 360.0;
        earthRotateY += 0.04;
        if (earthRotateY > 360.0) earthRotateY -= 360.0;
        marsOrbitY -= 0.0188;
        if (marsOrbitY > 360.0) marsOrbitY -= 360.0;
        marsRotateY += 0.04;
        if (marsRotateY > 360.0) marsRotateY -= 360.0;
        glutPostRedisplay();
        glutTimerFunc(animationPeriod, animate, 1);
    }
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:
        exit(0);
        break;
    case 'r':
        center = centerStart;
        eye = eyeStart;
        viewAngleX = 0.0;
        viewAngleY = 0.0;
        viewAngleZ = 0.0;
        break;
    case 'x':
        viewAngleX += 0.01;
        if (viewAngleX > 360.0) viewAngleX = 360.0;
        glutPostRedisplay();
        break;
    case 'X':
        viewAngleX -= 0.01;
        if (viewAngleX < 0.0) viewAngleX = 0.0;
        glutPostRedisplay();
        break;
    case 'y':
        viewAngleY += 0.01;
        if (viewAngleY > 360.0) viewAngleY = 360.0;
        glutPostRedisplay();
        break;
    case 'Y':
        viewAngleY -= 0.01;
        if (viewAngleY < 0.0) viewAngleY = 0.0;
        glutPostRedisplay();
        break;
    case 'z':
        viewAngleZ += 0.01;
        if (viewAngleZ > 360.0) viewAngleZ = 360.0;
        glutPostRedisplay();
        break;
    case 'Z':
        viewAngleZ -= 0.01;
        if (viewAngleZ < 0.0) viewAngleZ = 0.0;
        glutPostRedisplay();
        break;
    case ' ':
        if (isAnimate) isAnimate = 0;
        else
        {
            isAnimate = 1;
            animate(1);
        }
        break;
    default:
        break;
    }
}

void keySpecial(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
        center.x -= 0.2;
        if (center.x < -90.0) center.x = -90.0;
        break;
    case GLUT_KEY_RIGHT:
        center.x += 0.2;
        if (center.x > 90.0) center.x = 90.0;
        break;
    case GLUT_KEY_UP:
        center.y += 0.2;
        if (center.y > 90.0) center.y = 90.0;
        break;
    case GLUT_KEY_DOWN:
        center.y -= 0.2;
        if (center.y < -90.0) center.y = -90.0;
        break;
    case GLUT_KEY_PAGE_DOWN:
        eye.z += 0.25;
        if (eye.z > 80.0) eye.z = 70.0;
        break;
    case GLUT_KEY_PAGE_UP:
        eye.z -= 0.25;
        if (eye.z < 8.0) eye.z = 8.0;
        break;
    }
    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press x, X, y, Y, z, Z to turn the sphere." << endl;
    cout << "Use arrow keys to nudge the camera." << endl;
    cout << "PageUp and PageDown zoom the camera in and out." << endl;
}

// Main routine.
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Solar System");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(keySpecial);

    glewExperimental = GL_TRUE;
    glewInit();
    setup();
    glutMainLoop();
}

