//
//  main.cpp
//  123
//
//  Created by cdf on 15/12/3.
//  Copyright © 2015年 cdf. All rights reserved.
//

#include "GLUT/GLUT.h"

#define GLRGB(r, g, b) glColor3f(r / 255.0f, g / 255.0f, b / 255.0f)

static double year = 0, day = 0, startYear = 0;
static double moonday = 0, moonmonth = 0;
static double secondyear = 0, secondday = 0;

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    // 绘制太阳
    glPushMatrix();
    GLRGB(220, 198, 82);
    // 太阳自转
    glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
    glutWireSphere(1.01, 20, 16);
    glPopMatrix();
    // 绘制地球
    glPushMatrix();
    GLRGB(0, 97, 255);
    // 绘制地球轨道
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glRotatef(90.0f, 1.0, 0.0, 0.0);
    glutSolidTorus(0.005, 3.0, 10, 64);
    glPopMatrix();
    // 旋转
    glRotatef((GLfloat)year, 0.0, 1.0, 0.0);//行星公转
    glTranslatef(3.0, 0.0, 0.0);
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);//行星自转
    glutWireSphere(0.3, 10, 8);
    // 绘制月球
    glPushMatrix();
    glRotatef((GLfloat)moonmonth, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef((GLfloat)moonday, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glutWireSphere(0.2, 10, 8);
    glPopMatrix();
    // 绘制月球轨道
    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0);
    glRotatef(90.0f, 1.0, 0.0, 0.0);
    glutSolidTorus(0.005, 1.0, 10, 64);
    glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 8.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0);
}

void idle() {
    moonday = moonday + 1.2;
    if (moonday >= 360) {
        moonday -= 360;
    }
    moonmonth = moonmonth + 1.2;
    if (moonmonth >= 360) {
        moonmonth -= 360;
    }
    day = day + 0.6;
    if (day >= 360) {
        day -= 360;
    }
    year = year + 0.3;
    if (day >= 360) {
        year -= 360;
    }
    startYear = startYear + 0.6;
    if (startYear >= 360) {
        startYear -= 360;
    }
    secondday += 0.45;
    if (secondday>=360) {
        secondday -= 360;
    }
    secondyear += 0.09;
    if (secondyear >= 360) {
        secondyear -= 360;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
