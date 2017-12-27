///////////////////////////////////////////////////////////////////////
// Tony Wang and Luke Johnson
//
// CPSC 425 Final Project
//
// sphere.cpp
//
// This creates a sphere object
// It was modified from the cylinder.cpp in the lit cylinder project
///////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>

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

#include "sphere.h"

using namespace std;

// Fill the vertex array with co-ordinates of the sample points.
void fillSphereVertexArray(Vertex sphereVertices[(SPHERE_LONGS + 1) * (SPHERE_LATS + 1)])
{
    int index;

    index = 0;
    for (int j = 0; j <= SPHERE_LATS; j++)
        for (int i = 0; i <= SPHERE_LONGS; i++)
        {
            sphereVertices[index].coords.x = cos((-1 + 2 * (float)j/SPHERE_LATS) * PI / 2 ) * cos( 2.0 * (float)i/SPHERE_LONGS * PI );
            sphereVertices[index].coords.y = sin((-1 + 2 * (float)j/SPHERE_LATS) * PI / 2 );
            sphereVertices[index].coords.z = cos((-1 + 2 * (float)j/SPHERE_LATS) * PI / 2 ) * sin( 2.0 * (float)i/SPHERE_LONGS * PI );
            sphereVertices[index].coords.w = 1.0;
            sphereVertices[index].normal.x = cos((-1 + 2 * (float)j/SPHERE_LATS) * PI / 2 ) * cos( 2.0 * (float)i/SPHERE_LONGS * PI );
            sphereVertices[index].normal.y = sin((-1 + 2 * (float)j/SPHERE_LATS) * PI / 2 );
            sphereVertices[index].normal.z = cos((-1 + 2 * (float)j/SPHERE_LATS) * PI / 2 ) * sin( 2.0 * (float)i/SPHERE_LONGS * PI );
            sphereVertices[index].texCoords.s = (float)i / SPHERE_LONGS;
            sphereVertices[index].texCoords.t = (float)j / SPHERE_LATS;
            index++;
        }
}

// Fill the array of index arrays.
void fillSphereIndices(unsigned int sphereIndices[SPHERE_LATS][2*(SPHERE_LONGS+1)])
{
    int i, j;
    for(j = 0; j < SPHERE_LATS; j++)
    {
        for (i = 0; i <= SPHERE_LONGS; i++)
        {
            sphereIndices[j][2*i] = (j + 1) * (SPHERE_LONGS + 1) + i;
            sphereIndices[j][2*i+1] = j * (SPHERE_LONGS + 1) + i;
        }
    }
}

// Fill the array of counts.
void fillSphereCounts(int sphereCounts[SPHERE_LATS])
{
    int j;
    for(j = 0; j < SPHERE_LATS; j++)
        sphereCounts[j] = 2 * (SPHERE_LONGS + 1);
}

// Fill the array of buffer offsets.
void fillSphereOffsets(void* sphereOffsets[SPHERE_LATS])
{
    int j;
    for(j = 0; j < SPHERE_LATS; j++) sphereOffsets[j] = (GLvoid*)(2 * (SPHERE_LONGS+1) * j * sizeof(unsigned int));
}

// Initialize the sphere.
void fillSphere(Vertex sphereVertices[(SPHERE_LONGS + 1) * (SPHERE_LATS + 1)],
                  unsigned int sphereIndices[SPHERE_LATS][2 * (SPHERE_LONGS + 1)],
                  int sphereCounts[SPHERE_LATS],
                  void* sphereOffsets[SPHERE_LATS])
{
    fillSphereVertexArray(sphereVertices);
    fillSphereIndices(sphereIndices);
    fillSphereCounts(sphereCounts);
    fillSphereOffsets(sphereOffsets);
}
