//
//  main.c
//  OpenGL
//
//  Created by ax on 17/3/15.
//  Copyright © 2017年 aixing. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <math.h>
#include <stdio.h>
const int n = 20;
const GLdouble R = 0.5f;
const GLdouble Pi = 3.1415926536f;
const GLdouble factor = 0.1f;

// 太阳、地球和月亮
// 假设每个月都是30天
// 一年12个月，共是360天
static int day = 0; // day的变化：从0到359
void myDisplay(void)
{
    glClearColor(0.8, 0.8, 0.8, 1);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //    glClearDepth(1);
    //    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 400000000);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, -400000000, 400000000, 0, 0, 0, 0, 0, 1);
    
    // 定义太阳光源，它是一种白色的光源
    {
        GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_light_ambient[]   = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_light_diffuse[]   = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        
        glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT,   sun_light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,   sun_light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
        
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        
    }
    // 定义太阳的材质并绘制太阳
    {
        GLfloat sun_mat_ambient[]   = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_mat_diffuse[]   = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_mat_emission[] = {0.5f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_mat_shininess   = 0.0f;
        
        glMaterialfv(GL_FRONT, GL_AMBIENT,    sun_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,    sun_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,   sun_mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION,   sun_mat_emission);
        glMaterialf (GL_FRONT, GL_SHININESS, sun_mat_shininess);
        
        //        glutSolidSphere(2.0, 40, 32);
        // 绘制红色的“太阳”
        glColor3f(1.0f, 0.0f, 0.0f);
        glutSolidSphere(69600000, 20, 20);
        //        glEnable(GL_DEPTH_TEST);
    }
    
    // 定义地球的材质并绘制地球
    {
        GLfloat earth_mat_ambient[]   = {0.0f, 0.0f, 0.5f, 1.0f};
        GLfloat earth_mat_diffuse[]   = {0.0f, 0.0f, 0.5f, 1.0f};
        GLfloat earth_mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f};
        GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat earth_mat_shininess   = 50.0f;
        
        glMaterialfv(GL_FRONT, GL_AMBIENT,    earth_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,    earth_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,   earth_mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION,   earth_mat_emission);
        glMaterialf (GL_FRONT, GL_SHININESS, earth_mat_shininess);
        
        //        glRotatef(angle, 0.0f, -1.0f, 0.0f);
        //        glTranslatef(5.0f, 0.0f, 0.0f);
        //        glutSolidSphere(2.0, 40, 32);
        // 绘制蓝色的“地球”
        //地球半径
        GLdouble earth_radio = 15945000.0f;
        //日地距离
        GLdouble earth_sun_radio = 150000000.0f;
        //日地角度
        GLdouble earth_angle = day / 360.0 * 2 * Pi;
        //地球x坐标
        //    GLdouble earth_x = earth_sun_radio * cos(earth_angle);
        //地球y坐标
        //    GLdouble earth_y = earth_sun_radio * sin(earth_angle);
        
        //    printf("x = %f y = %f\n", earth_x, earth_y);
        
        glColor3f(0.0f, 0.0f, 1.0f);
        glRotatef(earth_angle / 2.0 / Pi * 360.0, 0.0f, 0.0f, -1.0f);
        glTranslatef(earth_sun_radio, 0.0f, 0.0f);
        glutSolidSphere(earth_radio, 20, 20);
    }
    
    // 定义地球的材质并绘制月球
    {
        GLfloat earth_mat_ambient[]   = {0.5f, 0.5f, 0.0f, 1.0f};
        GLfloat earth_mat_diffuse[]   = {0.5f, 0.5f, 0.0f, 1.0f};
        GLfloat earth_mat_specular[] = {1.0f, 1.0f, 0.0f, 1.0f};
        GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat earth_mat_shininess   = 100.0f;
        
        glMaterialfv(GL_FRONT, GL_AMBIENT,    earth_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,    earth_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,   earth_mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION,   earth_mat_emission);
        glMaterialf (GL_FRONT, GL_SHININESS, earth_mat_shininess);
        
        //        glRotatef(angle, 0.0f, -1.0f, 0.0f);
        //        glTranslatef(5.0f, 0.0f, 0.0f);
        //        glutSolidSphere(2.0, 40, 32);
        // 绘制黄色的“月亮”
        //月球半径
        GLdouble month_radio = 4345000.0f;
        //地月距离
        GLdouble month_earth_radio = 38000000.0f;
        //地月角度
        GLdouble month_earth_angle = (day % 30) / 30.0 * 2 * Pi;
        //    printf("%d %f %f\n", day, cos(month_earth_angle), sin(month_earth_angle));
        //月球x坐标
        //    GLdouble month_x = earth_x + month_earth_radio * cos(month_earth_angle);
        //月球y坐标
        //    GLdouble month_y = earth_y + month_earth_radio * sin(month_earth_angle);
        //月球太阳角度
        //    GLdouble month_sun_angle = atan(month_y / month_x);
        glColor3f(1.0f, 1.0f, 0.0f);
        glRotatef(month_earth_angle / 2.0 / Pi * 360.0, 0.0f, 0.0f, -1.0f);
        glTranslatef(month_earth_radio, 0.0f, 0.0f);
        glutSolidSphere(month_radio, 20, 20);
    }
    
    
    
    glFlush();
    
    glutSwapBuffers();
}

void myIdle(void) {
    ++ day;
    if (day >= 360) {
        day = 0;
    }
    myDisplay();
}


void myDisplay3(void)
{
    int i;
    // glShadeModel(GL_FLAT);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    for(i=0; i<=8; ++i)
    {
        glColor3f(i&0x04, i&0x02, i&0x01);
        glVertex2f(cos(i*Pi/4), sin(i*Pi/4));
    }
    glEnd();
    glFlush();
}

void myDisplay2(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, .0f, 1.0f);
    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
    glFlush();
}

void myDisplay1(void)

{
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glPolygonMode(GL_FRONT, GL_FILL); // 设置正面为填充模式
    
    glPolygonMode(GL_BACK, GL_LINE);   // 设置反面为线形模式
    
    glFrontFace(GL_CCW);               // 设置逆时针方向为正面
    
    glBegin(GL_POLYGON);               // 按逆时针绘制一个正方形，在左下方
    
    glVertex2f(-0.5f, -0.5f);
    
    glVertex2f(0.0f, -0.5f);
    
    glVertex2f(0.0f, 0.0f);
    
    glVertex2f(-0.5f, 0.0f);
    
    glEnd();
    
    glBegin(GL_POLYGON);               // 按顺时针绘制一个正方形，在右上方
    
    glVertex2f(0.0f, 0.0f);
    
    glVertex2f(0.0f, 0.5f);
    
    glVertex2f(0.5f, 0.5f);
    
    glVertex2f(0.5f, 0.0f);
    
    glEnd();
    
    glFlush();
    
}

//void myDisplay(void)
//
//{
//
//    int i;
//    GLdouble x;
//     glClear(GL_COLOR_BUFFER_BIT);
//     glBegin(GL_LINE_STRIP);
//        for (x = -1.0f/factor; x < 1.0f/factor; x += 0.01f)
//        {
//            glVertex2f(x*factor, sin(x)*factor);
//        }
//
//     glEnd();
//
////     glBegin(GL_POLYGON);
////     for(i=0; i<n; ++i)
////         glVertex2f(R*cos(2*Pi/n*i), R*sin(2*Pi/n*i));
////     glEnd();
//
//     glFlush();
//
//}

int main(int argc, char *argv[])

{
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    
    glutInitWindowPosition(100, 100);
    
    glutInitWindowSize(400, 400);
    
    
    glutCreateWindow("太阳，地球和月亮");
    
    glutDisplayFunc(&myDisplay);
    
    glutIdleFunc(&myIdle);
    glutMainLoop();
    
    return 0;
    
}
