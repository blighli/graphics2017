#ifndef TGATEXTURE_H
#define TGATEXTURE_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <time.h>
#include <iostream>
using namespace std;

typedef struct
{
	GLubyte *imageData;
	GLuint bpp; 
	GLuint width; 
	GLuint height;
	GLuint texID; 
}TextureImage;

bool LoadTGA(TextureImage *texture, char *fileName);
#endif
