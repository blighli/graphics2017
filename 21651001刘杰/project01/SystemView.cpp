/*************************************************************************
	> File Name: SystemView.cpp
	> Author: liujie
	> Mail: vip727155384@gmail.com 
	> Created Time: Fri 17 Mar 2017 10:49:25 PM PDT
 ************************************************************************/

#include<iostream>
#include"SolarSystem.h"
#include"Star.h"
#include<GL/glut.h>
using namespace std;

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700
#define WINDOW_INIT_X 50
#define WINDOW_INIT_Y 50

SolarSystem isystem;

void onDisplay(void){
	isystem.onDisplay();
}
void onUpdate(void){
	isystem.onUpdate();
}
void onkeyboard(unsigned char key,int x,int y){
	isystem.onkeyboard(key,x,y);
}

int main(int argc,char** argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowPosition(WINDOW_INIT_X,WINDOW_INIT_Y);
	glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	glutCreateWindow("SolorSystem at zhongwenjie");
	glutDisplayFunc(onDisplay);
	glutIdleFunc(onUpdate);
	glutKeyboardFunc(onkeyboard);
	glutMainLoop();
	return 0;
}

