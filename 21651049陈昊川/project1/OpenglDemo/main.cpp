#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <time.h>
const int n = 50;
const GLfloat R = 150000.0f;
const GLfloat Pi = 3.1415926536f;
static int day = 300;
void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
void Delay(int time)
{
	clock_t now = clock();
	while (clock() - now < time);
}

void display()
{

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1, 1, 400000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, -200000, 200000, 0, 0, 0, 0, 0, 1);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(69600, 20, 20);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; i++)
		glVertex3f(R *cos(2 * Pi / n*i), R *sin(2 * Pi / n*i), 0);
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(day / 360.0*360.0, 0.0f, 0.0f, -1.0f);
	glTranslatef(150000, 0.0f, 0.0f);
	glutSolidSphere(15945, 20, 20);
	glColor3f(1.0f, 1.0f, 0.0f);
	glRotatef(day / 30.0*360.0 - day / 360.0*360.0, 0.0f, 0.0f, -1.0f);
	glTranslatef(38000, 0.0f, 0.0f);
	glutSolidSphere(4345, 20, 20);
	glFlush();
	glutSwapBuffers();
}
void Idle(void)
{
	++day;
	if (day >= 360)
		day = 0;
	Delay(50);
	display();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(480, 480);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);

	glutCreateWindow("opengl");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutIdleFunc(Idle);

	glutMainLoop();
	return 0;
}