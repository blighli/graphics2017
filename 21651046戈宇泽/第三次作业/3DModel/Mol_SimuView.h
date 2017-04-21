// Mol_SimuView.h : interface of the CMol_SimuView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOL_SIMUVIEW_H__7F644A22_17BB_4CB1_9152_C9890E36D8E3__INCLUDED_)
#define AFX_MOL_SIMUVIEW_H__7F644A22_17BB_4CB1_9152_C9890E36D8E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

static char strFrameRate[50];

class CMol_SimuView : public CView
{
private:
	bool isFirstSurfShow, isSecondSurfShow; 
protected: // create from serialization only
	CMol_SimuView();
	CClientDC *m_pDC;
	HGLRC m_hRC;
	DECLARE_DYNCREATE(CMol_SimuView)

// Attributes
public:
	//曲面控制顶点:3x4个
	POINT3D ctrlPnts[3][4];
	CMol_SimuDoc* GetDocument();
	// 帧的长度计算
	float fStartFrameTime;
	float fEndFrameTime;
	float fTime;
	// 当前激活的预设
	int iFrames ;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMol_SimuView)
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
	double GordonFuncZ(double, double);
	double GordonFuncY(double, double);
	double GordonFuncX(double,double);
	double FuncZ(double,double);
	double FuncY(double,double);
	double FuncX(double,double);
	void ShowSurf();
	double Func2(double, double);
	double Func1(double, double);
	void drawSurface();
	void InitGL();
	int yPre, yNow;
	int xPre, xNow;
	int zPre, zNow;	
	bool IsMousePress;
	double minDistance;
	void drawBound();
	void drawAtom();
	virtual ~CMol_SimuView();
	bool SetupPixelFormat();
	void DrawScene();
	void OnDestroy();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMol_SimuView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void Onrotate();
	afx_msg void Ontranslate();
	afx_msg void Onscale();
	afx_msg void OnVDWsurface();
	afx_msg void OnPolySurf();
	afx_msg void OnRationalPolySurf();
	afx_msg void OnInitialMesh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Mol_SimuView.cpp
inline CMol_SimuDoc* CMol_SimuView::GetDocument()
   { return (CMol_SimuDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOL_SIMUVIEW_H__7F644A22_17BB_4CB1_9152_C9890E36D8E3__INCLUDED_)
