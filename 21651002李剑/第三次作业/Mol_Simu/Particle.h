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
	double x, y, z;//λ��
	double r; // �뾶
	int atom; //������ԭ��
	int type; //��¼�����ӵ����ͣ����������㣻�����߽�㣻�����ڵ�
	vector <int> neighArea;//�������������Ӻ�.

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
