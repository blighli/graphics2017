///////////////////////////////////////////////////////////////////////
// Tony Wang and Luke Johnson
//
// CPSC 425 Final Project
//
// light.h
//
// This defines a light struct for use in main and elsewhere
// Taken from lit cylinder project
///////////////////////////////////////////////////////////////////////

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct Light
{
    glm::vec4 ambCols;
    glm::vec4 difCols;
    glm::vec4 specCols;
    glm::vec4 coords;
};

#endif
