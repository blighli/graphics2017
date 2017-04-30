/*************************************************************************
	> File Name: SolarSystem.cpp
	> Author: liujie
	> Mail: vip727155384@gmail.com 
	> Created Time: Fri 17 Mar 2017 12:40:39 AM PDT
 ************************************************************************/

#include"SolarSystem.h"

#define REST 700
#define REST_Z REST
#define REST_Y -REST

void SolarSystem::onUpdate(){
	for(int i=0;i<STARS_NUM;i++){
		stars[i]->update(TIMEPAST);
	}
	onDisplay();
}

void SolarSystem::onDisplay(){
	glClear(GL_COLOR_BUFFER_BIT  |  GL_DEPTH_BUFFER_BIT);
    glClearColor(.7f, .7f, .7f, .1f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0f, 1.0f, 1.0f, 40000000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(viewX, viewY, viewZ, centerX, centerY, centerZ, upX, upY, upZ);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
	for(int i=0;i<STARS_NUM;i++){
		stars[i]->draw();
	}
	glutSwapBuffers();
}

#define OFFSET 20
#define SUN_RADIUS 48.74
#define MER_RADIUS  7.32
#define VEN_RADIUS 18.15
#define EAR_RADIUS 19.13
#define MOO_RADIUS  6.15
#define MAR_RADIUS 10.19
#define JUP_RADIUS 42.90
#define SAT_RADIUS 36.16
#define URA_RADIUS 25.56
#define NEP_RADIUS 24.78

#define MER_DIS   62.06
#define VEN_DIS  115.56
#define EAR_DIS  168.00
#define MOO_DIS   26.01
#define MAR_DIS  228.00
#define JUP_DIS  333.40
#define SAT_DIS  428.10
#define URA_DIS 848.00
#define NEP_DIS 949.10

#define MER_SPEED   87.0
#define VEN_SPEED  225.0
#define EAR_SPEED  365.0
#define MOO_SPEED   30.0
#define MAR_SPEED  687.0
#define JUP_SPEED 1298.4
#define SAT_SPEED 3225.6
#define URA_SPEED 3066.4
#define NEP_SPEED 6014.8
#define SELFROTATE 3

#define SET_VALUE_3(name,value0,value1,value2) ((name[0]=value0),\
		(name[1]=value1,name[2]=value2))

SolarSystem::SolarSystem(){
	viewX=0;
	viewY=REST_Y;
	viewZ=REST_Z;
	centerX=centerY=centerZ=0;
	upX=0;
	upY=0;
	upZ=10;
	GLfloat rgba_color[3]={1,0,0};
	stars[Sun]=new LightPlanet(SUN_RADIUS,0,SELFROTATE,0,NULL,rgba_color);
	SET_VALUE_3(rgba_color,.2,.2,.5);
	stars[Mercury]=new Planet(MER_RADIUS,MER_SPEED,SELFROTATE,MER_DIS,
			stars[Sun],rgba_color);
	SET_VALUE_3(rgba_color,1,.7,0);
	stars[Venus]=new Planet(VEN_RADIUS,VEN_SPEED,SELFROTATE,VEN_DIS,
			stars[Sun],rgba_color);
	SET_VALUE_3(rgba_color, 0, 1, 0);
    stars[Earth]   = new Planet(EAR_RADIUS, EAR_SPEED, SELFROTATE,EAR_DIS, stars[Sun], rgba_color);

    SET_VALUE_3(rgba_color, 1, 1, 0);
    stars[Moon]    = new Planet(MOO_RADIUS, MOO_SPEED, SELFROTATE,MOO_DIS ,stars[Earth], rgba_color);

    SET_VALUE_3(rgba_color, 1, .5, .5);
    stars[Mars]    = new Planet(MAR_RADIUS, MAR_SPEED, SELFROTATE,MAR_DIS ,stars[Sun], rgba_color);

    SET_VALUE_3(rgba_color, 1, 1, .5);
    stars[Jupiter] = new Planet(JUP_RADIUS, JUP_SPEED, SELFROTATE,JUP_DIS ,stars[Sun], rgba_color);

    SET_VALUE_3(rgba_color, .5, 1, .5);
    stars[Saturn]  = new Planet(SAT_RADIUS, SAT_SPEED, SELFROTATE,SAT_DIS ,stars[Sun], rgba_color);

    SET_VALUE_3(rgba_color, .4, .4, .4);
    stars[Uranus]  = new Planet(URA_RADIUS, URA_SPEED, SELFROTATE,URA_DIS ,stars[Sun], rgba_color);

    SET_VALUE_3(rgba_color, .5, .5, 1);
    stars[Neptune] = new Planet(NEP_RADIUS, NEP_SPEED, SELFROTATE,NEP_DIS ,stars[Sun], rgba_color);

}

SolarSystem::~SolarSystem(){
	for(int i=0;i<STARS_NUM;i++){
		delete stars[i];
	}
}

void SolarSystem::onkeyboard(unsigned char key,int x,int y){
	switch(key){
		case 'w':centerY+=OFFSET;break;
		case 's':centerZ+=OFFSET;break;
		case 'S':centerZ-=OFFSET;break;
		case 'a':centerX-=OFFSET;break;
		case 'd':centerX+=OFFSET;break;
		case 'x':centerY-=OFFSET;break;
		case 'r':
				 viewX=0;
				 viewY=REST_Y;
				 viewZ=REST_Z;
				 centerX=centerY=centerZ=0;
				 upX=upY=0;
				 upZ=1;
				 break;
		default:break;
	}
}
