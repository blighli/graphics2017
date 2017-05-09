///////////////////////////////////////////////////////////////////////
// Tony Wang and Luke Johnson
//
// CPSC 425 Final Project
//
// myCone.cpp
//
// This creates a cone and does not conflict with glm Cone
///////////////////////////////////////////////////////////////////////

#include "myCone.h"

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

#include<cmath>
#include "vertex.h"

const float PI = 3.141592653;

using namespace glm;

MyCone::MyCone(float radius, float height)
{
    vertices[0].coords.x = 0.0;
    vertices[0].coords.y = height;
    vertices[0].coords.z = 0.0;
    vertices[0].coords.w = 1.0;
    vertices[0].normal.x = 0.0;
    vertices[0].coords.y = 1.0;
    vertices[0].coords.z = 0.0;
    vertices[0].texCoords.s = 0.0;
    vertices[0].texCoords.t = 1.0;

    for(int i = 0; i <= numVertices + 1; i++)
    {
        vertices[i+1].coords.x = radius * cos(2 * PI * i / numVertices);
        vertices[i+1].coords.y = 0.0;
        vertices[i+1].coords.z = radius * sin(2 * PI * i / numVertices);
        vertices[i+1].coords.w = 1.0;
        vertices[i+1].normal.x = cos(2 * PI * i / numVertices);
        vertices[i+1].normal.y = radius / height;
        vertices[i+1].normal.z = sin(2 * PI * i / numVertices);
        vertices[i+1].texCoords.s = 1.0;
        vertices[i+1].texCoords.t = 0.0;
    }
}

MyCone::~MyCone()
{
    delete[] vertices;
}
