// Mol_SimuDoc.h : interface of the CMol_SimuDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOL_SIMUDOC_H__F04806AA_DABA_4B71_989D_1E39FCCFE5E6__INCLUDED_)
#define AFX_MOL_SIMUDOC_H__F04806AA_DABA_4B71_989D_1E39FCCFE5E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Atom.h"
#include "Bound.h"
#include "Particle.h"
#include <vector>
using namespace std;

class CMol_SimuDoc : public CDocument
{
protected: // create from serialization only
	CMol_SimuDoc();
	DECLARE_DYNCREATE(CMol_SimuDoc)

// Attributes
public:
	vector <Atom> atom;
	vector <Atom> drawAtom; //仅仅用于显示的原子信息
	vector <Bound> bound;
	vector <POINT3D> vertex; //分子表面顶点
	vector <Triangle> triangle; //表面三角形
	double xMax, xMin;
	double yMax, yMin;
	double zMax, zMin;
	double totalMax;
	double xCenter, yCenter, zCenter;
	int GeoOp; 
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMol_SimuDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetNeighbors();
	void GetVDWNeighbors();
	void ConstructSurface();
	void clamp(float *v);
	bool isShowSurface;
	double minDistance;
	double m_axis[3];
	double size;
	int xNow, yNow, zNow;
	int xPre, yPre, zPre;
	double m_displace[3];
	double m_ratio;
	double m_spin;
	double  cosa, sina;              //旋转角余弦
	void scale();
	void translate();
	void rotate();
	void GetMax();
	void ConvertAtom();
	int BoundBegin;
	void GetBound();
	double GetCoordinate(CString s, int index);
	void GetAtom();
	CStringArray fileContent, AtomStrAr, BoundStrAr;
	virtual ~CMol_SimuDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMol_SimuDoc)
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOL_SIMUDOC_H__F04806AA_DABA_4B71_989D_1E39FCCFE5E6__INCLUDED_)
