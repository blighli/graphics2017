// Mol_SimuView.cpp : implementation of the CMol_SimuView class
//

#include "stdafx.h"
#include "Mol_Simu.h"

#include "Mol_SimuDoc.h"
#include "Mol_SimuView.h"

#include <math.h>
#include <time.h>		
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PI 3.14159265358979

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuView

IMPLEMENT_DYNCREATE(CMol_SimuView, CView)

BEGIN_MESSAGE_MAP(CMol_SimuView, CView)
//{{AFX_MSG_MAP(CMol_SimuView)
ON_WM_SIZE()
ON_WM_ERASEBKGND()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_rotate, Onrotate)
ON_COMMAND(ID_translate, Ontranslate)
ON_COMMAND(ID_scale, Onscale)
ON_COMMAND(ID_VDWsurface, OnVDWsurface)
	ON_COMMAND(ID_PolySurf, OnPolySurf)
	ON_COMMAND(ID_RationalPolySurf, OnRationalPolySurf)
	ON_COMMAND(ID_InitialMesh, OnInitialMesh)
	//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuView construction/destruction

//���ù���
//���ģ�Ͳ�͸��
GLfloat mat_diffuseC[] = { 0.5, 0.5, 0.5, 1.0};//C�û�ɫ
GLfloat mat_diffuseO[] = { 0.7, 0.0, 0.0, 1.0};//O�ú�ɫ
GLfloat mat_diffuseN[] = { 0.0, 1.0, 0.0, 1.0};//N����ɫ
GLfloat mat_diffuseH[] = { 1.0, 1.0, 1.0, 1.0};//H�ð�ɫ
GLfloat mat_diffuseS[] = { 0.9, 0.9, 0.0, 1.0 };//S�û�ɫ
//�����͸��
GLfloat mat_diffuseBS[] = { 0.0, 0.0, 0.6, 0.4 };//��ɫ
GLfloat mat_diffuseCS[] = { 0.6, 0.6, 0.6, 0.6 };//C�û�ɫ
GLfloat mat_diffuseOS[] = { 1.0, 0.0, 0.0, 0.6 };//O�ú�ɫ
GLfloat mat_diffuseNS[] = { 0.0, 1.0, 0.0, 0.6 };//N����ɫ
GLfloat mat_diffuseHS[] = { 1.0, 1.0, 1.0, 0.6 };//H�ð�ɫ
GLfloat mat_diffuseSS[] = { 0.9, 0.9, 0.0, 0.6 };//S�û�ɫ
GLfloat mat_diffuseGold[] = { 0.83, 0.52, 0, 1.0 };//��ɫ

/* creates a enum type for mouse buttons */
enum {
    BUTTON_LEFT = 0,
		BUTTON_RIGHT,
		BUTTON_LEFT_TRANSLATE,
};

/* set global variables */
int mButton = -1;
int mOldY, mOldX;

/* vectors that makes the rotation and translation of the cube */
float eye[3] = {0.0f, 0.0f, 0.0f};
float rot[3] = {0.0f, 0.0f, 0.0f};

CMol_SimuView::CMol_SimuView()
{
	IsMousePress = false;
	isFirstSurfShow = false;
	isSecondSurfShow = false;
}

CMol_SimuView::~CMol_SimuView()
{
}

BOOL CMol_SimuView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuView drawing

void CMol_SimuView::OnDraw(CDC* pDC)
{
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	m_pDC = new CClientDC(this);
	ASSERT(m_pDC != NULL); 
	
	SetupPixelFormat();
	//����DC������RC
	m_hRC = wglCreateContext(m_pDC->m_hDC); 
	//���õ�ǰ��RC���Ժ�Ļ�ͼ����������m_pDCָ���DC��
	wglMakeCurrent(m_pDC->m_hDC, m_hRC);//ʹ RC �뵱ǰ DC ����� 
	//DrawScene();
	//���泬�޲�ֵ
	ShowSurf();
	//OnInitialMesh();
	SwapBuffers(m_pDC->GetSafeHdc());
	pDC->TextOut(0, 0, strFrameRate);
	wglMakeCurrent (NULL, NULL) ; 
	// delete the rendering context 
	wglDeleteContext(m_hRC); 
	if (!m_pDC)
	{
		delete(m_pDC);
		m_pDC = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuView printing

BOOL CMol_SimuView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMol_SimuView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMol_SimuView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuView diagnostics

#ifdef _DEBUG
void CMol_SimuView::AssertValid() const
{
	CView::AssertValid();
}

void CMol_SimuView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMol_SimuDoc* CMol_SimuView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMol_SimuDoc)));
	return (CMol_SimuDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuView message handlers

void CMol_SimuView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	//add down
	int w=cx;
	int h=cy;
	//�������Ϊ0
	if(h==0) h=1;

	//�����ӿ��봰��ƥ��
	glViewport(0,0,w,h);

	//������������ϵͳ
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//����͸��ͶӰ
	gluPerspective(30.0,(GLfloat)w/(GLfloat)h,1.0,20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//add up
}

bool CMol_SimuView::SetupPixelFormat()
{
	//����һ�����ظ�ʽ
	static PIXELFORMATDESCRIPTOR pfd =
	{   
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
			1,  // version number
			PFD_DRAW_TO_WINDOW |// support window
			PFD_SUPPORT_OPENGL |  // support OpenGL  ֧��OpenGL
			PFD_DOUBLEBUFFER, // double buffered ֧���ֻ���
			PFD_TYPE_RGBA,  // RGBA typeʹ��RGBAģʽ�����õ�ɫ��
			24, // 24-bit color depth  ʹ��24λ���ɫ
			0, 0, 0, 0, 0, 0,   // color bits ignored
			0,  // no alpha buffer
			0,  // shift bit ignored
			0,  // no accumulation buffer
			0, 0, 0, 0, // accum bits ignored
			32, // 32-bit z-buffer   32λZ�Ỻ��
			0,  // no stencil buffer
			0,  // no auxiliary buffer
			PFD_MAIN_PLANE, // main layer
			0,  // reserved
			0, 0, 0 // layer masks ignored
	};
	int pixelformat;
	
	//������Եõ�ָ����
	if ( (pixelformat = ChoosePixelFormat(m_pDC->m_hDC, &pfd)) == FALSE )
	{
		AfxMessageBox("ChoosePixelFormat failed");
		return false;
	}
	
	//������ȡ���ĸ�ʽ�����豸����
	if (SetPixelFormat(m_pDC->m_hDC, pixelformat, &pfd) == FALSE)
	{
		AfxMessageBox("SetPixelFormat failed");
		return false;
	}
	return true;
}

void CMol_SimuView::DrawScene()
{
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�������ģ����ԭ�ӵİ뾶
	double lx = 0, ly = 0, lz = 0;
	minDistance = 1000;
	for (int i = 0; i < pDoc->AtomStrAr.GetSize(); i++)
	{
		double x = pDoc->drawAtom[i].GetX();
		double y = pDoc->drawAtom[i].GetY();
		double z = pDoc->drawAtom[i].GetZ();
		if (i > 0 && i < pDoc->AtomStrAr.GetSize())
		{
			double distance = sqrt((x - lx) * (x - lx) +
				(y - ly) * (y - ly) + 
				(z - lz) * (z - lz));
			if (minDistance > distance) minDistance = distance;	
		}
		lx = x;
		ly = y;
		lz = z;
	}
	pDoc->minDistance = minDistance;

	InitGL();
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	double fac = PI / 180;
	/* translate camera to the coordinates provided by eye array */
	glTranslatef (-eye[0], -eye[1], -eye[2]);
	
	/* rotates the screen by the angles provided by rot array */
	glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
	glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
	glRotatef(rot[2], 0.0f, 0.0f, cos(rot[0] * fac) * sin(rot[1] * fac));

	
// 	GLfloat light_position[] = {sin(rot[1] * fac), sin(rot[0] * fac), 
// 								cos(rot[0] * fac) * cos(rot[1] * fac), 0.0};
// 	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	//���Ʒ��ӱ���
	if (pDoc->isShowSurface) 
	{
		drawBound();
		//if (!IsMousePress) {
			drawAtom();
			drawSurface();
		//}
	}
	else
	{
		drawBound();
		if (!IsMousePress) {
			drawAtom();
		}
	}
	// ��õ�ǰϵͳʱ��
	fTime = static_cast<float>(timeGetTime()) * 0.006f;
	fEndFrameTime = fTime/6;
	// ����֡��	
	if ((fEndFrameTime - fStartFrameTime) >= 1.0f) 
	{
		//////////////begin
		for(int i=0; i<50; i++) strFrameRate[i] = '\0';
		sprintf(strFrameRate, "[FPS: %d]", iFrames);
		/////////////////end
		iFrames = 0;
		fStartFrameTime = fTime/6;
	}
	else iFrames++;
	glFlush();
}

void CMol_SimuView::OnDestroy()
{
	CView::OnDestroy();
	HGLRC   hrc;
	
	hrc = ::wglGetCurrentContext();
	
	::wglMakeCurrent(NULL,  NULL);
	
	if (hrc)
		::wglDeleteContext(hrc);
}

BOOL CMol_SimuView::OnEraseBkgnd(CDC* pDC) 
{
	//return CView::OnEraseBkgnd(pDC);
	return true;
}

/*
�˺�������ԭ��
*/
void CMol_SimuView::drawAtom()
{
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	glMatrixMode(GL_MODELVIEW);
	//������ģ��
	for (int i = 0; i < pDoc->AtomStrAr.GetSize(); i++)
	{
		double x = pDoc->drawAtom[i].GetX();
		double y = pDoc->drawAtom[i].GetY();
		double z = pDoc->drawAtom[i].GetZ();
		glLoadIdentity();
		double fac = PI / 180;
		/* translate camera to the coordinates provided by eye array */
		glTranslatef (-eye[0], -eye[1], -eye[2]);
	
		/* rotates the screen by the angles provided by rot array */
		glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
		glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
		glRotatef(rot[2], 0.0f, 0.0f, cos(rot[0] * fac) * sin(rot[1] * fac));
		glTranslatef(x, y, z);
		int w = pDoc->drawAtom[i].GetWeight();
		switch(w)
		{
		case 12:
			{
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseC);
				break;
			}
		case 14:
			{
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseN);
				break;
			}
		case 8:
			{
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseO);
				break;
			}
		case 1:
			{
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseH);
				break;
			}
		}
		//auxSolidSphere(minDistance / 4);
		glutSolidSphere(minDistance / 4, 8, 8);
	}
}

void CMol_SimuView::drawBound()
{
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	//ǰ�����λ�ѧ������ɫ
	GLfloat mat_diffuse1[4];
	GLfloat mat_diffuse2[4];

	//GLfloat mat_diffuse2[] = { 0.0, 0.0, 1.0, 1.0 };
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);//����ѧ������ɫ

	glMatrixMode(GL_MODELVIEW);
	int num = pDoc->bound.size();
	double x1, y1, z1, x2, y2, z2, xv, yv, zv, distance, alpha;
	int i1, i2, type;
	for (int i = 0; i < num; i++)
	{
		//���ü��β�������
		glLoadIdentity();
		double fac = PI / 180;
		/* translate camera to the coordinates provided by eye array */
		glTranslatef (-eye[0], -eye[1], -eye[2]);
		/* rotates the screen by the angles provided by rot array */
		glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
		glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
		glRotatef(rot[2], 0.0f, 0.0f, cos(rot[0] * fac) * sin(rot[1] * fac));

		i1 = pDoc->bound[i].getStart() - 1;
		i2 = pDoc->bound[i].getEnd() - 1;
		type = pDoc->bound[i].getType();
		
		//������ʹ�����ֲ�ͬ����ɫ
		//������ʼԭ�ӵ�ʹ�ú���ʼԭ����ͬ����ɫ
		//������ֹԭ�ӵ�ʹ�ú���ֹԭ����ͬ����ɫ
		int w1=pDoc->drawAtom[i1].GetWeight();
		int w2=pDoc->drawAtom[i2].GetWeight();
		switch(w1)
		{
			case 32:
			{
				mat_diffuse1[1] =mat_diffuseS[1];
				mat_diffuse1[2] =mat_diffuseS[2];
				mat_diffuse1[3] =mat_diffuseS[3];
				mat_diffuse1[0] =mat_diffuseS[0];					
				break;
			}
			case 12:
			{
				mat_diffuse1[1] =mat_diffuseC[1];
				mat_diffuse1[2] =mat_diffuseC[2];
				mat_diffuse1[3] =mat_diffuseC[3];
				mat_diffuse1[0] =mat_diffuseC[0];
				break;
			}
			case 14:
			{
				mat_diffuse1[1] =mat_diffuseN[1];
				mat_diffuse1[2] =mat_diffuseN[2];
				mat_diffuse1[3] =mat_diffuseN[3];
				mat_diffuse1[0] =mat_diffuseN[0];			
 				break;
			}
			case 8:
			{
				mat_diffuse1[1] =mat_diffuseO[1];
				mat_diffuse1[2] =mat_diffuseO[2];
				mat_diffuse1[3] =mat_diffuseO[3];
				mat_diffuse1[0] =mat_diffuseO[0];
				break;
			}
			case 1:
			{
				mat_diffuse1[1] =mat_diffuseH[1];
				mat_diffuse1[2] =mat_diffuseH[2];
				mat_diffuse1[3] =mat_diffuseH[3];
				mat_diffuse1[0] =mat_diffuseH[0];
				break;
			}
			default:
				mat_diffuse1[1] =mat_diffuseGold[1];
				mat_diffuse1[2] =mat_diffuseGold[2];
				mat_diffuse1[3] =mat_diffuseGold[3];
				mat_diffuse1[0] =mat_diffuseGold[0];
			}
		switch(w2)
		{
			case 32:
			{
				mat_diffuse2[1] =mat_diffuseS[1];
				mat_diffuse2[2] =mat_diffuseS[2];
				mat_diffuse2[3] =mat_diffuseS[3];
				mat_diffuse2[0] =mat_diffuseS[0];					
				break;
			}
			case 12:
			{
				mat_diffuse2[1] =mat_diffuseC[1];
				mat_diffuse2[2] =mat_diffuseC[2];
				mat_diffuse2[3] =mat_diffuseC[3];
				mat_diffuse2[0] =mat_diffuseC[0];
				break;
			}
			case 14:
			{
				mat_diffuse2[1] =mat_diffuseN[1];
				mat_diffuse2[2] =mat_diffuseN[2];
				mat_diffuse2[3] =mat_diffuseN[3];
				mat_diffuse2[0] =mat_diffuseN[0];			
 				break;
			}
			case 8:
			{
				mat_diffuse2[1] =mat_diffuseO[1];
				mat_diffuse2[2] =mat_diffuseO[2];
				mat_diffuse2[3] =mat_diffuseO[3];
				mat_diffuse2[0] =mat_diffuseO[0];
				break;
			}
			case 1:
			{
				mat_diffuse2[1] =mat_diffuseH[1];
				mat_diffuse2[2] =mat_diffuseH[2];
				mat_diffuse2[3] =mat_diffuseH[3];
				mat_diffuse2[0] =mat_diffuseH[0];
				break;
			}
			default:
				mat_diffuse2[1] =mat_diffuseGold[1];
				mat_diffuse2[2] =mat_diffuseGold[2];
				mat_diffuse2[3] =mat_diffuseGold[3];
				mat_diffuse2[0] =mat_diffuseGold[0];
		}			
			
		//��ʼԭ��
		x1 = pDoc->drawAtom[i1].GetX();
		y1 = pDoc->drawAtom[i1].GetY();
		z1 = pDoc->drawAtom[i1].GetZ();
		
		//��ֹԭ��
		x2 = pDoc->drawAtom[i2].GetX();
		y2 = pDoc->drawAtom[i2].GetY();
		z2 = pDoc->drawAtom[i2].GetZ();

		//������
		xv = x2 - x1;
		yv = y2 - y1;
		zv = z2 - z1;
		distance = sqrt(xv * xv + yv * yv + zv * zv);

		//Ҫ��ת�ĽǶ�
		alpha = acos(zv / distance * 1) / PI * 180;
		//��0��0��1���루xv, yv, zv���Ĳ����-yvi+xvj,����������ת��

		//��Բ��������ѧ��
		if (type == 2)
		{
			glTranslatef(x1, y1, z1);
			glTranslatef(-minDistance / 12, 0, 0);
			glRotatef(alpha, -yv, xv, 0);
			//��ǰ��λ�ѧ��
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
			GLUquadric* qobj = gluNewQuadric();		
			gluCylinder(qobj, minDistance / 12, minDistance / 12, distance / 2, 5, 4);
			gluDeleteQuadric(qobj);
			glTranslatef(minDistance / 6, 0, 0);
			GLUquadric* qobj1 = gluNewQuadric();		
			gluCylinder(qobj1, minDistance / 12, minDistance / 12, distance / 2, 5, 4);
			gluDeleteQuadric(qobj1);

 			glLoadIdentity(); // ����ģ�͹۲����
			double fac = PI / 180;
			/* translate camera to the coordinates provided by eye array */
			glTranslatef (-eye[0], -eye[1], -eye[2]);
			/* rotates the screen by the angles provided by rot array */
			glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
			glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
			glRotatef(rot[2], 0.0f, 0.0f, cos(rot[0] * fac) * sin(rot[1] * fac));
			glTranslatef(-xv/2, -yv/2, -zv/2);

			//�����λ�ѧ��
			glTranslatef(x2, y2, z2);
			glTranslatef(-minDistance / 12, 0, 0);
			glRotatef(alpha, -yv, xv, 0);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
			GLUquadric* qobj2 = gluNewQuadric();		
			gluCylinder(qobj2, minDistance / 12, minDistance / 12, distance / 2, 5, 4);
			gluDeleteQuadric(qobj2);
			glTranslatef(minDistance / 6, 0, 0);
			GLUquadric* qobj3 = gluNewQuadric();		
			gluCylinder(qobj3, minDistance / 12, minDistance / 12, distance / 2, 5, 4);
			gluDeleteQuadric(qobj3);
		}
		else if (type == 3)
		{
		/*glTranslatef(-pDoc->minDistance / 18, 0, 0);
		glRotatef(alpha, -yv, xv, 0);
		GLUquadric* qobj = gluNewQuadric();		
		gluCylinder(qobj, pDoc->minDistance / 12, pDoc->minDistance / 12, distance, 15, 5);
		gluDeleteQuadric(qobj);
		glTranslatef(pDoc->minDistance / 6, 0, 0);
		GLUquadric* qobj2 = gluNewQuadric();		
		gluCylinder(qobj2, pDoc->minDistance / 12, pDoc->minDistance / 12, distance, 15, 5);
			gluDeleteQuadric(qobj2);*/
		}
		else //����
		{
			glTranslatef(x1, y1, z1);
			glRotatef(alpha, -yv, xv, 0);
			//��ǰ��λ�ѧ��
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
			GLUquadric* qobj = gluNewQuadric();		
			gluCylinder(qobj, pDoc->minDistance / 12, pDoc->minDistance / 12, distance / 2, 5, 4);
			gluDeleteQuadric(qobj);

			glLoadIdentity(); // ����ģ�͹۲����
			double fac = PI / 180;
			/* translate camera to the coordinates provided by eye array */
			glTranslatef (-eye[0], -eye[1], -eye[2]);
			/* rotates the screen by the angles provided by rot array */
			glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
			glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
			glRotatef(rot[2], 0.0f, 0.0f, cos(rot[0] * fac) * sin(rot[1] * fac));
			glTranslatef(-xv/2, -yv/2, -zv/2);
			glTranslatef(x2, y2, z2);
			glRotatef(alpha, -yv, xv, 0);
			
			//�����λ�ѧ��
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
			GLUquadric* qobj2 = gluNewQuadric();		
			gluCylinder(qobj2, pDoc->minDistance / 12, pDoc->minDistance / 12, distance / 2, 5, 4);
			gluDeleteQuadric(qobj2);
		}
	}
}

void CMol_SimuView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// TODO: Add your message handler code here and/or call default
	IsMousePress = true;
	xPre = point.x;
	yPre = point.y;
	xPre = (long)(xPre - 200);
	yPre = (long)(200 - yPre);
	zPre = (400 * 400 + 400 * 400)/4;
	zPre = zPre - (xPre * xPre + yPre * yPre);
	if(zPre > 0)
		zPre = (long)sqrt((double)zPre);
	else
		zPre = 0;
	pDoc->xPre = xPre;
	pDoc->yPre = yPre;
	pDoc->zPre = zPre;
	CView::OnLButtonDown(nFlags, point);
}

void CMol_SimuView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	::ClipCursor(NULL);
	IsMousePress = false;
	CView::OnLButtonUp(nFlags, point);
	InvalidateRect(0);
}

void CMol_SimuView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// TODO: Add your message handler code here and/or call default
	if (IsMousePress)
	{
		xNow = point.x;
		yNow = point.y;
		///////////////////////////////////////////////////////////
		//���������Ļ�ϵ��ƶ��������ڣ��֣������ϵ��ƶ�
		//��ʼת������,��ƽ������ϵת��Ϊԭ���ڿͻ������ĵ���ά�ѿ�������
		//���������ϵ������
		xNow = xNow - 200;
		yNow = 200 - yNow;
		zNow = (400 * 400 + 400 * 400) / 4;
		zNow = zNow - (xNow * xNow + yNow * yNow);
		if(zNow > 0)
			zNow = (long)sqrt((double)zNow);
		else
			zNow=0;
		pDoc->xNow = xNow;
		pDoc->yNow = yNow;
		pDoc->zNow = zNow;
		/////����ת�����///////////////////////
		pDoc->m_displace[0] = (xNow - xPre) / (double)400;
		pDoc->m_displace[1] = (yNow - yPre) / (double)400;
		pDoc->m_displace[2] = 0;
		switch(pDoc->GeoOp)
		{
		case 0: //��ת
			{
				pDoc->rotate();
				break;
			}
		case 1: //ƽ��
			{
				pDoc->translate();
				break;
			}
		case 2: //����
			{
				pDoc->scale();
				break;
			}
		}
	}
	CView::OnMouseMove(nFlags, point);
}

void CMol_SimuView::Onrotate() 
{
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	pDoc->GeoOp = 0;
	this->GetParent()->GetMenu()->CheckMenuItem(ID_rotate, MF_BYCOMMAND | MF_CHECKED);
	this->GetParent()->GetMenu()->GetSubMenu(1)->CheckMenuItem(ID_scale, MF_BYCOMMAND | MF_UNCHECKED);
	this->GetParent()->GetMenu()->GetSubMenu(1)->CheckMenuItem(ID_translate, MF_BYCOMMAND | MF_UNCHECKED);
}

void CMol_SimuView::Ontranslate() 
{
	// TODO: Add your command handler code here
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	pDoc->GeoOp = 1;
	this->GetParent()->GetMenu()->CheckMenuItem(ID_rotate, MF_BYCOMMAND | MF_UNCHECKED);
	this->GetParent()->GetMenu()->CheckMenuItem(ID_scale, MF_BYCOMMAND | MF_UNCHECKED);
	this->GetParent()->GetMenu()->CheckMenuItem(ID_translate, MF_BYCOMMAND | MF_CHECKED);
}

void CMol_SimuView::Onscale() 
{
	// TODO: Add your command handler code here
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	pDoc->GeoOp = 2;
	this->GetParent()->GetMenu()->CheckMenuItem(ID_rotate, MF_BYCOMMAND | MF_UNCHECKED);
	this->GetParent()->GetMenu()->CheckMenuItem(ID_scale, MF_BYCOMMAND | MF_CHECKED);
	this->GetParent()->GetMenu()->CheckMenuItem(ID_translate, MF_BYCOMMAND | MF_UNCHECKED);
}

void CMol_SimuView::InitGL()
{
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	//��Դλ��
	GLfloat light_position[] = { 0, 0, 10, 1.0 };//�����Դ
	//��Դ����
	GLfloat spot_direction[] = {0, 0, -1};
	//������ɫ�Ĺ���
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat blue_light[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 0.0 };
	//ָ������Ĳ�������
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	//ָ����Դλ�ú�����
// 	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 100.0);
// 	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);//ɢ������ɫ
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light); //���淴������ɫ
	//ָ������ģ�Ͳ���(ȫ�ֻ�����)
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	//���ӵ�
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	//���ù�Դ
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//������ȼ��������
	glEnable(GL_DEPTH_TEST);
	//���û��ģʽ
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDrawBuffer(GL_FRONT_AND_BACK);
	//�����Ļ������
	//glClearColor(0.7, 0.6, 1.0, 1.0);
	//glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double size;
// 	size = pDoc->totalMax * 5;
// 	pDoc->size = size;
// 	glOrtho(-size, size, 
// 			-size, size, 
// 			-size * 20, size * 20);
}

/*
�ú�����ʾ���ӵķ��»�����
*/
void CMol_SimuView::OnVDWsurface() 
{
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->isShowSurface = !pDoc->isShowSurface;
	pDoc->GetMax();
	Invalidate(true);
}

void CMol_SimuView::drawSurface()
{
	CMol_SimuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	glMatrixMode(GL_MODELVIEW);
	glLineWidth(1.0);
	int s = pDoc->triangle.size();
	Triangle t;
	POINT3D n;
	for (int i = 0; i < s; i++)
	{
		t = pDoc->triangle[i];
		switch(t.drawAtomWeight)
		{
		case 12:
			{
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseCS);
				break;
			}
		case 14:
			{
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseNS);
				break;
			}
		case 8:
			{
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseOS);
				break;
			}
		case 1:
			{
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseHS);
				break;
			}
		}
		glLoadIdentity();
		double fac = PI / 180;
		/* translate camera to the coordinates provided by eye array */
		glTranslatef (-eye[0], -eye[1], -eye[2]);
	
		/* rotates the screen by the angles provided by rot array */
		glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
		glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
		glRotatef(rot[2], 0.0f, 0.0f, cos(rot[0] * fac) * sin(rot[1] * fac));
		glBegin(GL_TRIANGLES);
			Atom a = pDoc->drawAtom[t.atomNum];
			n = t.normal;
			//glNormal3f(n.x, n.y, n.z);
			glNormal3f(t.GetA().normal.x, t.GetA().normal.y, t.GetA().normal.z);
			glVertex3f(t.GetA().p.x, t.GetA().p.y, t.GetA().p.z);
			glNormal3f(t.GetB().normal.x, t.GetB().normal.y, t.GetB().normal.z);
			glVertex3f(t.GetB().p.x, t.GetB().p.y, t.GetB().p.z);
			glNormal3f(t.GetC().normal.x, t.GetC().normal.y, t.GetC().normal.z);
			glVertex3f(t.GetC().p.x, t.GetC().p.y, t.GetC().p.z);
		glEnd();
// 		glBegin(GL_TRIANGLES);
// 			n = t.normal;
// 			glNormal3f(n.x, n.y, n.z);
// 			glVertex3f(t.GetA().p.x, t.GetA().p.y, t.GetA().p.z);
// 			glVertex3f(t.GetB().p.x, t.GetB().p.y, t.GetB().p.z);
// 			glVertex3f(t.GetC().p.x, t.GetC().p.y, t.GetC().p.z);
// 		glEnd();
// 		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseGold);
// 		glBegin(GL_LINES);
// 			n = t.normal;
// 			glNormal3f(n.x, n.y, n.z);
// 			glVertex3f(t.GetA().p.x, t.GetA().p.y, t.GetA().p.z);
// 			glVertex3f(t.GetB().p.x, t.GetB().p.y, t.GetB().p.z);
// 			glVertex3f(t.GetC().p.x, t.GetC().p.y, t.GetC().p.z);
// 			glVertex3f(t.GetA().p.x, t.GetA().p.y, t.GetA().p.z);
// 		glEnd();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseGold);
		glPointSize(3.0);
		if (t.isBorder)
		{
			int lSize = t.intersectLines.size();
			for (int j = 0; j < t.intersectLines.size(); j++)
			{
				glBegin(GL_POINTS);
					glNormal3f(n.x, n.y, n.z);
					glVertex3f(t.intersectLines[j].sP.p.x, t.intersectLines[j].sP.p.y, t.intersectLines[j].sP.p.z);
					glVertex3f(t.intersectLines[j].eP.p.x, t.intersectLines[j].eP.p.y, t.intersectLines[j].eP.p.z);
				glEnd();
			}
		}
	}
}

void CMol_SimuView::OnPolySurf() 
{
	isFirstSurfShow = !isFirstSurfShow;
}

double CMol_SimuView::Func1(double x, double y)
{
	double z = (x * x - y * y) * (x + y) * (x - 0.25) * (y - 0.2);
	return z;
}

double CMol_SimuView::Func2(double x, double y) 
{
	double z = (1201246200 * y * y * y * x * x * x * x - 
		1042072290 * y * y * y * x * x * x - 3144253140 * y * y * x * x * x * x + 
		797796225 * y * y * y * x * x + 1657228725 * y * y * x * x * x + 
		2339355240 * y * x * x * x * x + 249793902 * y * y * y * x - 
		1039620528 * y * y * x * x - 669606102 * y * x * x * x - 351710892 * x * x * x * x - 
		100720728 * y * y * y - 378142148 * y * y * x + 186027543 * y * x * x + 
		87927723 * x * x * x + 145899860 * y * y + 94870190 * y * x - 
		34019780 * y) / ((6673590 * y * y - 16133355 * y + 9769747) * 180);
	return z;
}

/************************************************************************/
/* ���޲�ֵ                                                             */
/************************************************************************/
void CMol_SimuView::ShowSurf()
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	//��Դλ��
	GLfloat light_position[] = { 0.2, 0.2, -1.0, 0.0 };//�����Դ
	//��Դ����
	GLfloat spot_direction[] = {0, 0, 1};
	//������ɫ�Ĺ���
	GLfloat white_light[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat yellow_light[] = { 1.0, 1.0, 0.5, 1.0 };
	GLfloat blue_light[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 0.0 };
	//ָ������Ĳ�������
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	//ָ����Դλ�ú�����
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);//ɢ������ɫ
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light); //���淴������ɫ
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellow_light);//ɢ������ɫ
	glLightfv(GL_LIGHT1, GL_SPECULAR, yellow_light); //���淴������ɫ
	//ָ������ģ�Ͳ���(ȫ�ֻ�����)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	//���ӵ�
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	//���ù�Դ
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//������ȼ��������
	glEnable(GL_DEPTH_TEST);
	//�����Ļ������
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double size = 1;
	glOrtho(-size, size, -size, size, -size, size);
	
	double fac = PI / 180;
	/* translate camera to the coordinates provided by eye array */
	glTranslatef (-eye[0], -eye[1], -eye[2]);
	
	/* rotates the screen by the angles provided by rot array */
	glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
	glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
	glRotatef(rot[2], 0.0f, 0.0f, cos(rot[0] * fac) * sin(rot[1] * fac));
	
	//GLfloat light_position[] = {sin(rot[1] * fac), sin(rot[0] * fac), 
	//							cos(rot[0] * fac) * cos(rot[1] * fac), 0.0};
	//GLfloat light_position[] = {0, 0, -10, 0.0};
	//GLfloat light_position1[] = {0, 0, -10, 0.0};
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLineWidth(2.5);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//0 <= x <= 1, 0 <= y <= 1
	//x������ϸ�֣���������������.����101*101������		
	//��ÿһ���������ƽ��
	double x1 = 0, y1 = 0, x2, y2;
	//���㵱ǰСƽ���ĸ�����ģ�ֵ
	double z11, z12, z22, z21;
	//Сƽ���ĸ�����
	POINT3D p[4];
	int divideNum = 100;
	if (isFirstSurfShow)
	{
		//��z = (x^2 - y^2) * (x + y) * (x - 1/4) * (y - 1/5)
		for (int i = 0; i < divideNum; i++)
		{
			x1 = (double)i / divideNum; x2 = x1 + 1.0 / divideNum;
			for (int j = 0; j < divideNum; j++)
			{
				y1 = j * (double)1.0 / divideNum; y2 = y1 + 1.0 / divideNum;
				z11 = Func1(x1, y1); z12 = Func1(x1, y2);
				z22 = Func1(x2, y2); z21 = Func1(x2, y1);
				//�õ�Сƽ�涥��
				p[0].SetXYZ(x1, y1, z11);
				p[1].SetXYZ(x1, y2, z12);
				p[2].SetXYZ(x2, y2, z22);
				p[3].SetXYZ(x2, y1, z21);
				//��������Ƭ
				//����������(����������), (0, 2, 3)
				//�����θ�������
				POINT3D EdgeV01, EdgeV20, EdgeV02 ,EdgeV23;
				EdgeV01.SetXYZ(p[1].x - p[0].x, p[1].y - p[0].y, p[1].z - p[0].z);
				EdgeV20.SetXYZ(p[0].x - p[2].x, p[0].y - p[2].y, p[0].z - p[2].z);
				EdgeV02.SetXYZ(p[2].x - p[0].x, p[2].y - p[0].y, p[2].z - p[0].z);
				EdgeV23.SetXYZ(p[3].x - p[2].x, p[3].y - p[2].y, p[3].z - p[2].z);
				//�ñߵĲ���õ�����������
				POINT3D n1, n2;
				n1.SetXYZ(EdgeV20.y * EdgeV01.z - EdgeV01.y * EdgeV20.z, 
						  -EdgeV20.x * EdgeV01.z + EdgeV01.x * EdgeV20.z,
						  EdgeV20.x * EdgeV01.y - EdgeV01.x * EdgeV20.y);
				n2.SetXYZ(EdgeV02.y * EdgeV23.z - EdgeV23.y * EdgeV02.z, 
						  -EdgeV02.x * EdgeV23.z + EdgeV23.x * EdgeV02.z,
						  EdgeV02.x * EdgeV23.y - EdgeV23.x * EdgeV02.y);
				//������һ��Ҫ��һ������������
				double d = sqrt(n1.x * n1.x + n1.y * n1.y + n1.z * n1.z);
				n1.SetXYZ(n1.x / d, n1.y / d, n1.z / d);
				d = sqrt(n2.x * n2.x + n2.y * n2.y + n2.z * n2.z);
				n2.SetXYZ(n2.x / d, n2.y / d, n2.z / d);
				//��������Ƭ
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseO);
				glBegin(GL_TRIANGLES);
					glNormal3f(n1.x, n1.y, n1.z);
					glVertex3f(p[0].x - 0.5, p[0].y - 0.5, p[0].z);
					glVertex3f(p[1].x - 0.5, p[1].y - 0.5, p[1].z);
					glVertex3f(p[2].x - 0.5, p[2].y - 0.5, p[2].z);
				glEnd();
				glBegin(GL_TRIANGLES);
					glNormal3f(n2.x, n2.y, n1.z);
					glVertex3f(p[0].x - 0.5, p[0].y - 0.5, p[0].z);
					glVertex3f(p[2].x - 0.5, p[2].y - 0.5, p[2].z);
					glVertex3f(p[3].x - 0.5, p[3].y - 0.5, p[3].z);
				glEnd();
				//���Ʊ����ϵ�����
				if (y1 == 0 || y1 == 0.2 || y1 == 0.4 || y1 == 0.6 || y1 == 0.8)
				{
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseN);
					glBegin(GL_LINES);
						glVertex3f(p[3].x - 0.5, p[3].y - 0.5, p[3].z);
						glVertex3f(p[0].x - 0.5, p[0].y - 0.5, p[0].z);
					glEnd();
				}
				if (y2 == 1.0)
				{
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseN);
					glBegin(GL_LINES);
						glVertex3f(p[2].x - 0.5, p[2].y - 0.5, p[2].z);
						glVertex3f(p[1].x - 0.5, p[1].y - 0.5, p[1].z);
					glEnd();

				}
			}
		}
	}
	if (isSecondSurfShow)
	{
		//������ͬ���ķ������ڶ�������
		for (int i = 0; i < divideNum; i++)
		{
			x1 = (double)i / divideNum; x2 = x1 + 1.0 / divideNum;
			for (int j = 0; j < divideNum; j++)
			{
				y1 = j * (double)1.0 / divideNum; y2 = y1 + 1.0 / divideNum;
				z11 = Func2(x1, y1); z12 = Func2(x1, y2);
				z22 = Func2(x2, y2); z21 = Func2(x2, y1);
				//�õ�Сƽ�涥��
				p[0].SetXYZ(x1, y1, z11);
				p[1].SetXYZ(x1, y2, z12);
				p[2].SetXYZ(x2, y2, z22);
				p[3].SetXYZ(x2, y1, z21);
				//��������Ƭ
				//����������(����������), (0, 2, 3)
				//�����θ�������
				POINT3D EdgeV01, EdgeV20, EdgeV02 ,EdgeV23;
				EdgeV01.SetXYZ(p[1].x - p[0].x, p[1].y - p[0].y, p[1].z - p[0].z);
				EdgeV20.SetXYZ(p[0].x - p[2].x, p[0].y - p[2].y, p[0].z - p[2].z);
				EdgeV02.SetXYZ(p[2].x - p[0].x, p[2].y - p[0].y, p[2].z - p[0].z);
				EdgeV23.SetXYZ(p[3].x - p[2].x, p[3].y - p[2].y, p[3].z - p[2].z);
				//�ñߵĲ���õ�����������
				POINT3D n1, n2;
				n1.SetXYZ(EdgeV20.y * EdgeV01.z - EdgeV01.y * EdgeV20.z, 
						  -EdgeV20.x * EdgeV01.z + EdgeV01.x * EdgeV20.z,
						  EdgeV20.x * EdgeV01.y - EdgeV01.x * EdgeV20.y);
				n2.SetXYZ(EdgeV02.y * EdgeV23.z - EdgeV23.y * EdgeV02.z, 
						  -EdgeV02.x * EdgeV23.z + EdgeV23.x * EdgeV02.z,
						  EdgeV02.x * EdgeV23.y - EdgeV23.x * EdgeV02.y);
				//������һ��Ҫ��һ������������
				double d = sqrt(n1.x * n1.x + n1.y * n1.y + n1.z * n1.z);
				n1.SetXYZ(n1.x / d, n1.y / d, n1.z / d);
				d = sqrt(n2.x * n2.x + n2.y * n2.y + n2.z * n2.z);
				n2.SetXYZ(n2.x / d, n2.y / d, n2.z / d);
				//��������Ƭ
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseH);
				glBegin(GL_TRIANGLES);
					glNormal3f(n1.x, n1.y, n1.z);
					glVertex3f(p[0].x - 0.5, p[0].y - 0.5, p[0].z);
					glVertex3f(p[1].x - 0.5, p[1].y - 0.5, p[1].z);
					glVertex3f(p[2].x - 0.5, p[2].y - 0.5, p[2].z);
				glEnd();
				glBegin(GL_TRIANGLES);
					glNormal3f(n2.x, n2.y, n1.z);
					glVertex3f(p[0].x - 0.5, p[0].y - 0.5, p[0].z);
					glVertex3f(p[2].x - 0.5, p[2].y - 0.5, p[2].z);
					glVertex3f(p[3].x - 0.5, p[3].y - 0.5, p[3].z);
				glEnd();
				//���Ʊ����ϵ�����
				if (y1 == 0 || y1 == 0.2 || y1 == 0.4 || y1 == 0.6 || y1 == 0.8 || y2 == 1)
				{
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseN);
					glBegin(GL_LINES);
						glVertex3f(p[3].x - 0.5, p[3].y - 0.5, p[3].z);
						glVertex3f(p[0].x - 0.5, p[0].y - 0.5, p[0].z);
					glEnd();
				}
				if (y2 == 1.0)
				{
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseN);
					glBegin(GL_LINES);
						glVertex3f(p[2].x - 0.5, p[2].y - 0.5, p[2].z);
						glVertex3f(p[1].x - 0.5, p[1].y - 0.5, p[1].z);
					glEnd();

				}
			}
		}
	}
	glFlush();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double s = 1.0;
	glOrtho(-s, s, -s, s, -s, s);
}

void CMol_SimuView::OnRationalPolySurf() 
{
	// TODO: Add your command handler code here
	isSecondSurfShow = !isSecondSurfShow;
}

/*************************************************************************/
/* ���Ƴ�ʼ���ƶ���������                                                 */
/************************************************************************/
void CMol_SimuView::OnInitialMesh() 
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	//��Դλ��
	GLfloat light_position[] = { 0, 0, -10.0, 0.0 };//�����Դ
	//��Դ����
	GLfloat spot_direction[] = {0, 0, 1};
	//������ɫ�Ĺ���
	GLfloat white_light[] = { 0.2, 0.2, 0.2, 0.0 };
	GLfloat yellow_light[] = { 1.0, 1.0, 0.5, 1.0 };
	GLfloat blue_light[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 0.0 };
	//ָ������Ĳ�������
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	//ָ����Դλ�ú�����
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);//ɢ������ɫ
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light); //���淴������ɫ
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellow_light);//ɢ������ɫ
	glLightfv(GL_LIGHT1, GL_SPECULAR, yellow_light); //���淴������ɫ
	//ָ������ģ�Ͳ���(ȫ�ֻ�����)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	//���ӵ�
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	//���ù�Դ
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//������ȼ��������
	glEnable(GL_DEPTH_TEST);
	//�����Ļ������
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLineWidth(6);

	double size = 1.0;
	glOrtho(-size, size, -size, size, -size, size);
	
	double fac = PI / 180;
	/* translate camera to the coordinates provided by eye array */
	glTranslatef (-eye[0], -eye[1], -eye[2]);
	
	/* rotates the screen by the angles provided by rot array */
	glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
	glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
	glRotatef(rot[2], 0.0f, 0.0f, cos(rot[0] * fac) * sin(rot[1] * fac));
	
	//GLfloat light_position[] = {sin(rot[1] * fac), sin(rot[0] * fac), 
	//							cos(rot[0] * fac) * cos(rot[1] * fac), 0.0};
	//GLfloat light_position[] = {0, 0, -10, 0.0};
	//GLfloat light_position1[] = {0, 0, -10, 0.0};
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
// 	//����Bezier���߿��ƶ�������
// 	GLfloat ctrlPoints1[4][3];
// 	for (int i = 0; i < 3; i++)
// 	{
// 		for (int j = 0; j < 4; j++)
// 		{
// 			//����Bezier���߿��ƶ���
// 			ctrlPoints1[j][0] = p.x; ctrlPoints1[j][1] = p.y; ctrlPoints1[j][2] = p.z;
// 		}
// 		//����u��Bezier����
// 		glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlPoints1[0][0]);
// 		glEnable(GL_MAP1_VERTEX_3);
// 		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseN);
// 		glLoadIdentity();
// 		//���й���ģʽ�£��κζ�����Ҫָʾ���򣡣���
// 		glBegin(GL_LINE_STRIP);
// 		glNormal3f(0, 0, 1);
// 			for (j = 0; j <= 100; j++)
// 				glEvalCoord1f((GLfloat) j / 100);
// 		glEnd();
// 	}

	//��ʼ�����������߿��ƶ���
	ctrlPnts[0][0].SetXYZ(-0.4, -0.6, -0.4); ctrlPnts[1][0].SetXYZ(-0.55, 0.2, 0.1); ctrlPnts[2][0].SetXYZ(-0.4, 0.6, 0.7);
	ctrlPnts[0][1].SetXYZ(-0.1, -0.3, 0); ctrlPnts[1][1].SetXYZ(-0.25, 0.2, -0.05); ctrlPnts[2][1].SetXYZ(-0.1, 0.6, 0);
	ctrlPnts[0][2].SetXYZ(0.3, -0.2, 0.15); ctrlPnts[1][2].SetXYZ(0.45, 0.3, 0); ctrlPnts[2][2].SetXYZ(0.3, 0.65, 0.05);
	ctrlPnts[0][3].SetXYZ(0.6, -0.2, -0.9); ctrlPnts[1][3].SetXYZ(0.82, 0.2, 0.05); ctrlPnts[2][3].SetXYZ(0.58, 0.9, -0.8);

	//����Bezier���߿��ƶ�������
	GLfloat ctrlPoints2[3][3];
	for (int i = 0; i < 4; i ++)//��
	{
		for (int j = 0; j < 3; j++)//��
		{
			//���ƣ£���������߿��ƶ���
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseO);
			POINT3D p = ctrlPnts[j][i];
			glLoadIdentity();
			glTranslatef(p.x, p.y, p.z);
			glutSolidSphere(0.025, 8, 8);
			//v��Bezier���߿��ƶ���
			ctrlPoints2[j][0] = p.x; ctrlPoints2[j][1] = p.y; ctrlPoints2[j][2] = p.z;
		}
		//���ƣ���Bezier����
		glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrlPoints2[0][0]);
		glEnable(GL_MAP1_VERTEX_3);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseN);
		glLoadIdentity();
		//����v����ƶ���
		glBegin(GL_LINE_STRIP);
			glNormal3f(0, 0, -1);
			glVertex3f(ctrlPoints2[0][0], ctrlPoints2[0][1], ctrlPoints2[0][2]);
			glVertex3f(ctrlPoints2[1][0], ctrlPoints2[1][1], ctrlPoints2[1][2]);
			glVertex3f(ctrlPoints2[2][0], ctrlPoints2[2][1], ctrlPoints2[2][2]);
		glEnd();
		glLoadIdentity();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseC);
		glBegin(GL_LINE_STRIP);
		//���й���ģʽ�£��κζ�����Ҫָʾ���򣡣���
		glNormal3f(0, 0, -1);
			for (j = 0; j <= 100; j++)
				glEvalCoord1f((GLfloat) j / 100);
		glEnd();
	}
	//��������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//0 <= u <= 1, 0 <= v <= 1
	//x������ϸ�֣���������������.����101*101������		
	//��ÿһ���������ƽ��
	double u1 = 0, v1 = 0, u2, v2;
	//���㵱ǰСƽ���ĸ������x, y,��ֵ
	double x11, x12, x22, x21;
	double y11, y12, y22, y21;
	double z11, z12, z22, z21;
	//Сƽ���ĸ�����
	POINT3D p[4];
	int divideNum = 100;
	if (isFirstSurfShow)
	{
		for (int i = 0; i < divideNum; i++)
		{
			u1 = (double)i / divideNum; u2 = u1 + 1.0 / divideNum;
			for (int j = 0; j < divideNum; j++)
			{
				v1 = j * (double)1.0 / divideNum; v2 = v1 + 1.0 / divideNum;
				//������
				x11 = FuncX(u1, v1); x12 = FuncX(u1, v2);
				x22 = FuncX(u2, v2); x21 = FuncX(u2, v1);
				//������
				y11 = FuncY(u1, v1); y12 = FuncY(u1, v2);
				y22 = FuncY(u2, v2); y21 = FuncY(u2, v1);
				//������
				z11 = FuncZ(u1, v1); z12 = FuncZ(u1, v2);
				z22 = FuncZ(u2, v2); z21 = FuncZ(u2, v1);
				//�õ�Сƽ�涥��
				p[0].SetXYZ(x11, y11, z11);
				p[1].SetXYZ(x12, y12, z12);
				p[2].SetXYZ(x22, y22, z22);
				p[3].SetXYZ(x21, y21, z21);
				//��������Ƭ
				//����������(����������), (0, 2, 3)
				//�����θ�������
				POINT3D EdgeV01, EdgeV20, EdgeV02 ,EdgeV23;
				EdgeV01.SetXYZ(p[1].x - p[0].x, p[1].y - p[0].y, p[1].z - p[0].z);
				EdgeV20.SetXYZ(p[0].x - p[2].x, p[0].y - p[2].y, p[0].z - p[2].z);
				EdgeV02.SetXYZ(p[2].x - p[0].x, p[2].y - p[0].y, p[2].z - p[0].z);
				EdgeV23.SetXYZ(p[3].x - p[2].x, p[3].y - p[2].y, p[3].z - p[2].z);
				//�ñߵĲ���õ�����������
				POINT3D n1, n2;
				n1.SetXYZ(EdgeV20.y * EdgeV01.z - EdgeV01.y * EdgeV20.z, 
						  -EdgeV20.x * EdgeV01.z + EdgeV01.x * EdgeV20.z,
						  EdgeV20.x * EdgeV01.y - EdgeV01.x * EdgeV20.y);
				n2.SetXYZ(EdgeV02.y * EdgeV23.z - EdgeV23.y * EdgeV02.z, 
						  -EdgeV02.x * EdgeV23.z + EdgeV23.x * EdgeV02.z,
						  EdgeV02.x * EdgeV23.y - EdgeV23.x * EdgeV02.y);
				//������һ��Ҫ��һ������������
				double d = sqrt(n1.x * n1.x + n1.y * n1.y + n1.z * n1.z);
				n1.SetXYZ(n1.x / d, n1.y / d, n1.z / d);
				d = sqrt(n2.x * n2.x + n2.y * n2.y + n2.z * n2.z);
				n2.SetXYZ(n2.x / d, n2.y / d, n2.z / d);
				//��������Ƭ
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseH);
				glBegin(GL_TRIANGLES);
					glNormal3f(n1.x, n1.y, n1.z);
					glVertex3f(p[0].x, p[0].y, p[0].z);
					glVertex3f(p[1].x, p[1].y, p[1].z);
					glVertex3f(p[2].x, p[2].y, p[2].z);
				glEnd();
				glBegin(GL_TRIANGLES);
					glNormal3f(n2.x, n2.y, n1.z);
					glVertex3f(p[0].x, p[0].y, p[0].z);
					glVertex3f(p[2].x, p[2].y, p[2].z);
					glVertex3f(p[3].x , p[3].y, p[3].z);
				glEnd();
			}
		}
	}
	if (isSecondSurfShow)
	{
		for (int i = 0; i < divideNum; i++)
		{
			u1 = (double)i / divideNum; u2 = u1 + 1.0 / divideNum;
			for (int j = 0; j < divideNum; j++)
			{
				v1 = j * (double)1.0 / divideNum; v2 = v1 + 1.0 / divideNum;
				//������
				x11 = GordonFuncX(u1, v1); x12 = GordonFuncX(u1, v2);
				x22 = GordonFuncX(u2, v2); x21 = GordonFuncX(u2, v1);
				//������
				y11 = GordonFuncY(u1, v1); y12 = GordonFuncY(u1, v2);
				y22 = GordonFuncY(u2, v2); y21 = GordonFuncY(u2, v1);
				//������
				z11 = GordonFuncZ(u1, v1); z12 = GordonFuncZ(u1, v2);
				z22 = GordonFuncZ(u2, v2); z21 = GordonFuncZ(u2, v1);
				//�õ�Сƽ�涥��
				p[0].SetXYZ(x11, y11, z11);
				p[1].SetXYZ(x12, y12, z12);
				p[2].SetXYZ(x22, y22, z22);
				p[3].SetXYZ(x21, y21, z21);
				//��������Ƭ
				//����������(����������), (0, 2, 3)
				//�����θ�������
				POINT3D EdgeV01, EdgeV20, EdgeV02 ,EdgeV23;
				EdgeV01.SetXYZ(p[1].x - p[0].x, p[1].y - p[0].y, p[1].z - p[0].z);
				EdgeV20.SetXYZ(p[0].x - p[2].x, p[0].y - p[2].y, p[0].z - p[2].z);
				EdgeV02.SetXYZ(p[2].x - p[0].x, p[2].y - p[0].y, p[2].z - p[0].z);
				EdgeV23.SetXYZ(p[3].x - p[2].x, p[3].y - p[2].y, p[3].z - p[2].z);
				//�ñߵĲ���õ�����������
				POINT3D n1, n2;
				n1.SetXYZ(EdgeV20.y * EdgeV01.z - EdgeV01.y * EdgeV20.z, 
						  -EdgeV20.x * EdgeV01.z + EdgeV01.x * EdgeV20.z,
						  EdgeV20.x * EdgeV01.y - EdgeV01.x * EdgeV20.y);
				n2.SetXYZ(EdgeV02.y * EdgeV23.z - EdgeV23.y * EdgeV02.z, 
						  -EdgeV02.x * EdgeV23.z + EdgeV23.x * EdgeV02.z,
						  EdgeV02.x * EdgeV23.y - EdgeV23.x * EdgeV02.y);
				//������һ��Ҫ��һ������������
				double d = sqrt(n1.x * n1.x + n1.y * n1.y + n1.z * n1.z);
				n1.SetXYZ(n1.x / d, n1.y / d, n1.z / d);
				d = sqrt(n2.x * n2.x + n2.y * n2.y + n2.z * n2.z);
				n2.SetXYZ(n2.x / d, n2.y / d, n2.z / d);
				//��������Ƭ
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuseO);
				glBegin(GL_TRIANGLES);
					glNormal3f(n1.x, n1.y, -n1.z);
					glVertex3f(p[0].x, p[0].y, p[0].z);
					glVertex3f(p[1].x, p[1].y, p[1].z);
					glVertex3f(p[2].x, p[2].y, p[2].z);
				glEnd();
				glBegin(GL_TRIANGLES);
					glNormal3f(n2.x, n2.y, -n1.z);
					glVertex3f(p[0].x, p[0].y, p[0].z);
					glVertex3f(p[2].x, p[2].y, p[2].z);
					glVertex3f(p[3].x , p[3].y, p[3].z);
				glEnd();
			}
		}
	}
	glFlush();
}

double CMol_SimuView::FuncX(double u, double v)
{
	double ans;
	ans = -0.4 * u * u - 1.10 * u * (1 - u) - 0.4 * (1 - u) * (1 - u)
		+ 4 * v / (-2.249689987 * v - 4.314449847 * v * v * v + 1.586442779 * v * v * v * v
		+ 4.587693897 * v * v + 0.4313756300) * 
		(-1.244435872 * v+ 0.4770041938 + 0.9068223295 * v * v) * 
		(-1.244435872 * v+ 0.4770041938 + 0.9068223295 * v * v) * 
		(0.4322766570 * u* u + 0.8645533140 * u * (1 - u) + 
		0.4322766570 * (1 - u) * (1 - u) + (v - 0.3333333333)
		/ (-24.88871745 * v + 9.540083876 + 18.13644659 * v * v)
		* (0.433691757 * u * u + 2.359856632 * u*(1 - u) + 0.433691757 * (1 - u) * (1 - u)
		+ (18.13644660 * v - 12.09096440) * 
		((-0.252753353e-2) * u * u + (0.205540849e-1) * u * (1-u) - (0.989194625e-2)*(1-u)*(1-u))));
	return ans;
}

double CMol_SimuView::FuncY(double u, double v)
{
	double ans;
	ans = -0.6*u*u + 0.4*u*(1-u) + 0.6*(1-u)*(1-u) + 
		4 * v / (-2.249689987*v-4.314449847*v*v*v + 1.586442779*v*v*v*v+4.587693897*v*v+0.4313756300)
		* (-1.244435872*v+0.4770041938+0.9068223295*v*v) * 
		(-1.244435872*v+0.4770041938+0.9068223295*v*v) * (0.4322766570*u*u + (v - 0.3333333333) /
		(-24.88871745*v+9.540083876+18.13644659*v*v) * (-0.3125448018*u*u+0.4974910393*u*(1-u) + 
		0.1243727598*(1-u)*(1-u) + (18.13644660*v-12.09096440) *
		(-0.1000228571*u*u + (0.8989334901e-1) * (1-u)*(1-u) - (0.8229137439e-1) * u * (1-u))));
	return ans;
}

double CMol_SimuView::FuncZ(double u, double v)
{
	double ans;
	ans = -0.4*u*u + 0.2*u*(1-u) + 0.7*(1-u)*(1-u) + 4 * v / 
		(-2.249689987*v-4.314449847*v*v*v + 1.586442779*v*v*v*v + 4.587693897*v*v + 0.43137563)
		* (-1.244435872*v+0.4770041938+0.9068223295*v*v) * (-1.244435872*v+0.4770041938+0.9068223295*v*v)
		* (0.5763688760*u*u - 0.4322766570*u*(1-u) - 1.008645533 * (1-u)*(1-u) + (v-.3333333333) 
		/(-24.88871745*v+9.540083876+18.13644659*v*v) * (-0.375268816*u*u + 0.8100358407*u*(1-u)
		+ 1.434050177*(1-u)*(1-u) + (18.13644660*v - 12.09096440) * 
		(-0.5207160873*u*u + 0.1281976752*u*(1-u) - (0.918511446e-1) * (1-u)*(1-u))));
	return ans;
}

double CMol_SimuView::GordonFuncX(double u, double v)
{
	double ans;
	double l0, l1, l2, l3;
	l0 = (v - 1.0/3) * (v - 2.0/3) * (v - 1) / ((0 - 1.0/3) * (0 - 2.0/3) * (0 - 1));
	l1 = (v - 0) * (v - 2.0/3) * (v - 1) / ((1.0/3 - 0) * (1.0/3 - 2.0/3) * (1.0/3 - 1));
	l2 = (v - 0) * (v - 1.0/3) * (v - 1) / ((2.0/3 - 0) * (2.0/3 - 1.0/3) * (2.0/3 - 1));
	l3 = (v - 0) * (v - 1.0/3) * (v - 2.0/3) / ((1 - 0) * (1 - 1.0/3) * (1 - 2.0/3));
	double guv0, guv1, guv2, guv3;
	guv0 = ctrlPnts[0][0].x * (1-u) * (1-u) + ctrlPnts[1][0].x * 2 * u * (1-u) + ctrlPnts[2][0].x * u * u;
	guv1 = ctrlPnts[0][1].x * (1-u) * (1-u) + ctrlPnts[1][1].x * 2 * u * (1-u) + ctrlPnts[2][1].x * u * u;
	guv2 = ctrlPnts[0][2].x * (1-u) * (1-u) + ctrlPnts[1][2].x * 2 * u * (1-u) + ctrlPnts[2][2].x * u * u;
	guv3 = ctrlPnts[0][3].x * (1-u) * (1-u) + ctrlPnts[1][3].x * 2 * u * (1-u) + ctrlPnts[2][3].x * u * u;
	ans = guv0 * l0 + guv1 * l1 + guv2 * l2 + guv3 * l3;
	return ans;
}

double CMol_SimuView::GordonFuncY(double u, double v)
{
	double ans;
	double l0, l1, l2, l3;
	l0 = (v - 1.0/3) * (v - 2.0/3) * (v - 1) / ((0 - 1.0/3) * (0 - 2.0/3) * (0 - 1));
	l1 = (v - 0) * (v - 2.0/3) * (v - 1) / ((1.0/3 - 0) * (1.0/3 - 2.0/3) * (1.0/3 - 1));
	l2 = (v - 0) * (v - 1.0/3) * (v - 1) / ((2.0/3 - 0) * (2.0/3 - 1.0/3) * (2.0/3 - 1));
	l3 = (v - 0) * (v - 1.0/3) * (v - 2.0/3) / ((1 - 0) * (1 - 1.0/3) * (1 - 2.0/3));
	double guv0, guv1, guv2, guv3;
	guv0 = ctrlPnts[0][0].y * (1-u) * (1-u) + ctrlPnts[1][0].y * 2 * u * (1-u) + ctrlPnts[2][0].y * u * u;
	guv1 = ctrlPnts[0][1].y * (1-u) * (1-u) + ctrlPnts[1][1].y * 2 * u * (1-u) + ctrlPnts[2][1].y * u * u;
	guv2 = ctrlPnts[0][2].y * (1-u) * (1-u) + ctrlPnts[1][2].y * 2 * u * (1-u) + ctrlPnts[2][2].y * u * u;
	guv3 = ctrlPnts[0][3].y * (1-u) * (1-u) + ctrlPnts[1][3].y * 2 * u * (1-u) + ctrlPnts[2][3].y * u * u;
	ans = guv0 * l0 + guv1 * l1 + guv2 * l2 + guv3 * l3;
	return ans;
}

double CMol_SimuView::GordonFuncZ(double u, double v)
{
	double ans;
	double l0, l1, l2, l3;
	l0 = (v - 1.0/3) * (v - 2.0/3) * (v - 1) / ((0 - 1.0/3) * (0 - 2.0/3) * (0 - 1));
	l1 = (v - 0) * (v - 2.0/3) * (v - 1) / ((1.0/3 - 0) * (1.0/3 - 2.0/3) * (1.0/3 - 1));
	l2 = (v - 0) * (v - 1.0/3) * (v - 1) / ((2.0/3 - 0) * (2.0/3 - 1.0/3) * (2.0/3 - 1));
	l3 = (v - 0) * (v - 1.0/3) * (v - 2.0/3) / ((1 - 0) * (1 - 1.0/3) * (1 - 2.0/3));
	double guv0, guv1, guv2, guv3;
	guv0 = ctrlPnts[0][0].z * (1-u) * (1-u) + ctrlPnts[1][0].z * 2 * u * (1-u) + ctrlPnts[2][0].z * u * u;
	guv1 = ctrlPnts[0][1].z * (1-u) * (1-u) + ctrlPnts[1][1].z * 2 * u * (1-u) + ctrlPnts[2][1].z * u * u;
	guv2 = ctrlPnts[0][2].z * (1-u) * (1-u) + ctrlPnts[1][2].z * 2 * u * (1-u) + ctrlPnts[2][2].z * u * u;
	guv3 = ctrlPnts[0][3].z * (1-u) * (1-u) + ctrlPnts[1][3].z * 2 * u * (1-u) + ctrlPnts[2][3].z * u * u;
	ans = guv0 * l0 + guv1 * l1 + guv2 * l2 + guv3 * l3;
	return ans;
}
