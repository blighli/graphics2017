
#include "stdafx.h"

# include < GL/glut.h > 
GLfloat light_position [ ] = { 1.0, 1.0, 1.0, 0.0 }; 
GLfloat light_position1 [ ] = { 0.0, 0.0, 1.0, 0.0 }; 
GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_close[]={0.0,0.0,0.0,1.0};
GLfloat mat_shininess[] = { 80.0 }; 
/* 初始化材料属性、光源属性、光照模型，打开深度缓冲区等 */ 
void init(void) 
{ 
	glClearColor ( 0.5, 0.0, 1.0, 0.0 ); //设置背景色为蓝色
	glShadeModel ( GL_SMOOTH ); 
	//glMaterialfv(GL_FRONT,GL_AMBIENT,mat_specular);
	 
	 glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);//反射指数
	

	glLightfv ( GL_LIGHT0, GL_POSITION, light_position); 

	glEnable (GL_LIGHTING); 
	glEnable (GL_LIGHT0); 
	glEnable (GL_DEPTH_TEST); 
} 
/*调用 GLUT 函数,绘制一个球*/ 
void display ( void ) 
{ 
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glutSolidSphere (1.0, 240, 250);    //半径为 1,40 条纬线,50 条经线
	glFlush (); 
} 
/* 定义 GLUT 的 reshape 函数，w、h 分别是输出图形的窗口的宽和高*/ 
void reshape (int w, int h) 
{ 
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION); 
	glLoadIdentity ( ); 
	if (w <= h) 
		glOrtho (-1.5, 1.5, -1.5 * ( GLfloat ) h / ( GLfloat ) w, 
		1.5* ( GLfloat ) h / ( GLfloat ) w, -10.0, 10.0 ); //创建平行视景体
	else 
		glOrtho (-1.5 * ( GLfloat ) w / ( GLfloat ) h,1.5 * ( GLfloat ) 
		w/( GLfloat ) h, -1.5, 1.5, -10.0, 10.0); 
	glMatrixMode ( GL_MODELVIEW ); 
	glLoadIdentity ( ) ; 
} 
void re(void)
{
	glMaterialfv(GL_FRONT,GL_SPECULAR, mat_close);
	glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_close);
	glMaterialfv(GL_FRONT,GL_AMBIENT, mat_close);
}
void keyboard(unsigned char key, int x, int y)
{
	if(key=='1'){re();glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);glutPostRedisplay();}//漫反射
	if(key=='2'){re();glMaterialfv(GL_FRONT,GL_SPECULAR, mat_diffuse);glutPostRedisplay();}//镜面反射
	if(key=='3'){re();glMaterialfv(GL_FRONT,GL_AMBIENT, mat_diffuse);glutPostRedisplay();}//环境光
	if(key=='4'){re();glMaterialfv(GL_FRONT,GL_AMBIENT, mat_diffuse);glMaterialfv(GL_FRONT,GL_SPECULAR, mat_diffuse);glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);glutPostRedisplay();}//环境光
	if(key=='5'){re();}//三种光
	if(key=='6'){glLightfv ( GL_LIGHT1, GL_POSITION, light_position1); glEnable (GL_LIGHT1); };
}
int main(int argc, char** argv) 
{ 
	glutInit (&argc, argv);     // GLUT 环境初始化
	glutInitDisplayMode (GLUT_SINGLE |GLUT_RGB |GLUT_DEPTH); // 显示模式初始化
	glutInitWindowSize (300, 300);       // 定义窗口大小
	glutInitWindowPosition (100, 100);   // 定义窗口位置  
	glutCreateWindow ( argv [ 0 ] );   // 显示窗口,窗口标题为执行函数名
	init( );
	glutDisplayFunc ( display ); 	// 注册 OpenGL 绘图函数(一种特殊的调用方式,下同) 
	glutReshapeFunc ( reshape );   // 注册窗口大小改变时的响应函数
	glutKeyboardFunc(keyboard);
	
	glutMainLoop( );      // 进入 GLUT 消息循环，开始执行程序
	return 0; 
} 