#include <cmath>
#include "gl/glut.h"
#include "gl/freeglut.h"

#include "LoadBMP.h"

void myinit(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);	//启用二维纹理
	

	GLfloat mat_shininess[] = { 10.0 };
    GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};//光源设置
	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//0号光源的位置
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);//使用lmodel_ambien
	
	LoadAllTextures();			//调入纹理
}

void myidle()
{
	day+=angle;
	glutPostRedisplay();

}

void mymouse(int button,int state,int x,int y)
{	
	if(state==GLUT_DOWN && button==GLUT_LEFT_BUTTON){
		if(first)
		{
			glutIdleFunc(myidle);
			first=!first;
		}
		else
		{
			glutIdleFunc(0);
			first=!first;
		}
	}	
}

void mykeyboard(unsigned char key, int x, int y) 
{
	if (key == 27)         
		exit(0); 
} 

void mydisplay(void)
{
	GLfloat  whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat	 lightPos[] = { 0.0f, 0.0f, 0.0f, 2.0f };

	year=day/365;
	month=day/30;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat mat_ambient1[]  = {1,0,0,1};
	GLfloat mat_emission[]  = {1,1,1,0};

	GLfloat mat_ambient2[]  = {0.4,0.4,0.8,1};
	GLfloat  no_emission[]  = {0,0,0,1};

	glPushMatrix();	
		glBindTexture(GL_TEXTURE_2D,sun->texID);
		glRotatef (month, 0.0, 1.0, 0.0);				//太阳自转
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient1);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glRotatef (90, -1, 0, 0);

		glDisable(GL_LIGHTING);
		gltDrawSphere(1.0, 60, 40);			//绘制太阳
		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

	glPopMatrix();

	glPushMatrix();
		glRotatef (year, 0.0, 1.0, 0.0);				//月亮与地球一起绕太阳转（地球公转）
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D,earth->texID);
			glTranslatef (3.0, 0.0, 0.0);
			glRotatef (month, 0.0, 1.0, 0.0);			//地球自转 
			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);
			glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
			glRotatef (90, -1, 0, 0);
			gltDrawSphere(0.4, 40, 40);		//绘制地球	
		glPopMatrix();

		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D,moon->texID);
			glTranslatef (3.0, 0.0, 0.0);
			glRotatef (60, -1, 1, 0);
			glRotatef (month, 0, 1.0, 0);				//月球绕地球转
			glTranslatef (0.6, 0.0, 0.0);
			glRotatef (90, -1, 0, 0);
			gltDrawSphere(0.07, 20, 20);	//绘制月球
		glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();   
}

void myreshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);				//指定视口大小
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
		
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1, 20);		//透视投影
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	//指定照相机的位置	
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1000, 600); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	myinit ();

	glutDisplayFunc(mydisplay); 
	glutReshapeFunc(myreshape);
	glutIdleFunc(myidle);
	glutMouseFunc(mymouse);
	glutKeyboardFunc(mykeyboard);

	glutMainLoop();
	return 0;
}

