//
//  main.cpp
//  Task1
//
//  Created by SuperleeOS on 17/3/14.
//  Copyright © 2017年 SuperleeOS. All rights reserved.
//

#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>

static double year = 0, day = 0, startYear = 0;
static double moonDay = 0, moonMonth = 0;

void display()

{
    
    glClear(GL_COLOR_BUFFER_BIT); //清除画布
    /*绘画太阳*/
    glPushMatrix();
    glColor3f(237 / 255.0, 109 / 255.0, 0.0); // 画笔颜色
    glRotatef((GLfloat)year, 0.0, 1.0, 0.0);//太阳自转
    glutWireSphere(1.01, 20, 16);     // 画球体网格
    glPopMatrix(); // 从栈中取出原始状态
    
    /*绘制地球轨道*/
    glPushMatrix();  // 保存该帧位置和状态
    glColor3f(1.0, 1.0, 1.0);
    glRotatef(90.0f, 1.0, 0.0, 0.0);
    glutSolidTorus(0.005, 3.0, 10, 64);
    glPopMatrix();
    
    /*绘制地球*/
    glPushMatrix();
    glRotatef((GLfloat)year, 0.0, 1.0, 0.0);//行星公转
    glTranslatef(3.0, 0.0, 0.0);
    glColor3f(56/255.0, 94/255.0, 15/255.0);
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);//行星自转
    glutWireSphere(0.3, 10, 8);
    
    /*绘制卫星轨道*/
    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0);
    glRotatef(90.0f, 1.0, 0.0, 0.0);
    //glRotatef(45.0f, 0.0, 0.0, 1.0);
    glutSolidTorus(0.005, 1.0, 10, 64);
    glPopMatrix();
    
    /*绘画卫星*/
    glPushMatrix();
    glRotatef((GLfloat)moonMonth, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef((GLfloat)moonDay, 0.0, 1.0, 0.0);
    glColor3f(255/255.0, 255/255.0, 203/255.0);
    glutWireSphere(0.2, 10, 8);
    glPopMatrix();
    glPopMatrix();
    
    glutSwapBuffers(); // 交换双缓冲区指针
    
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // 设置窗口清除颜色
    glShadeModel(GL_FLAT);  //设置着色模式
}


void reshape(int w, int h)
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
    moonDay = moonDay + 0.4;
    if (moonDay >= 360)
    {
        moonDay -= 360;
    }
    moonMonth = moonMonth + 0.3;
    if (moonMonth >= 360)
    {
        moonMonth -= 360;
    }
    day = day + 0.2;
    if (day >= 360)
    {
        day -= 360;
    }
    //    可控制地球转动的速度
    year = year + 0.4;
    if (day >= 360)
    {
        year -= 360;
    }
    startYear = startYear + 0.02;
    if (startYear >= 360)
    {
        startYear -= 360;
    }
    
    glutPostRedisplay();
}


int main(int argc, char ** argv) {
    // insert code here...
    glutInit(&argc, argv);   // 对GLUT初始化，并处理所有命令行参数
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // 指定使用RGBA模式，双缓冲窗口
    glutInitWindowSize(1000, 600);  // 初始化窗口大小
    glutInitWindowPosition(300, 100); // 初始化位置
    glutCreateWindow("21651107_李超_太阳系");  // 绘制
    init();
    glutDisplayFunc(display); // 绘制窗口
    glutReshapeFunc(reshape); // 改变窗口时重新绘制
    //glutRotateFunc(rotate);
    glutIdleFunc(idle); // 
    glutMainLoop();    // glut程序进入事件循环

    
}
