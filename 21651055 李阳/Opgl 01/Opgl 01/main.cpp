//
//  main.cpp
//  Opgl 01
//
//  Created by 李阳 on 2017/3/15.
//  Copyright © 2017年 Xcode. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <math.h>
#include <stdio.h>

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void SpecialKeys(int key, int x, int y);
void ChangeSize( GLsizei w, GLsizei h );
void DrawAxis();
void SetupRC();
void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    DrawAxis();
    
    //太阳
    glColor3f( 1.0f, 1.0f, 0.0f );
    glDisable( GL_LIGHTING );
    glutSolidSphere( 50.f, 15, 15 );
    
    //添加光源
    glEnable( GL_LIGHTING );
    GLfloat lightPos[] = { 0, 0, 0 };
    glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
    
    //地球
    static GLfloat earthAngle = 0;
    const GLfloat RADIUS = 100.f;
    glRotatef( earthAngle, 0, 0, 1 );
    glTranslatef( RADIUS, RADIUS, 0 );
    glutSolidSphere( 20.f, 15, 15 );
    
    //月亮
    glRotatef(earthAngle/30.0*360.0 - earthAngle/360.0*360.0, 0.0f, 0.0f, -1.0f);
    glTranslatef(RADIUS/3, 0.0f, 0.0f);
    glutSolidSphere(7, 20, 20);
    
    earthAngle += 0.1;
    if ( earthAngle >= 360 )
    {
        earthAngle -= 360;
    }
    glPopMatrix();
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (800, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow( "银河系" );
    glutDisplayFunc( RenderScene );
    glutIdleFunc (RenderScene);
    glutReshapeFunc( ChangeSize );
    glutSpecialFunc( SpecialKeys );
    SetupRC();
    glutMainLoop();
    return 1;
}


void SetupRC()
{
    glClearColor( 0, 0.0, 0.0, 1 );
    glShadeModel( GL_SMOOTH );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
}


void ChangeSize( GLsizei w, GLsizei h )
{
    GLfloat nRange = 200.0f;
    if(h == 0)
        h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else
        glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpecialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
        xRot -= 5.0f;
    if(key == GLUT_KEY_DOWN)
        xRot += 5.0f;
    if(key == GLUT_KEY_LEFT)
        yRot -= 5.0f;
    if(key == GLUT_KEY_RIGHT)
        yRot += 5.0f;
    if(xRot > 356.0f)
        xRot = 0.0f;
    if(xRot < -1.0f)
        xRot = 355.0f;
    if(yRot > 356.0f)
        yRot = 0.0f;
    if(yRot < -1.0f)
        yRot = 355.0f;
    glutPostRedisplay();
}

void DrawAxis()
{
    //绘制x、y、z坐标轴
    {
        glColor3f( 0.0f, 0.0f, 1.0f );
        glBegin( GL_LINES );
        {
            // x-axis
            glVertex3f( 0.0f, 0.0f, 0.0f);
            glVertex3f( 200.0f, 0.0f, 0.0f);
            // x-axis arrow
            glVertex3f( 200.0f, 0.0f, 0.0f);
            glVertex3f( 193.0f, 3.0f, 0.0f);
            glVertex3f( 200.0f, 0.0f, 0.0f);
            glVertex3f( 193.0f,-3.0f, 0.0f);
        }
        glEnd();
        glColor3f( 0.0f, 1.0f, 0.0f );
        glBegin( GL_LINES );
        {
            // y-axis
            glVertex3f( 0.0f, 0.0f, 0.0f);
            glVertex3f( 0.0f, 200.0f, 0.0f);
            glVertex3f( 0.0f, 200.0f, 0.0f);
            glVertex3f( 3.0f, 193.0f, 0.0f);
            glVertex3f( 0.0f, 200.0f, 0.0f);
            glVertex3f( -3.0f, 193.0f, 0.0f);
        }
        glEnd();
        glColor3f( 1.0f, 0.0f, 0.0f );
        glBegin( GL_LINES );
        {
            // z-axis
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 200.0f );
            glVertex3f( 0.0f, 0.0f, 200.0f );
            glVertex3f( 0.0f, 3.0f, 193.0f );
            glVertex3f( 0.0f, 0.0f, 200.0f );
            glVertex3f( 0.0f, -3.0f, 193.0f);
        }
        glEnd();
    }
}
