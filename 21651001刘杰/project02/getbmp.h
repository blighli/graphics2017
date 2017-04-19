///////////////////////////////////////////////////////////////////////
// Tony Wang and Luke Johnson
//
// CPSC 425 Final Project
//
// getbmp.h
//
// this is the header file for the getbmp.cpp file
// taken from lit cylinder with texture
///////////////////////////////////////////////////////////////////////

#ifndef GETBMP_H
#define GETBMP_H

using namespace std;

struct BitMapFile
{
    int sizeX;
    int sizeY;
    unsigned char *data;
};

BitMapFile *getbmp(string filename);

#endif
