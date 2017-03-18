//
//  main.cpp
//  OpenGL
//
//  Created by Armas on 2017/3/17.
//  Copyright © 2017年 Armas. All rights reserved.
//

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

static double year = 0, day = 0, startYear = 0;
static double moonDay = 0, moonMonth = 0;


void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}


void displayWindow()

{
    
    glClear(GL_COLOR_BUFFER_BIT);
    /*绘画太阳*/
    glPushMatrix();
    glColor3f(255 / 255.0, 0 / 255.0, 0.0);
    glRotatef((GLfloat)year, 0.0, 1.0, 0.0);//太阳自转
    glutWireSphere(3, 40, 32);     //球体网格
    glPopMatrix();
    
    /*绘制地球轨道*/
    glPushMatrix();
    glColor3f(255 / 255.0, 0 / 255.0, 0.0);
    glRotatef(90.0f, 1.0, 0.0, 0.0);
    glutSolidTorus(0.005, 6,10, 64);
    glPopMatrix();
    
    /*绘制地球*/
    glPushMatrix();
    glRotatef((GLfloat)year, 0.0, 1.0, 0.0);//行星公转
    glTranslatef(6.0, 0.0, 0.0);
    glColor3f(52 / 255.0, 143 / 255.0, 255 / 255.0);
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);//行星自转
    glutWireSphere(0.31, 20, 16);
    
    /*绘制卫星轨道*/
    glPushMatrix();
    glColor3f(52 / 255.0, 143 / 255.0, 255 / 255.0);
    glRotatef(90.0f, 1.0, 0.0, 0.0);
    
    glutSolidTorus(0.005, 1, 10, 64);
    glPopMatrix();
    
    /*绘画卫星*/
    glPushMatrix();
    glRotatef((GLfloat)moonMonth, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef((GLfloat)moonDay, 0.0, 1.0, 0.0);
    glColor3f(120 / 255.0, 121 / 255.0, 103 / 255.0);
    glutWireSphere(0.15, 20, 16);
    glPopMatrix();
    glPopMatrix();
    
    glutSwapBuffers(); // 交换双缓冲区指针
    
}


void redrawWindow(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0);
}

void idle()
{
    moonDay = moonDay + 0.8;
    if (moonDay >= 360)
    {
        moonDay -= 360;
    }
    moonMonth = moonMonth + 0.6;
    if (moonMonth >= 360)
    {
        moonMonth -= 360;
    }
    day = day + 0.4;
    if (day >= 360)
    {
        day -= 360;
    }
    
    /**控制地球的转动速度*/
    year = year + 0.8;
    if (day >= 360)
    {
        year -= 360;
    }
    startYear = startYear + 0.04;
    if (startYear >= 360)
    {
        startYear -= 360;
    }
    
    glutPostRedisplay();
}


int main(int argc, char ** argv) {
    
    glutInit(&argc, argv);   // GLUt初始化
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    
    glutInitWindowSize(800, 600);  // 800X600
    glutInitWindowPosition(250, 250); // 窗口位置
    glutCreateWindow("21651160-郭志强");  // 创建窗口
    
    init();
    glutDisplayFunc(displayWindow); // 窗口的绘制
    glutReshapeFunc(redrawWindow); // 每次窗口被改变需要重新绘制
    
    glutIdleFunc(idle);
    glutMainLoop();    // glut程序进入事件循环
    
    
}
