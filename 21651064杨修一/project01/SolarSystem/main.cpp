

#include<iostream>

#include "Camera.h"
#include <GL/freeglut.h>

using namespace std;

void rotate();

Camera camera(glm::vec3(0.0f, .0f, .0f));
GLfloat deltaTime = 0.0f;

static float year = 0, day = 0, month = 0;
float light_angle = 0;
float light_radius = 8.0;

double aix_x = 0.0, aix_y = 0.5, aix_z = 0.5;

GLdouble fovy = 60.0;

void set_light() {
	float y, z;
	y = light_radius * cos(light_angle);
	z = light_radius * sin(light_angle);
	float light_position[] = {3.0, y, z, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	set_light();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}

void set_sun() {
	GLfloat mat_specular[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	GLfloat model_ambient[] = {1.0, 0.0, 0.0, 1.0};

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, model_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
}

void material_earth() {
	GLfloat mat_specular[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	GLfloat model_ambient[] = {0.1, 0.2, 0.6, 1.0};

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, model_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
}

void set_moon() {
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	GLfloat model_ambient[] = {1.0, 1.0, 0.0, 1.0};

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, model_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
}

void draw_sun() {
	glPushMatrix();
	set_sun();
	glTranslatef(-15, 0, 0);
	glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
	glutSolidSphere(10, 200, 200);
	glPopMatrix();
}

void draw_earth() {
	glPushMatrix();
	material_earth();
	glTranslatef(-15, 0, 0);
	glRotatef((GLfloat) year, (GLfloat) aix_x, (GLfloat) aix_y, (GLfloat) aix_z);
	glTranslatef(15, 0, 0);
	glTranslatef(2, 0, 0);
	glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
	glTranslatef(-2, 0, 0);
	glTranslatef(2, 0, 0);
	glutSolidSphere(1.0, 20, 16);

	set_moon();
	glRotatef(month, 0.0, 0.0, 1.0);
	glTranslatef(2, 0.0, 0.0);
	glutSolidSphere(0.3, 10, 10);
	glPopMatrix();
}

void display() {
	set_light();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_sun();
	draw_earth();
	rotate();
	glutSwapBuffers();
}

void set_idle() {
	day += 10.0;
	if (day >= 360)
		day = day - 360;

	glutPostRedisplay();
}

void rotate() {
	year += 0.08;
	if (year >= 360)
		year -= 360;
	month += 0.007;
	if (month >= 360)
		month -= 360;
	glutPostRedisplay();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fovy, (GLfloat) w / (GLfloat) h, 1.0, 200.0);
	gluLookAt(camera.Position.x, camera.Position.y, camera.Position.z - 50.0, 0, 0, -1.0, 0, 1.0, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(camera.Position.x, camera.Position.y, camera.Position.z);
}

void key_callback(unsigned char key, int x, int y) {
	//keyboard
	if (key == 'w') {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (key == 'a') {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (key == 's') {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (key == 'd') {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	gluLookAt(camera.Position.x, camera.Position.y, camera.Position.z, 0, 0, -1.0, 0, 1.0, 0);
	glutPostRedisplay();
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("solar system");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(set_idle);

	glutKeyboardFunc(key_callback);

	glutMainLoop();
	return 0;
} 
