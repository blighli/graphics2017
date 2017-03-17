// TESTView.h : interface of the CTESTView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__B910BDF8_6666_4C7F_A24D_236D6466A4F7__INCLUDED_)
#define AFX_TESTVIEW_H__B910BDF8_6666_4C7F_A24D_236D6466A4F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//OpenGL Headers
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <gl/glaux.h>
#include <mmsystem.h>


class CTESTView : public CView
{
protected: // create from serialization only
	CTESTView();
	DECLARE_DYNCREATE(CTESTView)

// Attributes
public:
	CTESTDoc* GetDocument();

// Operations
public:
	HGLRC m_hRC; //Rendering Context
	CDC* m_pDC; //Device Context
	
	GLfloat m_xAngle;
	GLfloat m_yAngle;
	GLfloat m_xPos;
	GLfloat m_yPos;
CPoint m_MouseDownPoint;

BOOL InitializeOpenGL(); //Initialize OpenGL
BOOL SetupPixelFormat(); //Set up the Pixel Format
void RenderScene(); //Render the Scene

public:
	DWORD m_StartTime;DWORD m_ElapsedTime;DWORD m_previousElapsedTime;
	CString m_WindowTitle; //Window Title
	int DayOfYear;
    int HourOfDay;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTESTView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	DWORD ElapsedTimeinMSSinceLastRender();
	DWORD ElapsedTimeinMSSinceLastStartup();
	int FramesPerSecond( void );
	void PostRenderScene(void);
	virtual ~CTESTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTESTView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TESTView.cpp
inline CTESTDoc* CTESTView::GetDocument()
   { return (CTESTDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTVIEW_H__B910BDF8_6666_4C7F_A24D_236D6466A4F7__INCLUDED_)
