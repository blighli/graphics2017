#include <windows.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "src/SOIL.h"
#include <string.h>
#include <string>

static int slices = 20;
static int stacks = 20;

static int day = 0;

// textures
GLuint moon_textureId;
GLuint sun_textureId;
GLuint earth_textureId;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3*ar, 3*ar, -3.0*ar, 3.0*ar,0.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}


static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(5.0, 0.0, -1.0, 0.0, 0.0, -0.5, 1.0, 0.0, 0.0);


    glPushMatrix();

        glTranslatef(0, 0, -1);

        GLUquadric *sphere = NULL;
//        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sun_textureId);
        sphere = gluNewQuadric();
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gluQuadricTexture(sphere, GL_TRUE);
//        gluQuadricNormals(sphere, GLU_SMOOTH);
        gluSphere(sphere, 1, slices, stacks);
//        glutSolidSphere(1,slices,stacks);
        glBindTexture(GL_TEXTURE_2D, 0);

        // 蓝色
//        glColor3d(0.02,0.44,0.78);
        glRotatef(day/360.0*360.0,0,0,-1);
        // 根据日地相对距离进行偏移
        glTranslatef(-3, 0, 0);
        // 地球相对半径
//        glutSolidSphere(0.5,slices,stacks);
        GLUquadric *earthsphere = NULL;
//        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, earth_textureId);
        earthsphere = gluNewQuadric();
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gluQuadricTexture(earthsphere, GL_TRUE);
//        gluQuadricNormals(earthsphere, GLU_SMOOTH);
        gluSphere(earthsphere, 0.5, slices, stacks);
        glBindTexture(GL_TEXTURE_2D, 0);

        glRotatef(day/30.0*360.0 - day/360.0*360.0,0,0,-1);
        // 根据月地相对距离进行偏移
        glTranslatef(-1, 0, 0);
        // 月球相对半径
//        glutSolidSphere(0.25,slices,stacks);
        GLUquadric *moonsphere = NULL;
        moonsphere = gluNewQuadric();
        glBindTexture(GL_TEXTURE_2D, moon_textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gluQuadricTexture(moonsphere, GL_TRUE);
        gluQuadricNormals(moonsphere, GLU_SMOOTH);
        gluSphere(moonsphere, 0.25, slices, stacks);


    glPopMatrix();

    glutSwapBuffers();
}

static void idle(void)
{
    ++day;
	Sleep(50);
	if( day >= 360 )
		day = 0;
    glutPostRedisplay();

}

void loadTexture() {

    sun_textureId = SOIL_load_OGL_texture
	(
		"D:\\WorkSpace\\CodeBlocks\\PlanetModel\\tga\\sunmap.tga",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	if (!sun_textureId)
    {
        printf("soil failed to load texture\n");
        exit(0);
    }

	earth_textureId = SOIL_load_OGL_texture
	(
		"D:\\WorkSpace\\CodeBlocks\\PlanetModel\\tga\\earthmap.tga",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y
	);
	if (!earth_textureId)
    {
        printf("soil failed to load texture\n");
        exit(0);
    }

	moon_textureId = SOIL_load_OGL_texture
	(
		"D:\\WorkSpace\\CodeBlocks\\PlanetModel\\tga\\moonmap.tga",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y
	);
	if (!moon_textureId)
    {
        printf("soil failed to load texture\n");
        exit(0);
    }

}

//const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
//const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 0.0f, -1.0f, 1.0f };
const GLfloat light_direction[] = { 0.0f, 0.0f, -1.0f };

//const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
//const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
//const GLfloat mat_specular[]   = { 0.0f, 0.0f, 0.0f, 1.0f };
//const GLfloat high_shininess[] = { 20.0f };


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    loadTexture();
    glEnable(GL_TEXTURE_2D);

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

//    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light_direction);

//    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
//    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
//    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
