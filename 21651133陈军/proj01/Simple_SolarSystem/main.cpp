//
//  main.cpp
//  Simple_SolarSystem
//
//  Created by Chanjun2016 on 2017/3/15.
//  Copyright © 2017年 Chanjun2016. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>

static const GLfloat earth_spin = 1.0;
static const GLfloat earth_revolution = 0.3;

static const GLfloat moon_spin = 0.8;
static const GLfloat moon_revolution = 0.8;

static const GLfloat sun_spin = 1.2;
static const GLfloat sun_revolution = 1.2;

static GLfloat angle_earth_spin = 0.0;
static GLfloat angle_earth_revolution = 0.0;

static GLfloat angle_moon_spin = 0.0;
static GLfloat angle_moon_revolution = 0.0;

static GLfloat angle_sun_spin = 0.0;
static GLfloat angle_sun_revolution = 0.0;

class screenPt{
public:
    GLint x,y;
    
public:
    screenPt(){
        x = y = 0;
    }
    
    void setCoords(GLint xCoordValue, GLint yCoordValue){
        x = xCoordValue;
        y = yCoordValue;
    }
    
    GLint getx() const{
        return x;
    }
    
    GLint gety() const{
        return y;
    }
    
    void incrementx(){
        ++x;
    }
    
    void decrementy(){
        --y;
    }
};

void init(){
    glClearColor(0.0,0.0,0.0,1.0);
    glShadeModel(GL_FLAT);
    gluOrtho2D(-300.0, 300.0, -300.0, 300.0);
}

void setPiexl(GLint xCoord, GLint yCoord){
    glBegin(GL_POINTS);
    glVertex2i(xCoord, yCoord);
    glEnd();
}

void circlePlotPoints (screenPt circCtr, screenPt circPt)
{
    glColor3f(1.0, 0.0, 0.0);
    setPiexl (circCtr.x + circPt.x, circCtr.y + circPt.y);
    setPiexl (circCtr.x - circPt.x, circCtr.y + circPt.y);
    setPiexl (circCtr.x + circPt.x, circCtr.y - circPt.y);
    setPiexl (circCtr.x - circPt.x, circCtr.y - circPt.y);
    glColor3f(0.5, 0.5, 0.0);
    setPiexl (circCtr.x + circPt.y, circCtr.y + circPt.x);
    setPiexl (circCtr.x - circPt.y, circCtr.y + circPt.x);
    setPiexl (circCtr.x + circPt.y, circCtr.y - circPt.x);
    setPiexl (circCtr.x - circPt.y, circCtr.y - circPt.x);
}

void circleMidpoint (screenPt circCtr, GLint radius)
{
    screenPt circPt;
    
    GLint p = 1 - radius;         // Initial value of midpoint parameter.
    
    circPt.x = 0;             // Set coordinates for top point of circle.
    circPt.y = radius;
    
    /* Plot the initial point in each circle quadrant. */
    circlePlotPoints (circCtr, circPt);
    
    /* Calculate next points and plot in each octant. */
    while (circPt.x < circPt.y) {
        circPt.x++;
        if (p < 0)
            p += 2 * circPt.x + 1;
        else {
            circPt.y--;
            p += 2 * (circPt.x - circPt.y) + 1;
        }
        circlePlotPoints (circCtr, circPt);
    }
}

void adjust(GLfloat &val){
    if (val > 360.0){
        val = val - 360.0;
    }
}

void calRotateAngle(){
    angle_earth_spin += earth_spin;
    angle_earth_revolution += earth_revolution;
    angle_moon_spin += moon_spin;
    angle_moon_revolution += moon_revolution;
    angle_sun_spin += sun_spin;
    angle_sun_revolution += sun_revolution;
    
    adjust(angle_sun_spin);
    adjust(angle_earth_spin);
    adjust(angle_moon_spin);
    adjust(angle_earth_revolution);
    adjust(angle_moon_revolution);
    adjust(angle_sun_revolution);
}

void display(){
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    calRotateAngle();
    glColor3f(1.0, 0.0, 0.0);
    screenPt p;
    
    glPushMatrix();
    glRotatef(angle_sun_spin, 0.0, 0.0, 1.0);
    p.setCoords(0, 0);
    circleMidpoint(p, 40);
    
    glPushMatrix();
    glRotatef(angle_earth_revolution, 0.0, 0.0, 1.0);
    
    glPushMatrix();
    glTranslatef(0, 160, 0);
    glRotatef(angle_earth_spin, 0.0, 0.0, 1.0);
    p.setCoords(0, 0);
    circleMidpoint(p, 30);
    
    glPushMatrix();
    glRotatef(angle_moon_revolution, 0.0, 0.0, 1.0);
    
    glPushMatrix();
    glTranslatef(0, 60, 0);
    glRotatef(angle_moon_spin, 0.0, 0.0, 1.0);
    
    p.setCoords(0, 0);
    circleMidpoint(p, 20);
    
    glPopMatrix();  //moon_spin
    glPushMatrix(); //moon_revolution
    glPopMatrix();  //earth_spin
    glPopMatrix(); //earth_revolution
    glPopMatrix(); //sun_spin
    
    glutSwapBuffers();
}

void reshape(int w, int h){
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluOrtho2D(-300.0, 300.0, -300.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char * argv[]) {
    // insert code here...
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    
    init();
    
    glutDisplayFunc(display);
    glutIdleFunc(display);
    
    glutMainLoop();
    
    return 0;
}
