/*************************************************************************
	> File Name: Star.h
	> Author: liujie
	> Mail: vip727155384@gmail.com 
	> Created Time: Fri 17 Mar 2017 08:47:10 PM PDT
 ************************************************************************/

#ifndef STAR_H
#define STAR_H

#include<GL/glut.h>

class Star{
public:
	Star(GLfloat radius,
			GLfloat speed,
			GLfloat selfspeed,
			GLfloat distance,
			Star* parentStar );
	void drawstar();
	virtual void draw(){drawstar();}
	virtual void update(long timeSpan);
	//颜色
	GLfloat rgba_color[4];
	//公转半径
	GLfloat radius;
	//自转速度
	GLfloat selfspeed;
	//公转速度
	GLfloat speed;
	//距离
	GLfloat distance;
	//绕行的星球
	Star* parentstar; 
protected:
	//自转角度
	GLfloat alphaself;
	//公转角度
	GLfloat alpha;
};

class Planet:public Star{
public:
	Planet(GLfloat radius,
			GLfloat speed,
			GLfloat selfspeed,
			GLfloat distance,
			Star* parentstar,
			GLfloat rgba_color[3]);
	void drawplanet();
	virtual void draw(){drawstar();drawplanet();}
};

class LightPlanet:public Planet{
public:
	LightPlanet(GLfloat radius,
			GLfloat speed,
			GLfloat selfspeed,
			GLfloat distance,
			Star* parentstar,
			GLfloat rgba_color[333]);
	void drawlightplanet();
	virtual void draw(){drawstar();drawplanet();drawlightplanet();}
};

#endif
