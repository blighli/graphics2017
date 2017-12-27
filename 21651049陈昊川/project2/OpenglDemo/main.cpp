#include "TGATexture.h"
TextureImage texture[3];
const int n = 50;
const GLfloat R = 150000.0f;
const GLfloat Pi = 3.1415926536f;
static int day = 300;
void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if (!LoadTGA(&texture[0], "sunmap.tga")) return;
	if (!LoadTGA(&texture[1], "earthmap.tga")) return;
	//if (!LoadTGA(&texture[2], "moonmap.tga")) return;
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
}
void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks)
{
	GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat)iStacks;
	GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat)iSlices;
	GLfloat ds = 1.0f / (GLfloat)iSlices;
	GLfloat dt = 1.0f / (GLfloat)iStacks;
	GLfloat t = 1.0f;
	GLfloat s = 0.0f;
	GLint i, j;
	
		for (i = 0; i < iStacks; i++)
		{
		GLfloat rho = (GLfloat)i * drho;
		GLfloat srho = (GLfloat)(sin(rho));
		GLfloat crho = (GLfloat)(cos(rho));
		GLfloat srhodrho = (GLfloat)(sin(rho + drho));
		GLfloat crhodrho = (GLfloat)(cos(rho + drho));
		
			
			glBegin(GL_TRIANGLE_STRIP);
		s = 0.0f;
		for (j = 0; j <= iSlices; j++)
		{
			GLfloat theta = (j == iSlices) ? 0.0f : j * dtheta;
			GLfloat stheta = (GLfloat)(-sin(theta));
			GLfloat ctheta = (GLfloat)(cos(theta));
			
				GLfloat x = stheta * srho;
			GLfloat y = ctheta * srho;
			GLfloat z = crho;
			
				glTexCoord2f(s, t);
			glNormal3f(x, y, z);
			glVertex3f(x * fRadius, y * fRadius, z * fRadius);
			
				x = stheta * srhodrho;
			y = ctheta * srhodrho;
			z = crhodrho;
			glTexCoord2f(s, t - dt);
			s += ds;
			glNormal3f(x, y, z);
			glVertex3f(x * fRadius, y * fRadius, z * fRadius);
		}
		glEnd();
		
			t -= dt;
		}
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
	//glBindTexture(GL_TEXTURE_2D, suntexture[0].texID);
	//glutSolidSphere(69600, 20, 20);
	glBindTexture(GL_TEXTURE_2D, texture[0].texID);
	gltDrawSphere(69600, 20, 20);
	glColor3f(1.0f, 1.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(day / 360.0*360.0, 0.0f, 0.0f, -1.0f);
	glTranslatef(150000, 0.0f, 0.0f);
	//glutSolidSphere(15945, 20, 20);
	glBindTexture(GL_TEXTURE_2D, texture[1].texID);
	gltDrawSphere(15945, 20, 20);
	glColor3f(1.0f, 1.0f, 0.0f);
	glRotatef(day / 30.0*360.0 - day / 360.0*360.0, 0.0f, 0.0f, -1.0f);
	glTranslatef(38000, 0.0f, 0.0f);
	//glutSolidSphere(4345, 20, 20);
	//glBindTexture(GL_TEXTURE_2D, texture[2].texID);
	gltDrawSphere(4345, 40, 40);
	glFlush();
	glutSwapBuffers();
}
void SetupLights()
{
	GLfloat sun_light_position[] = { 69600.0f,20.0f,20.0f,1.0f };
	GLfloat sun_light_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
	GLfloat sun_light_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat sun_light_specular[] = { 1.0f,1.0f,1.0f,1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

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
	SetupLights();

	glutMainLoop();
	return 0;
}