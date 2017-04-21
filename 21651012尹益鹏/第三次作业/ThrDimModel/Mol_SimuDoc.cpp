// Mol_SimuDoc.cpp : implementation of the CMol_SimuDoc class
//

#include "stdafx.h"
#include "Mol_Simu.h"

#include <math.h>
#include "Mol_SimuDoc.h"
#include "MyObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuDoc

IMPLEMENT_DYNCREATE(CMol_SimuDoc, CDocument)

BEGIN_MESSAGE_MAP(CMol_SimuDoc, CDocument)
	//{{AFX_MSG_MAP(CMol_SimuDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuDoc construction/destruction
#define PI 3.14159265358979
 
CMol_SimuDoc::CMol_SimuDoc()
{
	// TODO: add one-time construction code here
	//��ʼԭ�Ӻͼ�������
	atom.clear();
	drawAtom.clear();
	atom.resize(10);
	drawAtom.resize(10);
	bound.resize(10);
	triangle.resize(0);
	vertex.resize(0);
	isShowSurface = FALSE;
	//Ĭ�ϼ��β���Ϊ��ת
	GeoOp = 0;
	m_ratio = 1;
	m_displace[0] = 0;
	m_displace[1] = 0;
	m_displace[2] = 0;
	minDistance = 0;
}

CMol_SimuDoc::~CMol_SimuDoc()
{
}

BOOL CMol_SimuDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuDoc serialization

void CMol_SimuDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuDoc diagnostics

#ifdef _DEBUG
void CMol_SimuDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMol_SimuDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMol_SimuDoc commands


void CMol_SimuDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString s;
	atom.clear();
	drawAtom.clear();
	isShowSurface = false;
	//���ļ��Ի���
	static char BASED_CODE szFilter[] = 
		"MOL Files (*.mol)|*.mol||";
	CFileDialog ofn(true, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, 
		szFilter);
	ofn.DoModal();
	CString fn = ofn.GetPathName();//�ļ���
	if (fn != "") //�ж��Ƿ�����ļ�
	{
		extern float rot[3], eye[3];
		for (int in = 0; in < 3; in++)
		{
			rot[in] = 0;
			eye[in] = 0;
		}
		fileContent.RemoveAll();//�������ԭ������
		CFile f;
		//���ļ�
		if (!f.Open(fn, CFile::modeRead | CFile::typeBinary | CFile::shareExclusive))
		{
			s.Format("File Open Failed: %s", fn);
			AfxMessageBox(s);
		}
		f.SeekToBegin();
		DWORD m_nLen = f.GetLength();
		char str[100000];
		f.Read(&str, m_nLen);
		CString tempStr, lineStr = "";
		for (int i = 0; i < (int) m_nLen; i++)
		{
			tempStr.Format("%c", str[i]);
			if (tempStr != "\n") lineStr = lineStr + tempStr;
			else 
			{
				fileContent.Add(lineStr);
				lineStr = "";
			}
		}
		f.Close();
		GetAtom();//��ȡԭ����Ϣ
		GetBound();//��ȡ��ѧ����Ϣ
		GetMax();
		//GetNeighbors();
		//�������ú��ڽ�ԭ����Ϣ
		GetVDWNeighbors();
		ConstructSurface();
		UpdateAllViews(NULL);
	}
}

/*
	�ú����õ�ԭ�ӵ���Ϣ
*/
void CMol_SimuDoc::GetAtom()
{
	AtomStrAr.RemoveAll();
	bool IsAtom = false;
	bool AtomComplete = false; //�ж��Ƿ��Ѿ�����ԭ�ӵ���Ϣ
	bool IsLastAtom = false; //�ж��Ƿ�Ϊ���һ��ԭ��
	for (int i = 1; i < fileContent.GetSize() - 1; i++)//ȡ����ԭ����ص���Ϣ
	{
		CString lineStr = "";
		CString temp = fileContent.GetAt(i);
		CString next = fileContent.GetAt(i + 1);
		CString previous = fileContent.GetAt(i - 1);
		IsAtom = false;
		if (!IsLastAtom) 
		{
			if (temp.GetLength() == next.GetLength()) IsAtom = true;
			else if (temp.GetLength() == previous.GetLength()) 
			{
				IsAtom = true;
				AtomComplete = true;
				IsLastAtom = true;
				BoundBegin = i + 1;
			}
		}
		if (IsAtom && (AtomComplete == false || IsLastAtom == true)) AtomStrAr.Add(temp);
	}
	atom.resize(AtomStrAr.GetSize());
	for (i = 0; i < AtomStrAr.GetSize(); i++)
	{
		double x = GetCoordinate(AtomStrAr.GetAt(i), 0);
		double y = GetCoordinate(AtomStrAr.GetAt(i), 10);
		double z = GetCoordinate(AtomStrAr.GetAt(i), 20);
		int weight;
		switch(AtomStrAr.GetAt(i).GetAt(31))
		{
		case 'O': {
			weight = 8;
			atom[i].VDWr = 1.70;
			break;
				  }
		case 'C': {
			weight = 12;
			atom[i].VDWr = 1.95;
			break;
				  }
		case 'H': {
			weight = 1;
			atom[i].VDWr = 1.20;
			break;
				  }
		case 'N': {
			weight = 14;
			atom[i].VDWr = 1.83;
			break;
				  }
		default: break;
		}
		atom[i].SetAtom(x, y, z, weight);
		atom[i].atomNum = i;
	}
	ConvertAtom();
}

/*
	�ú�������ԭ������
*/
double CMol_SimuDoc::GetCoordinate(CString s, int index)
{
	double r = 0;
	int flag = 0;
	for (int i = index; i <= index + 9; i++)
	{
		switch(s.GetAt(i))
		{
		case '1': {
			r = r * 10 + 1;
			break;
				  }
		case '2': {
			r = r * 10 + 2;
			break;
				  }
		case '3': {
			r = r * 10 + 3;
			break;
				  }
		case '4': {
			r = r * 10 + 4;
			break;
				  }
		case '5': {
			r = r * 10 + 5;
			break;
				  }
		case '6': {
			r = r * 10 + 6;
			break;
				  }
		case '7': {
			r = r * 10 + 7;
			break;
				  }
		case '8': {
			r = r * 10 + 8;
			break;
				  }
		case '9': {
			r = r * 10 + 9;
			break;
				  }
		case '0': {
			r = r * 10;
			break;
				  }
		case '-': {
			flag = 1;
			break;
				  }
		default: break;
		}
	}
	r = r / 10000;
	if (flag == 1) r = -r;
	return r;
}

/*
	�ú����õ���ѧ������Ϣ
*/
void CMol_SimuDoc::GetBound()
{
	BoundStrAr.RemoveAll();
	bool IsBound = false;
	bool BoundComplete = false; //�ж��Ƿ��Ѿ����껯ѧ������Ϣ
	bool IsLastBound = false; //�ж��Ƿ�Ϊ���һ����ѧ��
	for (int i = BoundBegin; i < fileContent.GetSize() - 1; i++)//ȡ���뻯ѧ����ص���Ϣ
	{
		CString lineStr = "";
		CString temp = fileContent.GetAt(i);
		CString next = fileContent.GetAt(i + 1);
		CString previous = fileContent.GetAt(i - 1);
		IsBound = false;
		if (!IsLastBound) 
		{
			if (temp.GetLength() == next.GetLength()) IsBound = true;
			else if (temp.GetLength() == previous.GetLength()) 
			{
				IsBound = true;
				BoundComplete = true;
				IsLastBound = true;
			}
		}
		if (IsBound && (BoundComplete == false || IsLastBound == true))
			BoundStrAr.Add(temp);
	}
	bound.resize(BoundStrAr.GetSize());
	for (i = 0; i < bound.size(); i++)
	{
		int start = 0, end = 0, type = 0;
		CString b = BoundStrAr.GetAt(i);
		CString s = "";
		for (int j = 0; j < 3; j++)
		{
			if (b.GetAt(j) != ' ') s += b.GetAt(j);
			start = atoi(s);
		}
		s = "";
		for (j = 3; j < 6; j++)
		{
			if (b.GetAt(j) != ' ') s += b.GetAt(j);
			end = atoi(s);
		}
		s = "";
		for (j = 6; j < 9; j++)
		{
			if (b.GetAt(j) != ' ') s += b.GetAt(j);
			type = atoi(s);
		}
		bound[i].setBounds(start, end, type);
	}
}

/*
	�˺�����ʵ��ԭ������ת��Ϊ��ʾ����,����������drawAtom������
*/
void CMol_SimuDoc::ConvertAtom()
{
	xMax = -99999; xMin = 99999;
	yMax = -99999; yMin = 99999;
	zMax = -99999; zMin = 99999;
	drawAtom.resize(atom.size());
	vector<Atom>::iterator it = atom.begin();
	for (; it != atom.end(); it++)
	{
		if (it->GetX() > xMax) xMax = it->GetX();
		if (it->GetY() > yMax) yMax = it->GetY();
		if (it->GetZ() > zMax) zMax = it->GetZ();
		if (it->GetX() < xMin) xMin = it->GetX();
		if (it->GetY() < yMin) yMin = it->GetY();
		if (it->GetZ() < zMin) zMin = it->GetZ(); 
	}
	//����е�����
	xCenter = (xMax + xMin) / 2; 
	yCenter = (yMax + yMin) / 2; 
	zCenter = (zMax + zMin) / 2; 
	xMax = 0; 
	yMax = 0; 
	zMax = 0; 
	double x, y, z;
	//��ʼת��������µ�����߽�ֵ
	for (int i = 0; i < atom.size(); i++)
	{
		x = atom[i].GetX() - xCenter;
		y = atom[i].GetY() - yCenter;
		z = atom[i].GetZ() - zCenter;
		drawAtom[i].SetAtom(x, y, z, atom[i].GetWeight());
		drawAtom[i].VDWr = atom[i].VDWr;
		drawAtom[i].atomNum = atom[i].atomNum;//ԭ�����б�Ų���
	}
}

void CMol_SimuDoc::GetMax()
{
	double x, y;
	for (int i = 0; i < drawAtom.size(); i++)
	{
		x = drawAtom[i].GetX();
		y = drawAtom[i].GetY();
		if (fabs(x) > xMax) xMax = fabs(x);
		if (fabs(y) > yMax) yMax = fabs(y);
	}
	totalMax = 0;
	if (xMax >= totalMax) totalMax = xMax;
	if (yMax >= totalMax) totalMax = yMax;
}

extern float eye[3];
extern float rot[3];

//------------------------------------------------------------------------
// If rotation angle is greater of 360 or lesser than -360,
// resets it back to zero.
//------------------------------------------------------------------------
void CMol_SimuDoc::clamp(float *v)
{
    int i;

    for (i = 0; i < 3; i ++)
        if (v[i] > 360 || v[i] < -360)
            v[i] = 0;
}

void CMol_SimuDoc::rotate()
{
	m_ratio = 1;
// 	cosa = xPre * xNow + yPre * yNow + zPre * zNow;
// 	cosa = cosa / sqrt((double)xPre * xPre + 
// 					           yPre * yPre + 
// 							   zPre * zPre);
// 	cosa = cosa / sqrt((double)xNow * xNow +
// 							   yNow * yNow +
// 				               zNow * zNow);
// 	if(cosa > 1 - 0.00001) m_spin = 0.0;
// 	else if(cosa < -1 + 0.00001) m_spin = 90.0;
// 	else m_spin = acos(cosa);
// 	sina = sqrt(1 - cosa * cosa);
// 	double t = 1 - cosa;  
// 	m_axis[0] = yPre * zNow - zPre * yNow;
// 	m_axis[1] = zPre * xNow - xPre * zNow;
// 	m_axis[2] = xPre * yNow - yPre * xNow;
// 	//��ʼ��תÿ��ԭ��
// 	//��������m_axis����תm_spin��
// 	for (int i = 0; i < atom.size(); i++)
// 	{
// 		//ȡ���任ǰԭ������
// 		double originalx = drawAtom[i].GetX();
// 		double originaly = drawAtom[i].GetY();
// 		double originalz = drawAtom[i].GetZ();
// 		//��m_axis��λ��,��Ϊ��x, y, z��
// 		double r = sqrt(m_axis[0] * m_axis[0] + 
// 						m_axis[1] * m_axis[1] + 
// 						m_axis[2] * m_axis[2]);
// 		double x = m_axis[0] / r;
// 		double y = m_axis[1] / r;
// 		double z = m_axis[2] / r;
// 		double nx = (t * x * x + cosa) * originalx +
// 					(t * x * y + sina * z) * originaly +
// 					(t * x * z - sina * y) * originalz;
// 		double ny = (t * x * y - sina * z) * originalx +
// 					(t * y * y + cosa) * originaly +
// 					(t * y * z + sina * x) * originalz;
// 		double nz = (t * x * z + sina * y) * originalx +
// 					(t * y * z - sina * x) * originaly +
// 					(t * z * z + cosa) * originalz;
// 		drawAtom[i].SetAtom(nx, ny, nz, drawAtom[i].GetWeight());
// 	}
// 	if (isShowSurface)
// 	{	
// 		particle.resize(0);
// 		InitParticles();
// 	}

	/* rotates screen */
    rot[0] += (yPre - yNow);
    rot[1] -= (xPre - xNow);
    clamp(rot);
	UpdateAllViews(NULL);
}

void CMol_SimuDoc::translate()
{
 	GeoOp = 1;
// 	for (int i = 0; i < atom.size(); i++)
// 	{
// 		double x, y;
// 		x = drawAtom[i].GetX() + m_displace[0];
// 		y = drawAtom[i].GetY() + m_displace[1];
// 		drawAtom[i].SetAtom(x, y, drawAtom[i].GetZ(), atom[i].GetWeight());	
// 	}
// 	if (isShowSurface)
// 	{	
// 		particle.resize(0);
// 		InitParticles();
// 	}
	eye[0] += (xPre - xNow) * 0.01f;
    eye[1] -= (yNow - yPre) * 0.01f;
    clamp(rot);
	UpdateAllViews(NULL);
}

void CMol_SimuDoc::scale()
{
	GeoOp = 2;
	m_ratio = (1 - (yNow-yPre) / (double)400);
	totalMax *= m_ratio;
	UpdateAllViews(NULL);
	m_ratio = 1;
}

/*
	�����ǻ����棬��ɷ��ӷ��»�����
*/
void CMol_SimuDoc::ConstructSurface()
{
	//��ÿһ��ԭ�Ӷ�����������Ƭ��ɵı���
	int s = atom.size(), tSize = 0;
	vector<Triangle> t;
	POINT3D p;
	triangle.clear();
	int counter = 0; //��¼����Ƭ����Ŀ

	for (int i = 0; i < s; i++)
	{
		drawAtom[i].constructVDWSurface();
		t = drawAtom[i].triangle;
		vector<Triangle>::iterator it = t.begin();
		for (; it != t.end(); it++)
		{	
 			counter++;
			it->CalculateCR();
			triangle.push_back(*it);
 		}
	}

	//�����ڱ߽��ϵ�����Ƭ
	Triangle tri;
	for (i = 0; i < triangle.size(); i++)
	{
		tri = triangle[i];
		if (!tri.isBorder) continue;
		//����߽�����Ƭ
		for (int j = 0; j < tri.neighborAtom.size(); j++)
		{
			Atom a = drawAtom[tri.neighborAtom[j]];
			//�������ڽ�ԭ�ӵ�����Ƭ�б��ҳ��뵱ǰ����Ƭ�ཻ������Ƭ���õ�����
			for (int k = 0; k < a.triangle.size(); k++)
			{
				Triangle t = a.triangle[k];
				t.CalculateCR();
				//ͬ��ֻ�����Ǳ߽�����Ƭ�������Ƭ�ཻ
				if(!t.isBorder) continue;
				//�����ж��Ƿ����ǰ����Ƭ����ԭ�ӱ����ཻ�������ཻ������
				//flag��Ϊ��־����,0:���ཻ	��1:�ཻ
				int flag = 0;
				for (int kk = 0; kk < t.neighborAtom.size(); kk++)
					if (t.neighborAtom[kk] == tri.atomNum) {flag = 1; break;}
				if (flag == 0) continue;
				//�ж������������ľ��룬������ǰ뾶�ĺͣ��򲻿����ཻ(�����󽻴���)
				if (tri.center.distance(t.center) > tri.r + t.r) continue;
				//һ�з����������������
				Line l = tri.IntersectLine(t);
				if (l.sP.p.x < 9990)
					tri.intersectLines.push_back(l);
			}
		}
		//����õĽ���滻ԭ�ȵ�����Ƭ
		triangle[i] = tri;
	}
	CString ss;
	ss.Format("%d", counter);
	AfxMessageBox("There is " + ss + " triangles");
}

void CMol_SimuDoc::GetVDWNeighbors()
{
	for (int j=0;j<drawAtom.size();j++)
	{
		drawAtom[j].RemoveNeighbors();
	}
	for (int i = 0;i < drawAtom.size(); i++)//������ڵ�ԭ��
	{
		for(int j=i+1;j<drawAtom.size();j++)
		{
			double xi,xj,yi,yj,zi,zj;
			xi=drawAtom[i].GetX();
			xj=drawAtom[j].GetX();
			yi=drawAtom[i].GetY();
			yj=drawAtom[j].GetY();
			zi=drawAtom[i].GetZ();
			zj=drawAtom[j].GetZ();
			if (fabs(xi-xj)<=(drawAtom[i].VDWr+drawAtom[j].VDWr)&&fabs(yi-yj)<=(drawAtom[i].VDWr+drawAtom[j].VDWr)&&fabs(zi-zj)<=(drawAtom[i].VDWr+drawAtom[j].VDWr))
			{
				double d=(xi-xj)*(xi-xj)+(yi-yj)*(yi-yj)+(zi-zj)*(zi-zj);
				d=sqrt(d);
				if (d < drawAtom[i].VDWr+drawAtom[j].VDWr)
				{
					drawAtom[i].AddNeighbor(&drawAtom[j]);
					drawAtom[j].AddNeighbor(&drawAtom[i]);
				}
			}
		}
	}
}

void CMol_SimuDoc::GetNeighbors()
{
	for (int i = 0; i < drawAtom.size(); i++)
	{
		drawAtom[i].RemoveNeighbors();
	}
	for (i = 0; i < bound.size(); i++)
	{
		int s = bound[i].getStart() - 1;
		int e = bound[i].getEnd() - 1;
		drawAtom[s].AddNeighbor(&drawAtom[e]);
		drawAtom[e].AddNeighbor(&drawAtom[s]);
	}
}
