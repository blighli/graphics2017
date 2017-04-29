// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//


#include <GL/glut.h>
static int day = 0;

void Display()
{    
	//开启深度测试
	glEnable(GL_DEPTH_TEST);
	//清空颜色缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//使用投影矩阵
	glMatrixMode(GL_PROJECTION);
	//将当前的用户坐标系的原点移到了屏幕中心
	glLoadIdentity();

	//设置一个视锥体
	gluPerspective(85, 1, 1, 500000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//第一组数据为脑袋位置，第二组数据为眼睛看的物体的位置，第三组是头顶朝向的方向
	gluLookAt(0, -200000, 200000, 0, 0, 0, 0, 0, 1);

	//绘制太阳
	glColor3f(1, 0, 0);
	glutSolidSphere(69600, 20, 20);

	//绘制地球
	glColor3f(0, 0, 1);
	glRotatef(day, 0, 0, -1);
	glTranslatef(150000, 0, 0);
	glutSolidSphere(15945, 20, 20);

	//绘制月亮
	glColor3f(1, 1, 0);
	glRotatef(day / 30.0 * 360 - day, 0, 0, -1);
	glTranslatef(38000, 0, 0);
	glutSolidSphere(4345, 20, 20);

	//执行双缓冲交换
	glutSwapBuffers();

}


//用value区别是哪个定时器，在函数里改变和位置有关的变量，调用glutPostRedisplay()用来重绘
//最后再次调用glutTimerFunc，因为glut的定时器是调用一次才产生一次定时
void Timer(int value)
{
	day++;
	if (day > 365)
	{
		day = 0;
	}
	
	//标记当前窗口需要重新绘制
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 0);
}

int main(int argc, char **argv)
{   
	//对GLUT进行初始化，并处理所有命令行参数
	glutInit(&argc, argv);

	//指定使用RGBA模式和双缓冲窗口
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	//设置窗口创建时的宽高
	glutInitWindowSize(800, 800);
	//创建一个窗口
	glutCreateWindow("test");
	
	//回调函数，当窗口内容需要更新显示时，注册的回调函数会被执行
	glutDisplayFunc(Display);
	//定时器
	glutTimerFunc(50, Timer, 0);
	glutMainLoop();
	return 0;
}