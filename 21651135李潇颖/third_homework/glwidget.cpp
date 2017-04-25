/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"

#include <QMouseEvent>
#include <QTimer>
#include <math.h>
#include <QDebug>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    loadTextures();
}

GLWidget::~GLWidget()
{
    makeCurrent();
}

void GLWidget::setXRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != xRot)
    {
        xRot = angle;
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != yRot)
    {
        yRot = angle;
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != zRot)
    {
        zRot = angle;
        updateGL();
    }
}

void GLWidget::initializeGL()
{
    static const GLfloat lightPos[4] = { 5.0f, 5.0f, 10.0f, 1.0f };
    static const GLfloat reflectance1[4] = { 0.8f, 0.1f, 0.0f, 1.0f };
    static const GLfloat reflectance2[4] = { 0.0f, 0.8f, 0.2f, 1.0f };
    static const GLfloat reflectance3[4] = { 0.2f, 0.2f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

    // draw coordinate system
    drawCoordinateSystem(0, 0, 0);

    // draw 3d obj
    drawObject();
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, +1.0, -1.0, 1.0, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -40.0);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        //setYRotation(yRot + 8 * dx);
        //setZRotation(zRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        //setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
        //setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
    float scaleFactory  = 1.0;
    if (event->angleDelta().y() > 0)
    {
        scaleFactory = 1.5f;
    }
    else if (event->angleDelta().y() < 0)
    {
        scaleFactory = 0.75f;
    }
    glScalef(scaleFactory, scaleFactory, scaleFactory);

    xRot = 0.0;
    yRot = 0.0;
    zRot = 0.0;

    updateGL();
}

void GLWidget::loadTextures()
{
    QImage buf;
    if(!buf.load("data/dog/maps/DogC1.jpg"))
    {
        qWarning("Cannot open the image...");
        QImage dummy(128, 128, QImage::Format_RGB32);
        dummy.fill(Qt::green);
        buf = dummy;
    }
    m_textureImage = convertToGLFormat(buf);


    QImage buf2;
    if(!buf2.load("data/dog/maps/DogEyeC1.jpg"))
    {
        qWarning("Cannot open the image...");
        QImage dummy(128, 128, QImage::Format_RGB32);
        dummy.fill(Qt::green);
        buf2 = dummy;
    }
    m_eyeTextureImage = convertToGLFormat(buf2);
}

void GLWidget::drawCoordinateSystem(float x, float y, float z)
{
    glTranslatef(x, y, z);

    glBegin(GL_LINES);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
        glVertex3d(0.0f, 0.0f, 0.0f);
        glVertex3d(20.0f, 0.0f, 0.0f);

        glClear(GL_COLOR_BUFFER_BIT);
        glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
        glVertex3d(0.0f, 0.0f, 0.0f);
        glVertex3d(0.0f, 20.0f, 0.0f);

        glClear(GL_COLOR_BUFFER_BIT);
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
        glVertex3d(0.0f, 0.0f, 0.0f);
        glVertex3d(0.0f, 0.0f, 20.0f);
    glEnd();

    glColor4f(1.0, 1.0, 1.0, 1.0);
    renderText(3, 0, 0, "x");
    renderText(0, 3, 0, "y");
    renderText(0, 0, 3, "z");
}

void GLWidget::drawObject()
{
    static GLfloat dark[4] = { 0.0, 0.0, 0.0, 1.0 };
    static GLfloat normal[4] = { 0.5, 0.5, 0.5, 1.0 };
    static GLfloat bright[4] = { 1.0, 1.0, 1.0, 1.0 };
    static GLfloat pos[4] = { -0.2588, 0.0, 0.9659, 0.0 };

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    if (!m_glmModel)
    {
        m_glmModel = glmReadOBJ("data/dog/dog.obj");//读取OBJ文件
        if (!m_glmModel) exit(0);
        glmUnitize(m_glmModel);//单位化模型model并返回缩放比例因子，单位化就是把模型通过平移和缩放变换限制到3维坐标系中点为中心的一个单位正方体区域内
        glmFacetNormals(m_glmModel);//计算模型面的法向（假定多边形顶点顺序为逆时针）
        glmVertexNormals(m_glmModel, 90.0);//计算模型的平滑顶点法向，angle参数为平滑交叉的最大角度（角度制）
    }

    glPushMatrix();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, dark);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dark);
    glLightfv(GL_LIGHT0, GL_SPECULAR, bright);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glMaterialfv(GL_FRONT, GL_AMBIENT, dark);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dark);
    glMaterialfv(GL_FRONT, GL_SPECULAR, bright);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // texture 1
    glTexImage2D(GL_TEXTURE_2D, 0, 3, m_eyeTextureImage.width(), m_eyeTextureImage.height(), 0, GL_RGBA , GL_UNSIGNED_BYTE, m_eyeTextureImage.bits());

    glmDraw(m_glmModel,GLM_SMOOTH |GLM_TEXTURE | GLM_MATERIAL);//先画一个实体——无纹理

    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, normal);
    glLightfv(GL_LIGHT0, GL_AMBIENT, normal);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, normal);
    glLightfv(GL_LIGHT0, GL_SPECULAR, dark);
    glMaterialfv(GL_FRONT, GL_AMBIENT, normal);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, normal);
    glMaterialfv(GL_FRONT, GL_SPECULAR, dark);
    glMateriali(GL_FRONT, GL_SHININESS, 0);

    glmDraw(m_glmModel,GLM_SMOOTH |GLM_TEXTURE | GLM_MATERIAL);//再画一个经纹理修饰和光照处理后的人物

    // texture 2
    glTexImage2D(GL_TEXTURE_2D, 0, 3, m_textureImage.width(), m_textureImage.height(), 0, GL_RGBA , GL_UNSIGNED_BYTE, m_eyeTextureImage.bits());
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, normal);
    glLightfv(GL_LIGHT0, GL_AMBIENT, normal);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, normal);
    glLightfv(GL_LIGHT0, GL_SPECULAR, dark);
    glMaterialfv(GL_FRONT, GL_AMBIENT, normal);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, normal);
    glMaterialfv(GL_FRONT, GL_SPECULAR, dark);
    glMateriali(GL_FRONT, GL_SHININESS, 0);
    glmDraw(m_glmModel,GLM_SMOOTH |GLM_TEXTURE | GLM_MATERIAL);//再画一个经纹理修饰和光照处理后的人物

    glPopMatrix();
}

void GLWidget::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}

