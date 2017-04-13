#include "mushroom.h"
#include <stdlib.h>
#include <stdio.h>

#include <math.h> 
#include <GL/glut.h>
#include <GL/GLAux.h>

extern float DEG_TO_RAD;
extern GLuint ID[2];
GLfloat mt_ambient[]   = {0.7f, 0.15f, 0.1f, 1.0f};
GLfloat mt_diffuse[]   = {0.7f, 0.15f, 0.1f, 1.0f};
GLfloat mt_specular[] = {0.99f, 0.99f, 0.99f, 1.0f};
GLfloat mt_emission[] = {0.1f, 0.1f, 0.1f, 0.0f};
GLfloat mt_shininess   = 0.0f;

GLfloat mb_ambient[]   = {0.6f, 0.6f, 0.5f, 0.0f};
GLfloat mb_diffuse[]   = {0.6f, 0.6f, 0.5f, 0.0f};
GLfloat mb_specular[] = {0.2f, 0.2f, 0.2f, 0.0f};
GLfloat mb_emission[] = {0.1f, 0.1f, 0.1f, 0.0f};
GLfloat mb_shininess   = 0.0f;

void MushRoom(float x0, float y0, float z0, float R, float H, int N, float R2, float H2,int N2,int tag)
{
	float deg = 0;
	float x,y,z;
	float x1, y1, z1;
	float x2, y2, z2;
	float x3, y3, z3;
	float r1 = 0;
	float r2;
	float x11,y11,z11;
	float x22,y22,z22;
	float n1x, n1y, n1z;
	float n2x, n2y, n2z;
	float nx, ny, nz;

	////////////////////////////////////Ä¢¹½µ×///////////////////////
	for (deg=0; deg <=360; deg += 360/N)
	{
	
		//glColor3f(0.5,0.5,0.5);
		 glMaterialfv(GL_FRONT, GL_AMBIENT, mb_ambient);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, mb_diffuse);
         glMaterialfv(GL_FRONT, GL_SPECULAR, mb_specular);
         glMaterialfv(GL_FRONT, GL_EMISSION, mb_emission);
         glMaterialf (GL_FRONT, GL_SHININESS, mb_shininess);

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		 float rr = R/2;

		for (float r=0;r<=rr+rr/N;r+= (rr/N))
		{
			r1 = r;
			r2 = r + rr/N;

			x1 = x0 + (R - rr + r1) * sin(DEG_TO_RAD * (deg));
			z1 = z0 - (R - rr + r1) * cos(DEG_TO_RAD * (deg));
			y1 = y0  + rr - sqrt(rr*rr - r1*r1);

			x11 = x0 + (R - rr + r1) * sin(DEG_TO_RAD * (deg + 360/N));
			z11 = z0 - (R - rr + r1) * cos(DEG_TO_RAD * (deg + 360/N));
			

			x2 = x0 + (R - rr + r2) * sin(DEG_TO_RAD * (deg));
			z2 = z0 - (R - rr + r2) * cos(DEG_TO_RAD * (deg));
			y2 = y0  + rr - sqrt(rr*rr - r2*r2);

			x22 = x0 + (R - rr + r2) * sin(DEG_TO_RAD * (deg + 360/N));
			z22 = z0 - (R - rr + r2) * cos(DEG_TO_RAD * (deg + 360/N));

			glBegin(GL_QUADS);
			n1x = x1 - x2;
			n1y = y1 - y2;
			n1z = z1 - z2;

			n2x = x22 - x2;
			n2y = y2 - y2;
			n2z = z22 - z2;

			nx = n1y * n2z - n1z * n2y;
			ny = n1z * n2x - n1x * n2z;
			nz = n1x * n2y - n1y * n2x;


			glNormal3f(-nx,-ny,-nz);
			glVertex3f(x1,y1,z1);
			glVertex3f(x2,y2,z2);
			glVertex3f(x22,y2,z22);
			glVertex3f(x11,y1,z11);

			glEnd();
		}	
		
		x1 =  x0 + R * sin(DEG_TO_RAD * deg);
		z1 = z0 - R * cos(DEG_TO_RAD * deg);
		y1 = y0;

		x2 =  x0 + R * sin(DEG_TO_RAD * (deg + 360/N));
		z2 = z0 - R * cos(DEG_TO_RAD * (deg + 360/N));
		y2 = y0;
		/////////////////////////////
			n1x = x1 - x2;
			n1y = 0;
			n1z = z1 - z2;

			n2x = 0;
			n2y = H/N;
			n2z = 0;

			nx = n1y * n2z - n1z * n2y;
			ny = n1z * n2x - n1x * n2z;
			nz = n1x * n2y - n1y * n2x;

			glNormal3f(nx,ny,nz);

		glBegin(GL_QUAD_STRIP);
		for (y = y0+r1;y<=(y0+H);y+=(H/N))
		{
			glVertex3f(x1,y,z1);
			glVertex3f(x2,y,z2);
		}
		glEnd();
		
		//glDisable(GL_CULL_FACE);
	}
	//////////////////////////////Ä¢¹½Ã±////////////////////////////////////
	for (deg=0; deg <= 360; deg += 360/N)
	{
		//glColor3f(1,0,0);
		 glMaterialfv(GL_FRONT, GL_AMBIENT, mt_ambient);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, mt_diffuse);
         glMaterialfv(GL_FRONT, GL_SPECULAR, mt_specular);
         glMaterialfv(GL_FRONT, GL_EMISSION, mt_emission);
         glMaterialf (GL_FRONT, GL_SHININESS, mt_shininess);
		for (float r=0;r<=R2+R2/N2;r+= (R2/N2))
		{
			r1 = r;
			r2 = r + R2/N2;

			x1 = x0 + r1 * sin(DEG_TO_RAD * (deg));
			z1 = z0 - r1 * cos(DEG_TO_RAD * (deg));
			y1 = y0 + H + H2 * sqrt(1-r*r/(R2 * R2));

			x11 = x0 + r1 * sin(DEG_TO_RAD * (deg + 360/N));
			z11 = z0 - r1 * cos(DEG_TO_RAD * (deg + 360/N));
			

			x2 = x0 + r2 * sin(DEG_TO_RAD * (deg));
			z2 = z0 - r2 * cos(DEG_TO_RAD * (deg));
			y2 = y0 + H + H2 * sqrt(1-r2*r2/(R2*R2));

			x22 = x0 + r2 * sin(DEG_TO_RAD * (deg + 360/N));
			z22 = z0 - r2 * cos(DEG_TO_RAD * (deg + 360/N));
			glDisable(GL_BLEND);


			//glBegin(GL_QUADS);
			////glTexCoord2f(0, 0);
			//glVertex3f(x1,y0 + H,z1);
			//
			////glTexCoord2f(50, 0);
			//glVertex3f(x2,y0 + H,z2);
			//
			////glTexCoord2f(50, 50);
			//glVertex3f(x22,y0 + H,z22);
			//
			////glTexCoord2f(0, 50);
			//glVertex3f(x11,y0 + H,z11);
			//glEnd();
			if (tag == 1)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, ID[0]);
				/*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
					GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);*/
				//glColor3ub(255,255,255);
				float fl[] = {0.5f,0.5f,0.5f};
				glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,fl);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
				
				//glEnable(GL_AUTO_NORMAL);


			}
			////////////////////////////
			n1x = x1 - x2;
			n1y = y1 - y2;
			n1z = z1 - z2;

			n2x = x22 - x2;
			n2y = y2 - y2;
			n2z = z22 - z2;

			nx = n1y * n2z - n1z * n2y;
			ny = n1z * n2x - n1x * n2z;
			nz = n1x * n2y - n1y * n2x;


			glNormal3f(-nx,-ny,-nz);
			if ((R==6)||(R==4))
				glNormal3f(nx,ny,nz);
			///////////////////////////
			glBegin(GL_QUADS);

			glTexCoord2f(0, 0);
			glVertex3f(x1,y1,z1);

			glTexCoord2f(0.005, 0);
			//glEdgeFlag(TRUE);
			glVertex3f(x2,y2,z2);

			glTexCoord2f(0.005, 0.005);
			glVertex3f(x22,y2,z22);

			glTexCoord2f(0, 0.005);
			//glEdgeFlag(FALSE);
			glVertex3f(x11,y1,z11);

			glEnd();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glDisable(GL_TEXTURE_2D);

			
		}	
	}
	
}