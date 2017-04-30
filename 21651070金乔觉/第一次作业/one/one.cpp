// one.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<glut.h>

GLfloat year1 = 1.0f, day1 = 1.0f,year2=0.1f,day2=0.1f;

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);  //清空颜色缓冲区
	glColor3f(1, 1, 1);   //重置颜色，白色
	glPushMatrix();  //压栈
	glRotatef((GLfloat)year1, 0, 1, 0);
	glutWireSphere(1, 20, 16);   //绘制太阳

	glRotatef((GLfloat)year1, 0, 1, 0);  //沿y轴旋转 
	glTranslatef(3, 0, 0); //将场景中的物体沿x轴正方向移动2个单位长
	glRotatef((GLfloat)day1, 0, 1, 0);  //沿y轴旋转
	glutWireSphere(0.4, 10, 8);   //绘制行星

	glRotatef((GLfloat)year2, 0, 1, 0);  //沿y轴旋转 
	glTranslatef(1, 0, 0); //将场景中的物体沿x轴正方向移动2个单位长
	glRotatef((GLfloat)day2, 0, 1, 0);  //沿y轴旋转
	glutWireSphere(0.1, 10, 8);

	glPopMatrix(); //出栈
	glutSwapBuffers();
	glFlush();   //刷新窗口以显示当前绘制图形
}


void init(void)
{
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_FLAT); //选择平面明暗模式或光滑明暗模式
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);   //设置机口
	glMatrixMode(GL_PROJECTION);  //指定哪一个矩阵是当前矩阵
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 20);   //创建透视投影矩阵(fovy,aspect,zNear,zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 5, 5, 0, 0, 0, 0, 1, 0);
}


void myAnimation() {
	year1+=0.001;
	day1+=0.01;
	year2 += 0.03;
	day2 += 0.02;
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv); //固定格式
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);   //缓存模式
	glutInitWindowSize(600, 600);    //显示框的大小
	glutInitWindowPosition(100, 100); //确定显示框左上角的位置
	glutCreateWindow("第1次作业");
	init(); // 初始化
	glutDisplayFunc(display); // 执行显示
	glutReshapeFunc(reshape);
	glutIdleFunc(myAnimation);
    glutMainLoop(); //进人GLUT事件处理循环
	return 0;
}

