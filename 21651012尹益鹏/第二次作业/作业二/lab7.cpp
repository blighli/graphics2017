
#include "stdafx.h"

# include < GL/glut.h > 
GLfloat light_position [ ] = { 1.0, 1.0, 1.0, 0.0 }; 
GLfloat light_position1 [ ] = { 0.0, 0.0, 1.0, 0.0 }; 
GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_close[]={0.0,0.0,0.0,1.0};
GLfloat mat_shininess[] = { 80.0 }; 
/* ��ʼ���������ԡ���Դ���ԡ�����ģ�ͣ�����Ȼ������� */ 
void init(void) 
{ 
	glClearColor ( 0.5, 0.0, 1.0, 0.0 ); //���ñ���ɫΪ��ɫ
	glShadeModel ( GL_SMOOTH ); 
	//glMaterialfv(GL_FRONT,GL_AMBIENT,mat_specular);
	 
	 glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);//����ָ��
	

	glLightfv ( GL_LIGHT0, GL_POSITION, light_position); 

	glEnable (GL_LIGHTING); 
	glEnable (GL_LIGHT0); 
	glEnable (GL_DEPTH_TEST); 
} 
/*���� GLUT ����,����һ����*/ 
void display ( void ) 
{ 
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glutSolidSphere (1.0, 240, 250);    //�뾶Ϊ 1,40 ��γ��,50 ������
	glFlush (); 
} 
/* ���� GLUT �� reshape ������w��h �ֱ������ͼ�εĴ��ڵĿ�͸�*/ 
void reshape (int w, int h) 
{ 
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION); 
	glLoadIdentity ( ); 
	if (w <= h) 
		glOrtho (-1.5, 1.5, -1.5 * ( GLfloat ) h / ( GLfloat ) w, 
		1.5* ( GLfloat ) h / ( GLfloat ) w, -10.0, 10.0 ); //����ƽ���Ӿ���
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
	if(key=='1'){re();glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);glutPostRedisplay();}//������
	if(key=='2'){re();glMaterialfv(GL_FRONT,GL_SPECULAR, mat_diffuse);glutPostRedisplay();}//���淴��
	if(key=='3'){re();glMaterialfv(GL_FRONT,GL_AMBIENT, mat_diffuse);glutPostRedisplay();}//������
	if(key=='4'){re();glMaterialfv(GL_FRONT,GL_AMBIENT, mat_diffuse);glMaterialfv(GL_FRONT,GL_SPECULAR, mat_diffuse);glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);glutPostRedisplay();}//������
	if(key=='5'){re();}//���ֹ�
	if(key=='6'){glLightfv ( GL_LIGHT1, GL_POSITION, light_position1); glEnable (GL_LIGHT1); };
}
int main(int argc, char** argv) 
{ 
	glutInit (&argc, argv);     // GLUT ������ʼ��
	glutInitDisplayMode (GLUT_SINGLE |GLUT_RGB |GLUT_DEPTH); // ��ʾģʽ��ʼ��
	glutInitWindowSize (300, 300);       // ���崰�ڴ�С
	glutInitWindowPosition (100, 100);   // ���崰��λ��  
	glutCreateWindow ( argv [ 0 ] );   // ��ʾ����,���ڱ���Ϊִ�к�����
	init( );
	glutDisplayFunc ( display ); 	// ע�� OpenGL ��ͼ����(һ������ĵ��÷�ʽ,��ͬ) 
	glutReshapeFunc ( reshape );   // ע�ᴰ�ڴ�С�ı�ʱ����Ӧ����
	glutKeyboardFunc(keyboard);
	
	glutMainLoop( );      // ���� GLUT ��Ϣѭ������ʼִ�г���
	return 0; 
} 