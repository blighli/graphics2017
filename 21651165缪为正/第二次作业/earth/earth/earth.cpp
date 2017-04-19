// earth.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "TGATexture.h"

static int year = 0, day = 0;
static float spin = 0;
static GLint fogMode;
const int n = 100;
const GLfloat R = 1.0f;
const GLfloat Pi = 3.1415926536f;
TextureImage texture[3];
GLUquadricObj *quadPlanet;
// Lighting values
GLfloat whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
//光源位置
GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };


int init(void) {
	//设置灯光和坐标
	//启用深度测试，有遮挡的效果
	glEnable(GL_DEPTH_TEST);
	//逆时针为多边形正面
	glFrontFace(GL_CCW);
	//启用剔除效果，多边形背面不用光照处理
	glEnable(GL_CULL_FACE);
	//启动灯光效果
	glEnable(GL_LIGHTING);
	//设置灯泡0
	//环境光颜色
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	//散射光强度
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	//光源位置
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	//开灯
	glEnable(GL_LIGHT0);
	//使用颜色材质
	glEnable(GL_COLOR_MATERIAL);

	//设置材料反射属性， 以相同的强度反射环境光和散射光
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glClearColor(0.0, 0.0, 0.0, 1.0);  /* fog color */

	//纹理贴图
	if (!LoadTGA(&texture[0], "sunmap.tga"))
		return GL_FALSE;
	if (!LoadTGA(&texture[1], "earthmap.tga"))
		return GL_FALSE;
	//if (!LoadTGA(&texture[2], "moonmap.tga"))
	//	return GL_FALSE;
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	return GL_TRUE;
	
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix(); //记住自己的位置

	glPushMatrix();
	glRotatef(spin, 0.0, 1.0, 0.0);	//太阳自转，绕着一个向量以给定角度旋转（正的为逆时针）
									
	//贴图
	glBindTexture(GL_TEXTURE_2D, texture[0].texID);
	quadPlanet = gluNewQuadric();
	gluQuadricTexture(quadPlanet, GLU_TRUE);

	//glColor3ub(255, 255, 0);
	//glutSolidSphere(1.0, 20, 8);   /* 画太阳半径、 20经度、16纬度*/

	gluSphere(quadPlanet, 2, 20, 8);


	glPopMatrix();


	glRotatef(spin, 0.0, 1.0, 0.0);	//地球公转，绕着一个向量以给定角度旋转（正的为逆时针）
	//glRotatef(spin, 1.0, 0.0, 0.0); 
	glTranslatef(6.0, 0.0, 0.0);
	glRotatef(spin, 0.0, 1.0, 0.0);//自转

	//贴图
	glBindTexture(GL_TEXTURE_2D, texture[1].texID);
	quadPlanet = gluNewQuadric();
	gluQuadricTexture(quadPlanet, GLU_TRUE);

	//glColor3ub(0, 0, 255);//地球
	//glutSolidSphere(0.5, 16, 8);
	gluSphere(quadPlanet, 1, 16, 8);


	glRotatef(spin, 0.0, 1.0, 0.0);//月球公转
	glTranslatef(2.0, 0.0, 0.0);

	//贴图
	glBindTexture(GL_TEXTURE_2D, texture[0].texID);
	quadPlanet = gluNewQuadric();
	gluQuadricTexture(quadPlanet, GLU_TRUE);

	//glColor3ub(255, 255, 255);//月球
	glutSolidSphere(0.5, 16, 8);
	gluSphere(quadPlanet, 0.5, 16, 8);

	
	glPopMatrix();//回到原来的位置
	glutSwapBuffers();
}

void spinDisplay(void) {
	spin = spin + 0.05;
	if (spin > 360)
		spin = spin - 360;
	glutPostRedisplay();
}


void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("地球和月亮");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(spinDisplay);
	glutMainLoop();

	return 0;
}

