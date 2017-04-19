// TESTView.cpp : implementation of the CTESTView class
//

#include "stdafx.h"
#include "TEST.h"

#include "TESTDoc.h"
#include "TESTView.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTESTView

IMPLEMENT_DYNCREATE(CTESTView, CView)

BEGIN_MESSAGE_MAP(CTESTView, CView)
	//{{AFX_MSG_MAP(CTESTView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTESTView construction/destruction

CTESTView::CTESTView()
{
	// TODO: add construction code here
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_xAngle = 0.0f;
    m_yAngle = 0.0f;

	DayOfYear = 1;
    HourOfDay = 1;

}

CTESTView::~CTESTView()
{
}

BOOL CTESTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
		cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTESTView drawing

void CTESTView::OnDraw(CDC* pDC)
{
	CTESTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	// Clear out the color & depth buffers

	

/*	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glLoadIdentity();
//	glTranslatef(-2.0f,-1.0f,-5.0f);
    //glRotatef(30.0f,0.0f,0.0f,0.0f);
	RenderScene();
	// Tell OpenGL to flush its pipeline
	::glFinish();
	// Now Swap the buffers
    ::SwapBuffers( m_pDC->GetSafeHdc() );*/

	
	m_ElapsedTime = ::timeGetTime(); // get current time
	//if ( ElapsedTimeinMSSinceLastRender() < 30 )
	//	return 
		// Clear out the color & depth buffers
    ::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();
	RenderScene();
	glPopMatrix();
	// Tell OpenGL to flush its pipeline
	::glFinish();
	// Now Swap the buffers
	::SwapBuffers( m_pDC->GetSafeHdc() );
	//Perform Post Display Processing
	// Only update the title every 15 redraws (this is about
	// every 1/2 second)
	PostRenderScene();
	// the very last thing we do is to save
	// the elapsed time, this is used with the
	// next elapsed time to calculate the
	// elapsed time since a render and the frame rate
	m_previousElapsedTime = m_ElapsedTime;
}
void CTESTView::RenderScene ()
{
	glTranslatef(0.0f,0.0f,-10.0f);
	//Draw the Sun
	glutWireSphere(1.0f,20,20);
	//Rotate the Planet in its orbit
	glRotatef((GLfloat) (360.0*DayOfYear)/365.0, 0.0f, 1.0f, 0.0f);
	glTranslatef(4.0f,0.0f,0.0f);
	glPushMatrix();
	//Rotate the Planet in its orbit
glRotatef((GLfloat)(360*HourOfDay)/24.0, 0.0f,1.0f,0.0f);

	
//Draw the Planet
glutWireSphere(0.2f,20,20);
glPopMatrix();
glRotatef((GLfloat) (360.0*12.5*DayOfYear)/365.0, 0.0f, 1.0f,0.0f);
glTranslatef(0.5f,0.0f,0.0f);
//Draw the Moon
glutWireSphere(0.01f,20,20);





}

/////////////////////////////////////////////////////////////////////////////
// CTESTView printing

BOOL CTESTView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTESTView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTESTView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTESTView diagnostics

#ifdef _DEBUG
void CTESTView::AssertValid() const
{
	CView::AssertValid();
}

void CTESTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTESTDoc* CTESTView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTESTDoc)));
	return (CTESTDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTESTView message handlers

void CTESTView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	GLdouble aspect_ratio; // width/height ratio
	if ( 0 >= cx || 0 >= cy )
	{
		return;
	}
	GLfloat light_pos[]={0.5,0,5,6.0,8.0};
	// select the full client area
	::glViewport(0, 0, cx, cy);
	// compute the aspect ratio
	// this will keep all dimension scales equal
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	aspect_ratio = (GLdouble)cx/(GLdouble)cy;
	// select the projection matrix and clear it
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	/*if(cx<=cy)

	::glOrtho(-10.0,10.0,-10.0*aspect_ratio,10.0*aspect_ratio,-10.0,10.0);//aspect_ratio
	else
    ::glOrtho(-10.0*aspect_ratio,10.0*aspect_ratio,-10.0,10.0,-10.0,-10.0);//aspect_ratio*/
	// select the viewing volume
	::gluPerspective(45.0f, aspect_ratio, .01f, 200.0f);
	// switch back to the modelview matrix and clear it
	::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();
    glLightfv(GL_LIGHT0,GL_POSITION,light_pos);

	
}

BOOL CTESTView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
}

int CTESTView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	 SetTimer(1,1,NULL);//启动定时器1,定时时间是1秒
	InitializeOpenGL();

	
	return 0;
}

void CTESTView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	if(::wglMakeCurrent (0,0) == FALSE)
	{
		MessageBox("Could not make RC non-current");
	}
	//Delete the rendering context
	if(::wglDeleteContext (m_hRC)==FALSE)
	{
		MessageBox("Could not delete RC");
	}
	//Delete the DC
	if(m_pDC)
	{
		delete m_pDC;
	}
	//Set it to NULL
    m_pDC = NULL;
	
}
BOOL CTESTView::InitializeOpenGL()
{
	//Get a DC for the Client Area
	m_pDC = new CClientDC(this);
	//Failure to Get DC
	if(m_pDC == NULL)
	{
		MessageBox("Error Obtaining DC");
		return FALSE;
	}
	//Failure to set the pixel format
	if(!SetupPixelFormat())
	{
		return FALSE;
	}
	//Create Rendering Context
	m_hRC = ::wglCreateContext (m_pDC->GetSafeHdc ());
	//Failure to Create Rendering Context
	if(m_hRC == 0)
	{
		MessageBox("Error Creating RC");
		return FALSE;
	}
	//Make the RC Current
	if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC)==FALSE)
	{
		MessageBox("Error making RC Current");
		return FALSE;
	}
	//Specify Black as the clear color
	::glClearColor(0.0f,0.0f,0.0f,0.0f);
	//Specify the back of the buffer as clear depth
	::glClearDepth(1.0f);
	//Enable Depth Testing
	::glEnable(GL_DEPTH_TEST);
//	glShadeModel(GL_SMOOTH);
	return TRUE;
}
//Setup Pixel Format
BOOL CTESTView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
			1, // version number
			PFD_DRAW_TO_WINDOW | // support window
			PFD_SUPPORT_OPENGL | // support OpenGL
			PFD_DOUBLEBUFFER, // double buffered
			PFD_TYPE_RGBA, // RGBA type
			24, // 24-bit color depth
			0, 0, 0, 0, 0, 0, // color bits ignored
			0, // no alpha buffer
			0, // shift bit ignored
			0, // no accumulation buffer
			0, 0, 0, 0, // accum bits ignored
			16, // 16-bit z-buffer
			0, // no stencil buffer
			0, // no auxiliary buffer
			PFD_MAIN_PLANE, // main layer
			0, // reserved
			0, 0, 0 // layer masks ignored
	};
	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	if ( m_nPixelFormat == 0 )
	{
		return FALSE;
	}
	if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd)== FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

/*void CTESTView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetCapture()==this)
	{
		//Increment the object rotation angles
		m_xAngle+=(point.y-m_MouseDownPoint.y)/3.6;
		m_yAngle+=(point.x-m_MouseDownPoint.x)/3.6;
		//Redraw the view
		InvalidateRect(NULL,FALSE);
		//Set the mouse point
		m_MouseDownPoint=point;
	}
	
	CView::OnMouseMove(nFlags, point);
}

void CTESTView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_MouseDownPoint=CPoint(0,0);
    ReleaseCapture();
	
	CView::OnLButtonUp(nFlags, point);
}

void CTESTView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_MouseDownPoint=point;
    SetCapture();
	
	CView::OnLButtonDown(nFlags, point);
}

void CTESTView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case VK_UP: m_yPos = m_yPos + 0.1f;
		break;
	case VK_DOWN: m_yPos = m_yPos - 0.1f;
		break;
	case VK_LEFT: m_xPos = m_xPos - 0.1f;
		break;
     case VK_RIGHT: m_xPos = m_xPos + 0.1f;
		 break;
	 default: MessageBox("Press the arrow keys only");
		 break;
	}
     InvalidateRect(NULL,FALSE);
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}*/

void CTESTView::PostRenderScene(void)
{
	static int updateFrame = 15;
	if (16 > ++updateFrame )
		return;
	updateFrame = 0;
	char string[256];
	_snprintf( string, 200, "%s ( %d Frames/sec )",(const char*)m_WindowTitle, FramesPerSecond());
     GetParentFrame()->SetWindowText( string );
}

int CTESTView::FramesPerSecond(void)
{
	double eTime = ElapsedTimeinMSSinceLastRender();
	if ( 0 == (int)eTime )
		return 0;
     return (int)(1000/(int)eTime);
}


void CTESTView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(DayOfYear < 365)
		DayOfYear++;
	else
		DayOfYear = 1;
	if(HourOfDay < 365)
		HourOfDay++;
	else
		HourOfDay = 1;
InvalidateRect(NULL, FALSE);
	
	CView::OnTimer(nIDEvent);
}

DWORD CTESTView::ElapsedTimeinMSSinceLastStartup()
{
	return(m_ElapsedTime-m_StartTime);
}

DWORD CTESTView::ElapsedTimeinMSSinceLastRender()
{
return(m_ElapsedTime-m_previousElapsedTime);
}
