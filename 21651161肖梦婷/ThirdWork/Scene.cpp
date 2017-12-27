//本部分代码由樊丽整理，为 场景 的实现

// Scene.cpp: implementation of the CEnvironment class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Scene.h"
#include <math.h>

#include "BITMAP.H"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLfloat	r; 
float  gao=1.8f;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


CEnvironment::CEnvironment()
{
//	g_imageData = LoadBit("data/Terrain1.bmp",&g_bit); //生成纹理图
	LoadT8("data/sand0.bmp",	 g_cactus[0]);
//	LoadT8("data/0RBack.bmp", g_cactus[2]);
//	LoadT8("data/0Front.bmp", g_cactus[3]);
//	LoadT8("data/0Top.bmp",	 g_cactus[4]);
//	LoadT8("data/0Left.bmp",  g_cactus[5]);
//	LoadT8("data/0Right.bmp", g_cactus[6]);
	InitTerrain(5);
	glEnable(GL_TEXTURE_2D);
	
}

CEnvironment::~CEnvironment()
{
	for(int i=0;i<16;i++) 
		glDeleteTextures(1, &g_cactus[i]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}
//==========================================================================
void CEnvironment::InitTerrain(float h)  
{ 
	int index = 0;
	int Vertex;
	for (int z = 0; z < MAP_W; z++)
		for (int x = 0; x < MAP_W; x++)
		{ 
			Vertex = z * MAP_W + x;
			g_terrain [Vertex][0] = float(x)*MAP_SCALE;
			g_terrain [Vertex][2] = 0.0;
			g_terrain [Vertex][1] = -float(z)*MAP_SCALE;
			g_texcoord[Vertex][0] = (float) x;
			g_texcoord[Vertex][1] = (float) z;	
			g_index [index++] = Vertex;
			g_index [index++] = Vertex+ MAP_W;
		}
	glEnableClientState(GL_VERTEX_ARRAY);	
	glVertexPointer    (3,GL_FLOAT,0,g_terrain);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer  (2,GL_FLOAT,0,g_texcoord);
}	

void CEnvironment::DrawSand()
{ 
	glBindTexture(GL_TEXTURE_2D, g_cactus[0]);
	
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	
	for (int z = 0; z < MAP_W-1; z++)
		glDrawElements(GL_TRIANGLE_STRIP,MAP_W*2,GL_UNSIGNED_INT,&g_index[z*MAP_W*2]); //渲染数组数据中的图元
}

void CEnvironment::CreateSkyBox(int a,int wi,int he,int le)
{
	float width =MAP*wi;
	float height=MAP*he;
	float length=MAP*le;
	float x = MAP  -width /2;
	float y = MAP/a-height/2;
	float z = -MAP -length/2;
	///////////////////////////////////////////////////////////////////////////////
	texture(g_cactus[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z);
	glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z); 
	glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z);
	glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z);
	glEnd();
	texture(g_cactus[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z+length);
	glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z+length);
	glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
	glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z+length);
	glEnd();
	
	texture(g_cactus[4]);
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z);
	glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y+height,z+length); 
	glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y+height,z+length);
	glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z);
	glEnd();
	texture(g_cactus[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z);	
	glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z+length); 
	glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z+length);
	glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z);		
	glEnd();
	texture(g_cactus[6]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z);
	glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z+length);
	glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
	glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z);
	glEnd();
}

void CEnvironment::texture(UINT textur)
{	
	glBindTexture  (GL_TEXTURE_2D, textur);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

//==========================================================================
bool CEnvironment::LoadT8(char *filename, GLuint &texture)
{	AUX_RGBImageRec *pImage = NULL;
pImage = auxDIBImageLoad(filename);
if(pImage == NULL)		return false;
glGenTextures(1, &texture);	
glBindTexture    (GL_TEXTURE_2D,texture);
gluBuild2DMipmaps(GL_TEXTURE_2D,4, pImage->sizeX, 
				  pImage->sizeY,GL_RGB, GL_UNSIGNED_BYTE,pImage->data);
free(pImage->data);
free(pImage);	
return true;
}
void CEnvironment::LoadT16(char *filename, GLuint &texture)
{ glGenTextures(1, &texture);  
glBindTexture(GL_TEXTURE_2D, texture);
BITMAPINFOHEADER bitHeader;
unsigned char *buffer;  
buffer=LoadBitmapFileWithAlpha(filename,&bitHeader);
gluBuild2DMipmaps	( GL_TEXTURE_2D,  
					 4,    
					 bitHeader.biWidth, 
					 bitHeader.biHeight,
					 GL_RGBA, 
					 GL_UNSIGNED_BYTE,
					 buffer  
					 ); 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
free(buffer);  
}
unsigned char * CEnvironment::LoadBit(char *filename, BITMAPINFOHEADER *bitmap)
{ FILE *filePtr;  
BITMAPFILEHEADER  Header; 
unsigned char    *Image; 
unsigned int      imageIdx = 0; 
unsigned char     tempRGB;  
filePtr = fopen(filename, "rb");
if (filePtr == NULL)    return NULL;
fread(&Header, sizeof(BITMAPFILEHEADER), 1, filePtr);
if (Header.bfType != BITMAP_ID)
{ fclose(filePtr);
return NULL;
}
fread(bitmap, sizeof(BITMAPINFOHEADER), 1, filePtr);
fseek(filePtr, Header.bfOffBits, SEEK_SET);
Image = (unsigned char*)malloc(bitmap->biSizeImage);
if (!Image)
{ free(Image);
fclose(filePtr);
return NULL;
}
fread(Image, 1, bitmap->biSizeImage, filePtr);
if (Image == NULL)
{ fclose(filePtr);
return NULL;
}
for (imageIdx = 0; imageIdx < bitmap->biSizeImage; imageIdx+=3)
{ tempRGB = Image[imageIdx];
Image[imageIdx] = Image[imageIdx + 2];
Image[imageIdx + 2] = tempRGB;
}
fclose(filePtr);
return Image;
}