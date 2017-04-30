///////////////////////////////////////////////////////////////////////
// Tony Wang and Luke Johnson
//
// CPSC 425 Final Project
//
// myCone.h
//
// This the header file for the cone class. It a proper c++ class.
///////////////////////////////////////////////////////////////////////

#ifndef MYCONE_H
#define MYCONE_H

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

#include "vertex.h"

using glm::vec4;

const int numVertices = 62;

class MyCone
{
    public:
        MyCone(float radius, float height);
        virtual ~MyCone();

        Vertex vertices[numVertices + 2];
};

#endif // CONE_H
