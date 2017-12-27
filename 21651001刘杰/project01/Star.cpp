/*************************************************************************
	> File Name: Star.cpp
	> Author: liujie
	> Mail: vip727155384@gmail.com 
	> Created Time: Fri 17 Mar 2017 11:11:54 PM PDT
 ************************************************************************/

#include"Star.h"
#include<cmath>

#define PI 3.1415926535

Star::Star(GLfloat radius,GLfloat speed,GLfloat selfspeed,
		GLfloat distance,Star* parentstar ):
	radius(radius),selfspeed(selfspeed),
	distance(distance),parentstar(parentstar),alpha(0),alphaself(0){
		if(speed>0){
			this->speed=360.0f/speed;
		}
		else{
			this->speed=0.0f;
		}
		for(int i=0;i<4;i++){
			rgba_color[i]=1.0f;
		}
	}

void Star::drawstar(){
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	int n=1440;
	glPushMatrix();
	{
		if(parentstar!=NULL&&parentstar->distance>0){
			glRotatef(parentstar->alpha,0,0,1);
			glTranslatef(parentstar->distance,0.0,0.0);
		}
		glBegin(GL_LINES);
		for(int i=0;i<n;i++){
			glVertex2f(distance*cos(2*PI*i/n),distance*sin(2*PI*i/n));
		}
		glEnd();
		glRotatef(alpha,0,0,1);
		glTranslatef(distance,0.0,0.0);
		glRotatef(alphaself,0,0,1);
		glColor3f(rgba_color[0],rgba_color[1],rgba_color[2]);
		glutSolidSphere(radius,40,32);
	}
	glPopMatrix();
}

void Star::update(long timeSpan){
	alpha+=timeSpan*speed;
	alphaself+=selfspeed;
}

Planet::Planet(GLfloat radius,GLfloat speed,GLfloat selfspeed,
		GLfloat distance,Star* parentstar,GLfloat rgbacolor[3]):
	Star(radius,speed,selfspeed,distance,parentstar){
		for(int i=0;i<3;i++){
			rgba_color[i]=rgbacolor[i];
		}
	}

void Planet::drawplanet(){
	GLfloat mat_ambient[]  ={0.0f, 0.0f, 0.5f, 1.0f};
    GLfloat mat_diffuse[]  ={0.0f, 0.0f, 0.5f, 1.0f};
    GLfloat mat_specular[] ={0.0f, 0.0f, 1.0f, 1.0f};
    GLfloat mat_emission[] ={rgba_color[0], rgba_color[1], rgba_color[2], rgba_color[3]};
    GLfloat mat_shininess  = 90.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION,  mat_emission);
    glMaterialf (GL_FRONT, GL_SHININESS, mat_shininess);
}

LightPlanet::LightPlanet(GLfloat radius,GLfloat speed,GLfloat selfspeed,
		GLfloat distance,Star* parentstar,GLfloat rgbacolor[3]):
	Planet(radius,speed,selfspeed,distance,parentstar,rgbacolor){}

void LightPlanet::drawlightplanet() {
    GLfloat light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); 
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);  
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);  
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}
