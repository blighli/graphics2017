#include "stdafx.h"
#include <gl/glut.h>
#include <stdio.h>
#include <Windows.h>
// 太阳、地球和月亮   
// 假设每个月都是12天   
// 一年12个月，共是360天   
static int day = 200; // day的变化：从0到359

void myDisplay(void)
{
	//glEnable不能写在glBegin和glEnd两个函数中间。
	glEnable(GL_DEPTH_TEST);//启用深度测试。根据坐标的远近自动隐藏被遮住的图形（材料）
	glDepthFunc(GL_EQUAL);//指定深度缓冲比较值：如果输入的深度值等于参考值，则通过
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲以及深度缓冲

	glMatrixMode(GL_PROJECTION);//对投影矩阵应用随后的矩阵操作
	glLoadIdentity();//将当前的用户坐标系的原点移到了屏幕中心
	gluPerspective(75, 1, 1, 400000000);//指定了观察的视景体，参数为：视野的角度，视景体的宽高比，近平面，远平面
	glMatrixMode(GL_MODELVIEW);//对模型视景矩阵堆栈应用随后的矩阵操作
	glLoadIdentity();
	gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);//定义一个视图矩阵，参数为：
	//第一组eyex, eyey,eyez 相机在世界坐标的位置
	//第二组centerx,centery,centerz 相机镜头对准的物体在世界坐标的位置
	//第三组upx,upy,upz 相机向上的方向在世界坐标中的方向

	const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

	// 绘制红色的“太阳”
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glRotated(a, 0, 0, -1);//不明白这里为什么用const变量还可以看到自转。
	glutSolidSphere(69600000, 20, 20);//渲染一个球体。球体球心位于原点。参数为：半径、以Z轴上线段为直径分布的圆周线的条数、围绕在Z轴周围的线的条数
	glPopMatrix();

	// 绘制蓝色的“地球”
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(day/360.0*360.0, 0.0f, 0.0f, -1.0f);//旋转矩阵。右手规则
	glTranslatef(150000000, 0.0f, 0.0f);//平移矩阵。注意不是物体，而是绘制坐标系。glTranslatef、glRotatef都是。
	glRotated(a, 0, 0, -1);
	glutSolidSphere(15945000, 20, 20);
	
	// 绘制黄色的“月亮”
	glColor3f(1.0f, 1.0f, 0.0f);
	glRotatef(day/30.0*360.0 - day/360.0*360.0, 0.0f, 0.0f, -1.0f);
	glTranslatef(38000000, 0.0f, 0.0f);
	glRotated(a, 0, 0, -1);
	glutSolidSphere(4345000, 20, 20);
	glPopMatrix();

	glFlush();//强制刷新缓冲，保证绘图命令将被执行，而不是存储在缓冲区
	glutSwapBuffers();//双缓冲区机制，交换缓冲区
}  

void myIdle(void)
{
	++day;
	Sleep(50);
	if( day >= 360 )
		day = 0;
	myDisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

int main(int argc, char *argv[])
{  
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//设置初始显示模式。 RGB 颜色模式和双缓存的窗口
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("太阳，地球和月亮");

	glutDisplayFunc(&myDisplay);//注册一个绘图函数
	glutIdleFunc(&myIdle);//设置全局的回调函数，当没有窗口事件到达时，GLUT程序功能可以执行后台处理任务或连续动画。

	glEnable(GL_CULL_FACE);//遮挡
    glCullFace(GL_BACK);//背面剔除

    glEnable(GL_LIGHT0);//光源0
    glEnable(GL_COLOR_MATERIAL);//反射材质
    glEnable(GL_LIGHTING);//光照

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);//环境光
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);//散射光
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//镜面反射光
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);//光源位置

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);//材质的环境颜色
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);//材质的散射颜色
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);//材质的镜面反射颜色
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);//镜面反射指数

	glutMainLoop();//进入GLUT事件处理循环，让所有的与“事件”有关的函数调用无限循环。

	return 0;
}