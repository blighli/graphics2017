#include "OpenGLDemo.h"
#include <stdlib.h>
#include <gl/glut.h>
#include "LoadFileDlg.h"
#include "glm.h"
#include "trackball.h"
int window_width  = 800;
int window_height = 600;
GLMmodel* pModel = NULL;
double pModelViewMatrix[16];
bool bLeftBntDown = false;
int OldX = 0;
int OldY = 0;

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Demo");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)window_width/(float)window_height, 0.01f, 200.0f);
	glClearColor(0, 0, 0, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
    glEnable( GL_LIGHTING );
    glEnable( GL_NORMALIZE );
    glShadeModel( GL_SMOOTH );
    
	GLfloat light_model_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_direction[] = {0.0f, 0.0f, 10.0f, 0.0f};
	GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_model_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable( GL_LIGHT0 );

	PopFileInitialize( NULL );
}
void display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslated( 0.0, 0.0, -5.0 );
	glMultMatrixd( pModelViewMatrix );

	if ( pModel )
	{
		glmDraw( pModel, GLM_FLAT );
	}
	
	glutSwapBuffers();
}
void reshape(int w, int h)
{
	window_width  = w;
	window_height = h;
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)window_width/(float)window_height, 0.01f, 200.0f);
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	char FileName[128] = "";
	char TitleName[128]= "";
	float modelCenter[] = {0.0f, 0.0f, 0.0f};
	switch ( key )
	{
	case 'o':
	case 'O':
		PopFileOpenDlg( NULL, FileName, TitleName );
		if ( pModel )
		{
			glmDelete( pModel );
			pModel = NULL;
		}
		pModel = glmReadOBJ( FileName );
		glmFacetNormals( pModel );
		glmUnitize( pModel, modelCenter );
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );	

		break;

	case '+':
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glLoadMatrixd( pModelViewMatrix );
		glScaled( 1.05, 1.05, 1.05 );
		glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
		break;

	case '-':
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glLoadMatrixd( pModelViewMatrix );
		glScaled( 0.95, 0.95, 0.95 );
		glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
		break;

	default:
		break;
	}

	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y)
{
	if ( pModel )
	{
		if ( state==GLUT_DOWN && button==GLUT_LEFT_BUTTON )
		{
			OldX = x;
			OldY = y;
			bLeftBntDown = true;
		}
		else if ( state==GLUT_DOWN && button==GLUT_LEFT_BUTTON )
		{
			bLeftBntDown = false;
		}
	}
}
void motion(int x, int y)
{
	if ( bLeftBntDown && pModel )
	{
		float fOldX =  2.0f*OldX/(float)window_width  - 1.0f;
		float fOldY = -2.0f*OldY/(float)window_height + 1.0f;
		float fNewX =  2.0f*x/(float)window_width  - 1.0f;
		float fNewY = -2.0f*y/(float)window_height + 1.0f;

		double pMatrix[16];
		trackball_opengl_matrix( pMatrix, fOldX, fOldY, fNewX, fNewY);

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glLoadMatrixd( pMatrix );
		glMultMatrixd( pModelViewMatrix );
		glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );

		OldX = x;
		OldY = y;
		glutPostRedisplay();
	}
}
void idle(void)
{
	glutPostRedisplay();
}
