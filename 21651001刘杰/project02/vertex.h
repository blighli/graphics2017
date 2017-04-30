///////////////////////////////////////////////////////////////////////
// Tony Wang and Luke Johnson
//
// CPSC 425 Final Project
//
// vertex.h
//
// This creates the vertex struct that is used in main and for objects.
// taken from lit cylinder project.
///////////////////////////////////////////////////////////////////////

#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec4 coords;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

#endif
