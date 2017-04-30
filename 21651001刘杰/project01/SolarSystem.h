/*************************************************************************
	> File Name: SolarSystem.h
	> Author: liujie
	> Mail: vip727155384@gmail.com 
	> Created Time: Fri 17 Mar 2017 10:30:24 PM PDT
 ************************************************************************/

#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include"Star.h"
#include<GL/glut.h>

#define STARS_NUM 10
#define TIMEPAST 1
#define SELFROTATE 3

enum STARS{
	Sun,//太阳
	Mercury,//水星
	Venus,//金星
	Earth,//地球
	Moon,//月亮
	Mars,//火星
	Jupiter,//木星
	Saturn,//土星
	Uranus,//天王星
	Neptune//海王星
};

class SolarSystem{
public:
	SolarSystem();
	~SolarSystem();
	void onDisplay();
	void onUpdate();
	void onkeyboard(unsigned char key,int x,int y);
private:
	Star* stars[STARS_NUM];
	GLdouble viewX,viewY,viewZ;
	GLdouble centerX,centerY,centerZ;
	GLdouble upX,upY,upZ;
};

#endif
