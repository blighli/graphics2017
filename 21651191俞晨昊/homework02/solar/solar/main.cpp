//
//  main.cpp
//  solar
//
//  Created by 俞晨昊 on 2017/3/16.
//  Copyright © 2017年 俞晨昊. All rights reserved.
//
#include <GLUT/GLUT.h>
#include<math.h>
#include<iostream>

#include </usr/local/include/SOIL.h>
using namespace std;


#define PI 3.14159
void Rotate() ;

static float year = 0, day = 0,month=0;
float light_angle=0;
float light_radius=8.0;
double x=0,y=0;
double aix_x=0.0,aix_y=0.5,aix_z=0.5;


GLdouble angle = 100.0;
string pics[3]{"sun", "earth", "moon"};
GLuint textures[3];
void addTexture(int index) {
    string filePath = "/Users/yuchenhao/Documents/objective－c/OpenGL/solar/textures" + pics[index] + ".jpg";
    glGenTextures(1, &textures[index]);
    glBindTexture(GL_TEXTURE_2D, textures[index]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height;
    unsigned char *image = SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    //	glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}
void lPosition()
{
    float y,z;
    y=light_radius*cos(light_angle);
    z=light_radius*sin(light_angle);
    float light_position[] = { 3.0,y,z, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}


void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    lPosition();
    glShadeModel (GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
}




void sun_material()                               //设置太阳材质，以下同
{
    GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat lmodel_ambient[]={1.0,0.0,0.0,1.0};        //太阳颜色为红色
    GLfloat white_light[]={1.0, 1.0,1.0, 1.0};
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}



void esrth_material()
{
    GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat lmodel_ambient[]={0.1,0.2,0.6,1.0};
    GLfloat white_light[]={1.0, 1.0,1.0, 1.0};
    
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}




void moon_material()
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat lmodel_ambient[]={1.0,1.0,0.0,1.0};
    GLfloat white_light[]={1.0, 1.0,1.0, 1.0};
    
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}





void sun()                  //绘制太阳
{
    glPushMatrix();
    sun_material();
    glTranslatef (-15,0,0);
    glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);   //太阳自转
    glutSolidSphere(10, 200, 200);                //画球体
    glPopMatrix();
}



void earth()
{
    glPushMatrix();
    esrth_material();
    glTranslatef (-15,0,0);
    glRotatef ((GLfloat) year, aix_x, aix_y, aix_z);
    glTranslatef (15,0,0);
    glTranslatef (2,0,0);
    glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
    glTranslatef (-2,0,0);
    glTranslatef (2,0,0);
    //	glutSolidSphere(1.0, 20, 16);
    GLUquadricObj *sphere = gluNewQuadric();
    gluQuadricOrientation(sphere, GLU_OUTSIDE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluQuadricTexture(sphere, GL_TRUE);
    gluSphere(sphere, 1.0, 20, 16);
    gluDeleteQuadric(sphere);
    
    addTexture(2);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glEnable(GL_TEXTURE_2D);
    //	set_moon();
    glRotatef(month, 0.0, 0.0, 1.0);
    glTranslatef(2, 0.0, 0.0);
    //	glutSolidSphere(0.3, 10, 10);
    GLUquadricObj *sphere1 = gluNewQuadric();
    gluQuadricOrientation(sphere1, GLU_OUTSIDE);
    gluQuadricNormals(sphere1, GLU_SMOOTH);
    gluQuadricTexture(sphere1, GL_TRUE);
    gluSphere(sphere1, 0.3, 10, 10);
    gluDeleteQuadric(sphere1);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}




void display(void)
{
    lPosition();
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    sun();
   
    earth();
    
    Rotate();
    
    glutSwapBuffers();
}


void myidle()
{
    day+=10.0;
    if (day>=360)
        day=day-360;
    
    
    glutPostRedisplay();
}


void Rotate()          //设置行星的公转周期
{
    
    //  year=(year+0.8)%360;
    year+=0.08;
    if(year>=360)
        year-=360;
   
    glutPostRedisplay();
    month+=0.03;
    if(month>=360)
        month-=360;
}


void mykeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'U':
        case 'u':
            aix_y-=0.01;
            aix_z+=0.01;
            break;
        case 'D':
        case 'd':
            aix_y+=0.01;
            aix_z-=0.01;
            break;
            
            
        case 'W':
        case 'w':
            angle-=10.0;
            break;
        case 'S':
        case 's':
            angle+=10.0;
            break;
    }
    glutPostRedisplay();
    
}




void reshape (int w, int h)
{ 
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);                
    glMatrixMode (GL_PROJECTION);                               
    glLoadIdentity ();    
    
    gluPerspective(angle, (GLfloat) w/(GLfloat) h, 1.0, 200.0); 
    
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    glTranslatef(0.0,0.0,-50.0);                               
} 



int main(int argc, char** argv) 
{ 
    cout<<argc<<endl;
    cout<<&argc<<endl;
    
    
    for(int i=0;i<argc;i++)
    {
        cout<<*argv[i]<<endl;
    }
    
    
    
    
    glutInit(&argc, argv);                                      
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   
    glutInitWindowSize (800, 600);                              
    glutInitWindowPosition (100, 100);                          
    glutCreateWindow ("solar system");                          
    init (); 
    glutDisplayFunc(display);          
    glutReshapeFunc(reshape);       
    glutIdleFunc(myidle);   
    
    glutKeyboardFunc(mykeyboard);   
    
    
    glutMainLoop();                 
    return 0; 
}
