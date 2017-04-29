#include <stdlib.h>
#include <stdio.h>
#include <math.h> 
#include <GL/glut.h>
#include <GL/GLAux.h>
#include "camera.h"
struct MUSHROOM
{
	float x, y, z;					//蘑菇的位置
	int texT, texM, texB;			//蘑菇不同部分对应的纹理
};
extern void Display();
extern void Display2();
void Reshape(int width, int height);

extern CAMERA camera;
extern int SPOT_LIGHT_ON;			//聚光灯是否打开
extern float DEG_TO_RAD;
extern float viewDX, viewDY, viewDZ;
extern float eyePX, eyePY, eyePZ;
extern int Window_Width;
extern int Window_Height;
extern int L;
extern float W;
extern int H;
extern float PR;
float BORDERW = 60;
POINT mousePos;
#define MAX_MUSHROOM_ORG 30			//最大蘑菇数
extern int MAX_MUSHROOM;
extern MUSHROOM* mushroom;
extern void MushroomInit();
float dx = 0.0f;
float dy = 0.0f;
float dz = -1.0f;

float ax = 0.0f;
float ay = 0.0f;

float r = 1000.0f;
extern struct mPOINT oldpt;                 
int l_button_down=FALSE; 
struct POLAR polar={ 150, 0, 45};         

const float FIT1 = 0.05;
const float FIT2 = 0.08;
int UDSPEED = 2.0f;
int SPEED0 = 2.0f;
int SPEED = SPEED0;
int maxSPEED = 8.0f;
int ACC = 2.0f;
int JUMP = 30.0f;
int Jtag = 1;

void SetCamera(GLfloat x, GLfloat y)        
{
				ax += x*FIT1;
				ay -= y*FIT2;
				if (ay >= 90) ay = 90;
				if (ay <= -90) ay = -90;
				
				dz = -r*cos(ax*DEG_TO_RAD);
				dx = r*sin(ax*DEG_TO_RAD);

				dy = r*sin(ay);

				dy = r*sin(ay*DEG_TO_RAD);
			    dx = dx * cos(ay*DEG_TO_RAD);
				dz = dz * cos(ay*DEG_TO_RAD);
				viewDX = eyePX + dx;
				viewDY = eyePY + dy;
				viewDZ = eyePZ + dz;               
              //Display();                    
				Display2();
}
 
void OnMouseMove(int x, int y)         //鼠标按住移动
{
       if(l_button_down)        
       {
              SetCamera(float(x-oldpt.x),float(y-oldpt.y));      
 
              oldpt.x=x;             
              oldpt.y=y;
       }
}
void MouseMove(int x, int y)		//鼠标移动
{
	GetCursorPos(&mousePos);
	if ((mousePos.x != Window_Width/2) || (mousePos.y != Window_Height/2))
	{
			  

			  SetCamera(float(mousePos.x)-float(Window_Width/2),float(mousePos.y)-float(Window_Height/2));      
              SetCursorPos(Window_Width/2, Window_Height/2);
			  oldpt.x=x;             
              oldpt.y=y;
	}
}
void Mouse(int button, int state, int x, int y)       
{
   if (button == GLUT_WHEEL_UP)
   {
		if ((eyePY - UDSPEED) > -24)
			eyePY -= UDSPEED;
		viewDX = eyePX + dx;
		viewDY = eyePY + dy;
		viewDZ = eyePZ + dz;
			Display2();
   }
   if (button == GLUT_WHEEL_DOWN)
   {
		if ((eyePY + UDSPEED) > -24)
			eyePY += UDSPEED;
		viewDX = eyePX + dx;
		viewDY = eyePY + dy;
		viewDZ = eyePZ + dz;
		Display2();
   }
   if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
   {
					if (Jtag == 1)
					{
						//eyePZ -= SPEED*cos(ax*DEG_TO_RAD);
						//eyePX += SPEED*sin(ax*DEG_TO_RAD);
						eyePY += JUMP;

						viewDX = eyePX + dx;
						viewDY = eyePY + dy;
						viewDZ = eyePZ + dz;
						Jtag = 0;
						Display2();
					}
   }
   if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
   {
			eyePY -= JUMP;
			Jtag = 1;
			viewDX = eyePX + dx;
			viewDY = eyePY + dy;
			viewDZ = eyePZ + dz;
			Display2();
   }
}
bool ValidX(float x)
{
	if ((-L + BORDERW < x) && (x < L - BORDERW))
		return true;
	return false;
}
bool ValidZ(float z)
{
	if ((-H + BORDERW < z) && (z < H - BORDERW))
		return true;
	return false;
}
void Keyboard(unsigned char key, int x, int y)
{
		switch (key)
		{
				case 'a':
					if (ValidX(eyePX - SPEED*cos(ax*DEG_TO_RAD)))
					eyePX -= SPEED*cos(ax*DEG_TO_RAD);
					if (ValidZ(eyePZ - SPEED*sin(ax*DEG_TO_RAD)))
					eyePZ -= SPEED*sin(ax*DEG_TO_RAD);
					break;
				case 'd':
					if (ValidX(eyePX + SPEED*cos(ax*DEG_TO_RAD)))
					eyePX += SPEED*cos(ax*DEG_TO_RAD);
					if (ValidZ(eyePZ + SPEED*sin(ax*DEG_TO_RAD)))
					eyePZ += SPEED*sin(ax*DEG_TO_RAD);
					break;
				case 'w':
					if (ValidZ(eyePZ - SPEED*cos(ax*DEG_TO_RAD)))
					eyePZ -= SPEED*cos(ax*DEG_TO_RAD);
					if (ValidX(eyePX + SPEED*sin(ax*DEG_TO_RAD)))
					eyePX += SPEED*sin(ax*DEG_TO_RAD);
					break;
				case 's':
					if (ValidZ(eyePZ + SPEED*cos(ax*DEG_TO_RAD)))
					eyePZ += SPEED*cos(ax*DEG_TO_RAD);
					if (ValidX(eyePX - SPEED*sin(ax*DEG_TO_RAD)))
					eyePX -= SPEED*sin(ax*DEG_TO_RAD);
					break;
				case ' ':
					if (Jtag == 1)
					{
						//eyePZ -= SPEED*cos(ax*DEG_TO_RAD);
						//eyePX += SPEED*sin(ax*DEG_TO_RAD);
						eyePY += JUMP;
						Jtag = 0;
					}
					break;
				case 27:
					exit(0);
					break;
				case 'f':
					SPOT_LIGHT_ON = -SPOT_LIGHT_ON;
					break;
				case 'j':
					PR+=0.05;
					break;
				case 'k':
					PR-=0.05;
					break;
				case 'i':
					delete[] mushroom;
					MAX_MUSHROOM+=5;
					MushroomInit();
					break;
				case 'o':
					if (MAX_MUSHROOM >50)
					{
					delete[] mushroom;
					MAX_MUSHROOM-=5;
					MushroomInit();
					}
					break;
				default:
					break;
		}
		if (SPEED <= maxSPEED)
			SPEED += ACC;
		viewDX = eyePX + dx;
		viewDY = eyePY + dy;
		viewDZ = eyePZ + dz;
		Display2();	
}
void UpKeyboard(unsigned char key, int x ,int y)
{
	if (key)
	{
		SPEED = SPEED0;
		if (key == ' ')
		{
			eyePY -= JUMP;
			Jtag = 1;
			viewDX = eyePX + dx;
			viewDY = eyePY + dy;
			viewDZ = eyePZ + dz;
			Display2();
		}
	}
}
void SpecialKeyboard(int key,int x,int y)
{
		switch (key)
		{
				case GLUT_KEY_LEFT:
					if (ValidX(eyePX - SPEED*cos(ax*DEG_TO_RAD)))
					eyePX -= SPEED*cos(ax*DEG_TO_RAD);
					if (ValidZ(eyePZ - SPEED*sin(ax*DEG_TO_RAD)))
					eyePZ -= SPEED*sin(ax*DEG_TO_RAD);
					break;
				case GLUT_KEY_RIGHT:
					if (ValidX(eyePX + SPEED*cos(ax*DEG_TO_RAD)))
					eyePX += SPEED*cos(ax*DEG_TO_RAD);
					if (ValidZ(eyePZ + SPEED*sin(ax*DEG_TO_RAD)))
					eyePZ += SPEED*sin(ax*DEG_TO_RAD);
					break;
				case GLUT_KEY_UP:
					if (ValidZ(eyePZ - SPEED*cos(ax*DEG_TO_RAD)))
					eyePZ -= SPEED*cos(ax*DEG_TO_RAD);
					if (ValidX(eyePX + SPEED*sin(ax*DEG_TO_RAD)))
					eyePX += SPEED*sin(ax*DEG_TO_RAD);
					break;
				case GLUT_KEY_DOWN:
					if (ValidZ(eyePZ + SPEED*cos(ax*DEG_TO_RAD)))
					eyePZ += SPEED*cos(ax*DEG_TO_RAD);
					if (ValidX(eyePX - SPEED*sin(ax*DEG_TO_RAD)))
					eyePX -= SPEED*sin(ax*DEG_TO_RAD);
					break;
				case GLUT_KEY_F1:
					glutFullScreen();
					ShowCursor (FALSE);
					break;
				case GLUT_KEY_F2:
					glutReshapeWindow(Window_Width,Window_Height);
					ShowCursor (TRUE);
					break;
				case GLUT_KEY_F3:
					
					break;
				default:
					break;
		}
		if (SPEED <= maxSPEED)
			SPEED += ACC;
		viewDX = eyePX + dx;
		viewDY = eyePY + dy;
		viewDZ = eyePZ + dz;
		Display2();
}
void SpecialUpKeyboard(int key,int x,int y)
{
	if (key)
	{
		SPEED = SPEED;
	}
}