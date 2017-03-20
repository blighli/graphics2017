/**
 * Created by lixiaowen on 2017/3/14.
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "gl/glut.h"

double year = 0, day = 0, startYear = 0;
double moonday = 0, moonmonth = 0;
double secondyear = 0, secondday = 0;
double thirdyear = 0, thirdday = 0;
double thirdsatelliteyear = 0, thridsatelliteday = 0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);
	glRotatef((GLfloat)year, 0.0, 1.0, 1.0);

	glutWireSphere(1.31, 25, 16);
	glPopMatrix();

	glPushMatrix();
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
	glTranslatef(3.0, 0.0, 0.0);
	glColor3f(0.5, 0.5, 0.5);
	glRotatef((GLfloat)day, 1.0, 1.0, 1.0);
	glutWireSphere(0.5, 8, 6);

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glRotatef(70.0f, 1.0, 0.0, 0.0);
	glutSolidTorus(0.005, 1.0, 10, 64);
	glPopMatrix();


	glPushMatrix();
	glRotatef((GLfloat)moonmonth, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glColor3f(0.5, 0.6, 0.5);
	glutWireSphere(0.3, 10, 8);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glRotatef(90.0f, 1.0, 0.0, 0.0);
	glutSolidTorus(0.005, 3.0, 10, 64);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glRotatef(90.0f, 1.0, 0.0, 0.0);
	glutSolidTorus(0.005, 5.0, 10, 64);
	glPopMatrix();

	glPushMatrix();
	glRotatef((GLfloat)secondyear, 0.0, 1.0, 0.0);
	glTranslatef(5.0, 0.0, 0.0);
	glColor3f(0.7, 0.4, 0.2);
	glRotatef((GLfloat)secondday, 0.0, 1.0, 0.0);
	glutWireSphere(0.75, 10, 8);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glRotatef(90.0f, 1.0, 0.0, 0.0);
	glutSolidTorus(0.005, 7.0, 10, 64);
	glPopMatrix();

	glPushMatrix();

	glRotatef((GLfloat)thirdyear, 0.0, 1.0, 0.0);
	glTranslatef(7.0, 0.0, 0.0);
	glColor3f(0.5, 0.5, 1.0);

	glRotatef((GLfloat)thirdday, 0.0, 1.0, 0.0);
	glutWireSphere(0.45, 10, 8);

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);

	glRotatef(-(GLfloat)thirdday, 0.0, 1.0, 0.0);
	glRotatef(90.0f, 1.0, 1.0, 0.0);
	glutSolidTorus(0.005, 0.8, 10, 64);

	glRotatef(-(GLfloat)thirdsatelliteyear, 0.0, 0.0, 1.0);
	glTranslatef(0.8, 0.0, 0.0);
	glRotatef((GLfloat)thridsatelliteday, 0.0, 1.0, 0.0);
	glColor3f(0.2, 0.3, 0.4);
	glutWireSphere(0.2, 10, 8);
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//x, y, z, cx, cy, cz, ux, uy, uz
	gluLookAt(2.0,10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0);
}

void idle()
{
	moonday += 0.4;
	if (moonday >= 360){
		moonday -= 360;
	}

	moonmonth += 0.3;
	if (moonmonth >= 360){
		moonmonth -= 360;
	}

	day += 0.2;
	if (day >= 360){
		day -= 360;
	}

	year += 0.1;
	if (day >= 360){
		year -= 360;
	}

	startYear += 0.02;
	if (startYear >= 360){
		startYear -= 360;
	}

	secondday += 0.15;
	if (secondday >= 360){
		secondday -= 360;
	}

	secondyear += 0.03;
	if (secondyear >= 360){
		secondyear -= 360;
	}

	thirdday += 0.1;
	if (thirdday >= 360){
		thirdday -= 360;
	}

	thirdyear += 0.02;
	if (thirdyear >= 360){
		thirdyear -= 360;
	}

	thirdsatelliteyear += 0.25;
	if (thirdsatelliteyear >= 360){
		thirdsatelliteyear -= 360;
	}

	thridsatelliteday += 0.2;
	if (thridsatelliteday >= 360){
		thridsatelliteday -= 360;
	}

	glutPostRedisplay();
}

void initView(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(800, 650);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	initView();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}}