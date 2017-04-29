#if !defined(AFX_PARTICLE_H__A21F16A3_0B97_4E1E_B350_D721F484F17A__INCLUDED_)
#define AFX_PARTICLE_H__A21F16A3_0B97_4E1E_B350_D721F484F17A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Particle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParticle window
#include <vector>
using namespace std;

class CParticle
{
// Construction
public:
	CParticle();
	CParticle(double, double, double, double, int, int);

// Attributes
public:
	double x, y, z;//位置
	double r; // 半径
	int atom; //所属的原子
	int type; //记录该粒子的类型：０－孤立点；１－边界点；２－内点
	vector <int> neighArea;//存放领域里的粒子号.

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParticle)
	//}}AFX_VIRTUAL

// Implementation
public:
	void setParticle(double, double, double, double, int, int);
	virtual ~CParticle();

	// Generated message map functions
protected:
	//{{AFX_MSG(CParticle)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTICLE_H__A21F16A3_0B97_4E1E_B350_D721F484F17A__INCLUDED_)
