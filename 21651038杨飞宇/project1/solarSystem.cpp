#include "GLUT/glut.h"
#include <math.h>


static int day = 100;


void myDisplay(void){
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 1, 1, 400000000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 300000000, 300000000, 0, 0, 0, 0, 0, 1);
    
    //light0
    {
        GLfloat sl_positon[] = {0,0,0,1};
        GLfloat sl_ambient[] = {0,0,0,1};
        GLfloat sl_diffuse[] = {1,1,1,1};
        GLfloat sl_specular[] = {1,1,1,1};
        glLightfv(GL_LIGHT0, GL_POSITION, sl_positon);
        glLightfv(GL_LIGHT0, GL_AMBIENT, sl_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sl_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, sl_specular);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
    }
    
    //light1
//    {
//        GLfloat sl_positon[] = {0,-20000000,-20000000,1};
//        GLfloat sl_ambient[] = {0,0,0,1};
//        GLfloat sl_diffuse[] = {1,1,1,1};
//        GLfloat sl_specular[] = {1,1,1,1};
//        glLightfv(GL_LIGHT1, GL_POSITION, sl_positon);
//        glLightfv(GL_LIGHT1, GL_AMBIENT, sl_ambient);
//        glLightfv(GL_LIGHT1, GL_DIFFUSE, sl_diffuse);
//        glLightfv(GL_LIGHT1, GL_SPECULAR, sl_specular);
//        glEnable(GL_LIGHT1);
//        glEnable(GL_LIGHTING);
//        glEnable(GL_DEPTH_TEST);
//    }
    
    //sun
    {
        GLfloat sun_ambient[] = {0,0,0,1};
        GLfloat sun_diffuse[] = {0,0,0,1};
        GLfloat sun_specular[] = {0,0,0,1};
        GLfloat sun_emission[] = {0.5,0,0,1};
        GLfloat sun_shininess = 0.f;
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, sun_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, sun_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);
        glMaterialf(GL_FRONT, GL_SHININESS, sun_shininess);
        
        //glColor3f(1.f, 0.f, 0.f);
        glutSolidSphere(69600000, 20, 20);
        
    }
    
    //earth
    {
        GLfloat earth_ambient[] = {0,0,0.5,1};
        GLfloat earth_diffuse[] = {0,0,0.5,1};
        GLfloat earth_specular[] = {0,0,1,1};
        GLfloat earth_emission[] = {0,0,0,1};
        GLfloat earth_shininess = 135.f;
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, earth_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, earth_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, earth_emission);
        glMaterialf(GL_FRONT, GL_SHININESS, earth_shininess);
        
        
        //glColor3f(0, 0, 1);
        glRotatef(day, 0, 0, 1);
        

        glTranslatef(150000000, 0, 0);
        glRotatef(90, 0, 1, 0);
        glutSolidSphere(26000000, 20, 20);
        
    }
    
    //moon
    {
        GLfloat moon_ambient[] = {0,0,0.5,1};
        GLfloat moon_diffuse[] = {0,0,0.5,1};
        GLfloat moon_specular[] = {0,0.6,0.8,0.3};
        GLfloat moon_emission[] = {0,0,0,1};
        GLfloat moon_shininess = 150.f;
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, moon_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, moon_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, moon_emission);
        glMaterialf(GL_FRONT, GL_SHININESS, moon_shininess);
        
        
        //glColor3f(0, 0, 1);
        glRotatef(day*12, 0, 0, 1);
        
        
        glTranslatef(38000000, 0, 0);
        glutSolidSphere(9000000, 20, 20);
        
    }
    
    
    
    glFlush();
    glutSwapBuffers();
    
    
}

void move(){
    ++day;
    if (day >= 365) {
        day = 0;
    }
    myDisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB |GLUT_SINGLE);
    glutInitWindowSize (400, 400);
    glutInitWindowPosition (100, 100);
    glutCreateWindow( "Triangles Example" );
    glutDisplayFunc( myDisplay );
    glutIdleFunc(move);
    
    glutMainLoop();
    
    return 1;
}
