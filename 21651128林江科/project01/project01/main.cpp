#include <GL/glut.h>

double angle = 0.0f;
const double speed = 0.01f;

void display(void) {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0f, 0.0f, 0.0f);		//Sun
	glutSolidSphere(0.1f, 50, 50);

	glColor3f(0.0f, 0.0f, 1.0f);		//Earth
	glRotatef(angle, 0.0f, 0.0f, -1.0f);
	glTranslatef(0.5f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 20, 20);

	glColor3f(1.0f, 1.0f, 0.0f);		//Moon
	glRotatef(angle / 30.0 * 365.0 - angle, 0.0f, 0.0f, -1.0f);
	glTranslatef(0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.01f, 10, 10);

	glFlush();
}

void idle()
{
	angle += speed;
	if (angle > 360.0f)
		angle -= 360.0f;
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Simplified Solar System");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}