//本部分代码由樊丽整理，为 场景 的实现
// Scene.h: interface for the CEnvironment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENVIRONMENT_H__395FF7AB_979B_4C27_BCE9_6DB48E99F49C__INCLUDED_)
#define AFX_ENVIRONMENT_H__395FF7AB_979B_4C27_BCE9_6DB48E99F49C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int  MAP_W = 32;       // size of map along x-axis 32 
const float  MAP_SCALE = 24.0;     // the scale of the terrain map
#define MAP			MAP_W*MAP_SCALE/2
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define RAND_COORD(x)   ((float)rand()/RAND_MAX * (x))
#define FRAND   (((float)rand()-(float)rand())/RAND_MAX)

class CEnvironment  
{
public:
	CEnvironment();
	virtual ~CEnvironment();
	
public:
	
	float		g_terrain	[MAP_W*MAP_W][3];
	GLuint		g_index		[MAP_W*MAP_W* 2];
	float		g_texcoord	[MAP_W*MAP_W][2];
	void		InitTerrain(float h);
	void		DrawSand();	
	
	UINT		g_cactus[16];
	BITMAPINFOHEADER  g_bit;   
	unsigned char    *g_imageData; 
	void		CreateSkyBox(int a,int wi,int he,int le);
	void		texture(UINT textur);
	
	bool		LoadT8(char *filename, GLuint &texture);
	void		LoadT16(char *filename, GLuint &texture);
	unsigned char* LoadBit(char *filename, BITMAPINFOHEADER *bitmap);
	
};

#endif // !defined(AFX_ENVIRONMENT_H__395FF7AB_979B_4C27_BCE9_6DB48E99F49C__INCLUDED_)
