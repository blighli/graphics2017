#include <GL/glut.h>
static int day = 0;

void MyDisplay(){    
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//设置一个视锥体
	gluPerspective(75, 1, 1, 400000);

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


void OnTimer(int value){
	day++;
	if (day > 365){
		day = 0;
	}
	
	glutPostRedisplay();
	glutTimerFunc(30, OnTimer, 0);
}

int main(int argc, char **argv){   
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowSize(400, 400);
	glutCreateWindow("homework");
	
	glutDisplayFunc(MyDisplay);
	glutTimerFunc(30, OnTimer, 0);
	glutMainLoop();
	return 0;
}
