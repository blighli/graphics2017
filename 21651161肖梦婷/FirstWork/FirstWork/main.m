//
//  main.m
//  FirstWork
//
//  Created by xmt on 2017/3/16.
//  Copyright © 2017年 xmt. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLUT/glut.h>

static int sun_rotation = 0, earth_revolution = 0, earth_rotation = 0, planet_revolution = 0;

void init()
{
    glClearColor (1.0,1.0, 1.0, 0.0);  //将显示窗口的背景颜色设定为白色.
    glShadeModel(GL_FLAT);             //设置阴影模式为平面明暗模式
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // 清理显示窗口.
    glColor3f (1.0,0.0, 0.0);     // 设置绘图颜色为红色.
    glPushMatrix();               //把矩阵压入堆栈
    //第一个参数表示目标沿从原点到指定点(x,y,z)的方向矢量逆时针旋转的角度，后三个参数则是 //指定旋转方向矢量的坐标。
    glRotatef((GLfloat) sun_rotation, 0.0, -1.0, 2.0); //地球自转轴的偏转角度
    glRotatef(30, 1.0,0.0, 0.0);       //距离X轴偏转30度
    // glutWireSphere中第1、2、3个参数分别表示半径，经度线数、纬度线数
    glutWireSphere(1.0,20, 16);        // 按给定的参数值绘制太阳
    glPopMatrix();
    glColor3f (0.0,0.0, 1.0);     // 设置绘图颜色为蓝色.
    glPushMatrix();
    glRotatef((GLfloat)earth_revolution,0.0, 0.0, 1.0);   // 地球公转轴的偏转角度
    glTranslatef(2.0,0.0, 0.0); // 通过平移变换，产生太阳和地球之间的距离
    glPushMatrix();
    glRotatef((GLfloat)earth_rotation,0.0, -1.0, 2.0);   //地球自转轴的偏转角度
    glRotatef(10, 1.0,0.0, 0.0); // 距离X轴偏转10度
    glutWireSphere(0.25,10, 8);   // 按给定的参数值绘制地球
    
    //绘制卫星
    glTranslatef(0.5,0.0, 0.0); // 通过平移变换，产生卫星和地球之间的距离
    glRotatef((GLfloat)planet_revolution,0.0, -1.0, 2.0);   //卫星公转的偏转角度
    glRotatef(10, 1.0,0.0, 0.0); // 距离X轴偏转10度
    glutWireSphere(0.15,10, 8);   // 按给定的参数值绘制卫星
    glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();     // 刷新命令缓冲区
}

void reshape (int w, int h)
{
    glViewport (0, 0,(GLsizei) w, (GLsizei) h);   ////设置视区尺寸
    // 重置坐标系统，使投影变换复位
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ();
    //创建透视投影矩阵,4个参数分别表示：y方向上可见区域的夹角；纵横比为x（宽度）/y（高度）；//从观察者到近修剪平面的距离；从观察者到远修剪平面的距离
    gluPerspective(60.0,(GLfloat) w/(GLfloat) h, 1.0, 20.0);
    // 重置坐标系统，使投影变换复位
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //定义视景转换,前3个参数表示视点的空间位置；中间3个参数表示参考点的空间位置；最后3
    //个参数表示向上向量的方向
    gluLookAt (0.0,0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void keyboard(unsigned char key, int x, int y)    //定义键盘按键操作
{
    switch (key) {
        case 'w':
            //按键盘w则卫星顺时针自转，每次旋转角度为20度
            planet_revolution= (planet_revolution + 10) % 360;
            //地球逆时针自转，每次旋转角度为10度
            earth_rotation =(earth_rotation - 10) % 360;
            //地球绕着太阳逆时针公转，每次旋转角度为5度
            earth_revolution =(earth_revolution + 3) % 360;
            //太阳逆时针自转，每次旋转角度为10度
            sun_rotation =(sun_rotation - 10) % 360;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

int main(int argc, const char * argv[]) {
    glutInit(&argc, argv); //固定格式
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // 设置OpenGL窗口的显示模式
    glutInitWindowSize(500, 500);       //设置显示窗口的宽和高
    glutInitWindowPosition(100, 100);   //设置显示窗口左上角坐标
    glutCreateWindow("The model of galaxy");   // 创建显示窗口
    init ();    //初始化
    glutDisplayFunc(display);   //将图形发送至显示窗口
    glutReshapeFunc(reshape);     // 为当前窗口设置窗口再整形回调函数
    glutKeyboardFunc(keyboard);//接收键盘操作
    glutMainLoop();   //显示初始图形并使程序进入检查鼠标或键盘等设备输入的无穷循环中
    return 0;
}
