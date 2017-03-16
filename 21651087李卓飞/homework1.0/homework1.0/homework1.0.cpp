#include<stdio.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include<gl/GLUT.H>


static float year = 0, day = 0;


void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);//设定着色模式，使用图元中某个顶点的颜色来渲染整个图元。
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	glPushMatrix();
	gluLookAt(0.0, 0.0, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


	//绘制太阳
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
	glutWireSphere(0.8, 20, 20);



	//绘制地球
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
	glutWireSphere(0.2, 20, 20);


	//绘制月球
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
	glTranslatef(0.4, 0.0, 0.0);
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
	glutWireSphere(0.1, 15, 15);

	//glutWireCube(2.0);
	glPopMatrix();
	glutSwapBuffers();


}


void spinDisplay(void)
{
	year = year + 0.003;
	if (year > 360.0)
	{
		year = year - 360.0;
	}
	day = day + 0.006;
	if (day > 360.0)
	{
		day = day - 360.0;
	}


	glutPostRedisplay();
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);//函数作用：用一个视景体截取图像


											 //glFrustum(-1.0,1.0,-1.0,1.0,-1.0,1.0);
	glMatrixMode(GL_PROJECTION);//mode 指定哪一个矩阵堆栈是下一个矩阵操作的目标,参数表示接下来要进行投影相关的操作


	glLoadIdentity();//设置当前变化矩阵为单位矩阵；单位矩阵就是对角线上都是1，其余元素皆为0的矩阵。然后复位


					 //glOrtho(-3.0,3.0,-3.0,3.0,-3.0,3.0);
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20);
	glMatrixMode(GL_MODELVIEW);//进行视景相关的操作


							   //glLoadIdentity();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Homework 01");
	glutInitWindowPosition(50, 50);
	//glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(spinDisplay);
	glutMainLoop();
	return 0;
}