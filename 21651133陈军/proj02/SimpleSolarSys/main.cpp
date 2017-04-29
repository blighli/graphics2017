//
//  main.cpp
//  SimpleSolarSys
//
//  Created by Chanjun2016 on 2017/4/9.
//  Copyright © 2017年 Chanjun2016. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <GLUT/GLUT.h>
#include <math.h>

#define TEXTURE_COUNT 3
GLuint textures[TEXTURE_COUNT];

//环境光参数
GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
//漫反射光参数
GLfloat lightDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
//镜面光参数
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

//光源位置
GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//镜面反射属性
GLfloat specularf[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLUquadric* sunQuad;
GLUquadric* earthQuad;
GLUquadric* moonQuad;
const char* filePath[] = { "sunmap.tga", "earthmap.tga", "moonmap.tga"};

float spin = 0;
static GLint fogMode;
const int n = 100;
GLfloat R = 1.0f;
const GLfloat Pi = 3.1415926536f;

#pragma pack(1)
typedef struct
{
    GLbyte identsize;              // Size of ID field that follows header (0)
    GLbyte colorMapType;           // 0 = None, 1 = paletted
    GLbyte imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short colorMapStart;          // First colour map entry
    unsigned short colorMapLength;         // Number of colors
    unsigned char colorMapBits;   // bits per palette entry
    unsigned short xstart;                 // image x origin
    unsigned short ystart;                 // image y origin
    unsigned short width;                  // width in pixels
    unsigned short height;                 // height in pixels
    GLbyte bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8)
GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

void  Init()
{
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
    
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT1);
    
    glEnable(GL_LIGHTING);//启用光照
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularf);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
    
    glGenTextures(TEXTURE_COUNT, textures);
    GLubyte *pImage = NULL;
    GLint iWidth, iHeight, iComponent;
    GLenum eFormat;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int i = 0; i < TEXTURE_COUNT; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        //载入纹理
        GLbyte* pBytes = gltLoadTGA(filePath[i], &iWidth, &iHeight, &iComponent, &eFormat);
        glTexImage2D(GL_TEXTURE_2D, 0, iComponent, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        free(pBytes);
    }
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
    
    /*GLfloat zPlane[] = { 0.0f, 0.0f, 1.0f, 0.0f };*/
    //glEnable(GL_TEXTURE_GEN_T);
    //glEnable(GL_TEXTURE_GEN_S);
    
    //glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    //glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    
    //glTexGenfv(GL_S, GL_OBJECT_PLANE, zPlane);
    //glTexGenfv(GL_T, GL_OBJECT_PLANE, zPlane);
    sunQuad = gluNewQuadric();
    gluQuadricTexture(sunQuad, GLU_TRUE);
    gluQuadricDrawStyle(sunQuad, GLU_FILL);
    earthQuad = gluNewQuadric();
    gluQuadricTexture(earthQuad, GLU_TRUE);
    gluQuadricDrawStyle(earthQuad, GLU_FILL);
    moonQuad = gluNewQuadric();
    gluQuadricTexture(moonQuad, GLU_TRUE);
    gluQuadricDrawStyle(moonQuad, GLU_FILL);
}

void DrawCircle()
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n; ++i)
    {
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(R*cos(2 * Pi / n*i), 0, R*sin(2 * Pi / n*i));
    }
    glEnd();
    glFlush();
}

void display(void)
{
    glEnable(GL_DEPTH_TEST);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
    
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT1);
    
    glEnable(GL_LIGHTING);//启用光照
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularf);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(0.8, 0.2, 0.1);
    glPushMatrix(); //记住太阳的位置
    glRotatef(spin, 0.0, 1.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    gluSphere(sunQuad, 1.5f, 20, 16);
    //glutSolidSphere(1.0, 20, 16);   //太阳
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    R = 4;
    DrawCircle();
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(spin, 0.0, 1.0, 0.0);  //公转，绕着一个向量以给定角度旋转（正的为逆时针）
    glTranslatef(4.0, 0.0, 0.0);
    R = 1;
    DrawCircle();
    
    glRotatef(90, 1.0, 0.0, 0.0);//绕x轴旋转90度，将地球方向调正确
    glRotatef(5 * spin, 0.0, 0.0, -1.0); //自转
    glColor3f(1.0, 1.0, 0.9);
    ///glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
    //glutSolidSphere(0.5, 16, 8);   //地球
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    gluSphere(earthQuad, 0.5, 16, 8);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();//回到太阳的位置
    
    glPushMatrix();
    glRotatef(spin, 0.0, 1.0, 0.0);  //公转，绕着一个向量以给定角度旋转（正的为逆时针）
    glTranslatef(4.0, 0.0, 0.0);
    //glTranslatef(1.0, 0.0, 0.0);
    glRotatef(3 * spin, 0.0, 1.0, 0.0);//月球绕地球转
    ////glPopMatrix();
    
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(5 * spin, 0.0, 1.0, 0.0);//月球自转
    
    ///glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse2);
    //glutSolidSphere(0.3, 16, 8); //月亮
    glColor3f(1.0, 0.9, 0.9);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    gluSphere(moonQuad, 0.3, 16, 8);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();//回到太阳的位置
    
    glDisable(GL_COLOR_MATERIAL);
    glutSwapBuffers();
}
void spinDisplay(void)
{
    spin = spin + 0.05;
    if (spin > 360)
        spin = spin - 360;
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.5, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("MyOpengGL");
    Init();
    glutDisplayFunc(display);
    glutIdleFunc(spinDisplay);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

void BYTE_SWAP(unsigned short &x){
    return;
    //x = ((x << 8 & 0xff00) | x >> 8);
}

GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
    FILE *pFile;    // File pointer
    TGAHEADER tgaHeader;   // TGA file header
    unsigned long lImageSize;   // Size in bytes of image
    short sDepth;    // Pixel depth;
    GLbyte *pBits = NULL;          // Pointer to bits
    
    // Default/Failed values
    *iWidth = 0;
    *iHeight = 0;
    *eFormat = GL_BGR_EXT;
    *iComponents = GL_RGB8;
    
    // Attempt to open the fil
    pFile = fopen(szFileName, "rb");
    if (pFile == NULL)
        return NULL;
    
    //return NULL;
    
    // Read in header (binary)
    fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);
    
    // Do byte swap for big vs little endian
#ifdef __APPLE__
    BYTE_SWAP(tgaHeader.colorMapStart);
    BYTE_SWAP(tgaHeader.colorMapLength);
    BYTE_SWAP(tgaHeader.xstart);
    BYTE_SWAP(tgaHeader.ystart);
    BYTE_SWAP(tgaHeader.width);
    BYTE_SWAP(tgaHeader.height);
#endif
    
    // Get width, height, and depth of texture
    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;
    
    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targa's.
    if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return NULL;
    // Calculate size of image buffer
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    
    // Allocate memory and check for success
    pBits = static_cast<GLbyte*>(malloc(lImageSize * sizeof(GLbyte)));
    if (pBits == NULL)
        return NULL;
    
    // Read in the bits
    // Check for read error. This should catch RLE or other
    // weird formats that I don't want to recognize
    if (fread(pBits, lImageSize, 1, pFile) != 1)
    {
        free(pBits);
        return NULL;
    }
    
    // Set OpenGL format expected
    switch (sDepth)
    {
        case 3:     // Most likely case
            *eFormat = GL_BGR_EXT;
            *iComponents = GL_RGB8;
            break;
        case 4:
            *eFormat = GL_BGRA_EXT;
            *iComponents = GL_RGBA8;
            break;
        case 1:
            *eFormat = GL_LUMINANCE;
            *iComponents = GL_LUMINANCE8;
            break;
    };
    
    
    // Done with File
    fclose(pFile);
    
    // Return pointer to image data
    return pBits;
}
