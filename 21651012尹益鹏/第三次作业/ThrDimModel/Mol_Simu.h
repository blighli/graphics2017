// Mol_Simu.h : main header file for the MOL_SIMU application
//

#if !defined(AFX_MOL_SIMU_H__9071A8AD_862E_4E1C_9386_9A2128E37CB3__INCLUDED_)
#define AFX_MOL_SIMU_H__9071A8AD_862E_4E1C_9386_9A2128E37CB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <GL/glut.h>
//include "matlib.h"

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuApp:
// See Mol_Simu.cpp for the implementation of this class
//

class CMol_SimuApp : public CWinApp
{
public:
	CMol_SimuApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMol_SimuApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMol_SimuApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOL_SIMU_H__9071A8AD_862E_4E1C_9386_9A2128E37CB3__INCLUDED_)
