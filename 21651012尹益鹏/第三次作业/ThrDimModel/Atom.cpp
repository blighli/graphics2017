#include "stdafx.h"
#include "Atom.h"
#include <math.h>

#define PI 3.1415926535898

Atom::Atom()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->atomWeight = 0;
	this->r = 0;
	this->thita = 0;
	this->fai = 0;
}

Atom::Atom(double x, double y, double z, int weight)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->atomWeight = weight;
	r = sqrt(x * x + y * y + z * z);
	double ang1 = atan(sqrt(x * x + y * y) / z); 
	if (z == 0) fai =  PI / 2;
	else if (z > 0) fai = ang1;
	else fai = ang1 + PI;
	double ang2 = atan(y / x);
	if (x == 0)
	{
		if (y > 0) thita = PI / 2;
		else if (y < 0) thita = PI * 3 / 2;
	}
	else if (x > 0)
	{
		if (y > 0) thita = ang2;
		else thita = ang2 + PI * 2;
	}
	else if (x < 0)
	{
		if (y > 0) thita = ang2 + PI;
		if (y < 0) thita = ang2 + PI;
	}
}
  
void Atom::SetAtom(double x, double y, double z, int weight)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->atomWeight = weight;
	r = sqrt(x * x + y * y + z * z);
	double ang1 = atan(sqrt(x * x + y * y) / z); 
	if (z == 0) fai =  PI / 2;
	else if (z > 0) fai = ang1;
	else this->fai = ang1 + PI;
	double ang2 = atan(y / x);
	if (x == 0)
	{
		if (y > 0) thita = PI / 2;
		else if (y < 0) thita = PI * 3 / 2;
	}
	else if (x > 0)
	{
		if (y > 0) thita = ang2;
		else thita = ang2 + PI * 2;
	}
	else if (x < 0)
	{
		if (y > 0) thita = ang2 + PI;
		if (y < 0) thita = ang2 + PI;
	}
}

void Atom::AddNeighbor(Atom* a)
{
	this->neighbors.push_back(a);
}

vector <Atom*> Atom::GetNeighbors()
{
	return this->neighbors;
}

double Atom::GetX()
{
	return this->x;
}

double Atom::GetY()
{
	return this->y;
}

double Atom::GetZ()
{
	return this->z;
}

double Atom::GetR()
{
	return this->r;
}

double Atom::GetF()
{
	return this->fai;
}

double Atom::GetTH()
{
	return this->thita;
}

int Atom::GetWeight()
{
	return this->atomWeight;
}

void Atom::initVertex()
{
	vertex.resize(13); //�ѣ��ſ�ȱ
	double theta, fai;
	vertex[1].p.SetPolarCoord(this->VDWr, 0, 0);
	vertex[12].p.SetPolarCoord(this->VDWr, 0, PI);
	for (int i = 2; i <= 6; i++)
	{
		theta = 2 * PI / 5 * (i - 2);
		fai = 63.435 / 180 * PI;
		vertex[i].p.SetPolarCoord(this->VDWr, theta, fai);
	}
	for (i = 7; i <= 11; i++)
	{
		theta = 2 * PI / 5 * (i - 7) + PI / 5;
		fai = 116.565 / 180 * PI;
		vertex[i].p.SetPolarCoord(this->VDWr, theta, fai);
	}
}

void Atom::initTriangles()
{
	Triangle t;
	for (int i = 2; i <= 5; i++)
	{
		//�붥��������
		t.SetT(vertex[1], vertex[i], vertex[i + 1]);
		triangle.push_back(t);
		//����һ�ŵ�������
		t.SetT(vertex[i], vertex[i + 5], vertex[i + 1]);
		triangle.push_back(t);
	}
	//2,6,1
	t.SetT(vertex[1], vertex[6], vertex[2]);
	triangle.push_back(t);
	//2,6,11
	t.SetT(vertex[6], vertex[11], vertex[2]);
	triangle.push_back(t);
	for (i = 7; i <= 10; i++)
	{
		//�붥��������
		t.SetT(vertex[12], vertex[i + 1], vertex[i]);
		triangle.push_back(t);
		//������������
		t.SetT(vertex[i - 4], vertex[i], vertex[i + 1]);
		triangle.push_back(t);
	}
	//12,11,7
	t.SetT(vertex[12], vertex[7], vertex[11]);
	triangle.push_back(t);
	//2,11,7
	t.SetT(vertex[2], vertex[11], vertex[7]);
	triangle.push_back(t);
}

/************************************************************************/
/* �������������ϵĽ���                                                  */
/************************************************************************/
POINT3D Atom::mid(POINT3D p1, POINT3D p2)
{ 
	double x1, y1, z1, r1;
	POINT3D p;
	p1.SetXYZ(p1.x - x, p1.y - y, p1.z - z);
	p2.SetXYZ(p2.x - x, p2.y - y, p2.z - z);
	x1 = p1.x + p2.x; y1 = p1.y + p2.y; z1 = p1.z + p2.z;  
	//��λ��
	r1 = sqrt(x1 * x1 + y1 * y1 + z1 * z1);
	x1 = x1 / r1 * VDWr; y1 = y1 / r1 * VDWr; z1 = z1 / r1 * VDWr; 
	p.SetXYZ(x1 + x, y1 + y, z1 + z);
	return p;
}

void Atom::divide(Triangle t)
{
	//�������߸����������ϵ��е�
	Vertex a, b, c;
	Vertex ab, ac, bc;
	Triangle tri1, tri2, tri3, tri4;
	a = t.GetA(); b = t.GetB(); c = t.GetC(); 
	//�ȷֱ�������������������֮��ĺͣ���Ϊ�е������ķ���
	ab.SetCoord(mid(a.p, b.p)); ac.SetCoord(mid(a.p, c.p)); bc.SetCoord(mid(b.p, c.p)); 
	tri1.SetA(a); tri1.SetB(ab); tri1.SetC(ac); 
	tri2.SetA(ab); tri2.SetB(b); tri2.SetC(bc);
	tri3.SetA(ac); tri3.SetB(bc); tri3.SetC(c);
	tri4.SetA(ab); tri4.SetB(bc); tri4.SetC(ac);
	tri1.drawAtomWeight = tri2.drawAtomWeight = 
	tri3.drawAtomWeight = tri4.drawAtomWeight = this->atomWeight;
	tri1.atomNum = tri2.atomNum = tri3.atomNum = tri4.atomNum = atomNum;
	triangle.push_back(tri4);
	triangle.push_back(tri3);
	triangle.push_back(tri2);
	triangle.push_back(tri1);
}

void Atom::tessellation()
{
	int size = triangle.size();
	for (int i = 0; i < size; i++) //��Ŀǰÿһ�������ζ�ϸ��Ϊ����
	{
		Triangle tri = triangle[i];
		divide(tri);
	}
	//�������ڵ���������Ŀ��ɾ�����ڲ���������
	for (i = size; i < triangle.size(); i++) triangle[i - size] = triangle[i];
	triangle.resize(triangle.size() - size);
}

//���»����α���
void Atom::constructVDWSurface()
{
	int i;
	initVertex();
	initTriangles();
	//�������ζ���ƽ����ԭ���ܱ�ָ��λ��
	translation();
	for (i = 0; i < 2; i++)
	{
		//ϸ��
		tessellation();
		//ɾ���������ڲ�������Ƭ������δ��ϸ�ָ���
		//ͨ���Ƚϸ�����Ƭ���Լ�����ԭ���Լ��ڽ�ԭ�ӵľ������ж�
		DeleteHiddenTri();
	}
	//���㷨�����Լ�����ƽ�淽��Ϊ�󽻼�����ʹ��
	for (i = 0; i < triangle.size(); i++) 
	{
		triangle[i].SetNormal();	
		triangle[i].CalculatePlane();
	}
	//��ÿ������ķ���
	CalculateVertexNormal();
}

void Atom::translation()
{
	int s = triangle.size();
	for (int i = 0; i < s; i++)
	{
		Vertex A = triangle[i].GetA();
		A.SetCoord(A.p.x + x, A.p.y + y, A.p.z + z);
		Vertex B = triangle[i].GetB();
		B.SetCoord(B.p.x + x, B.p.y + y, B.p.z + z);
		Vertex C = triangle[i].GetC();
		C.SetCoord(C.p.x + x, C.p.y + y, C.p.z + z);
		triangle[i].SetT(A, B, C);
	}
}

void Atom::RemoveNeighbors()
{
	this->neighbors.clear();
}

/************************************************************************/
/* ɾ�������ڱ����ڲ���������                                             */
/************************************************************************/
void Atom::DeleteHiddenTri()
{
	POINT3D p;
	int counter = 0; //��¼����Ƭ����Ŀ
	bool flag = false; //���flagΪtrue�����ʾ����ӵ�������������
	vector<Atom*>::iterator neighborit;
	vector<Triangle>::iterator it = triangle.begin();
	vector <Triangle> triArray;
	triArray.clear();
	for (; it != triangle.end(); it++)
	{	
		neighborit = neighbors.begin();
		//ȥ���������ڲ���������
		//����������ζ����������ԭ�Ӹ�����ɾ����
		POINT3D a = it->GetA().p;
		POINT3D b = it->GetB().p;
		POINT3D c = it->GetC().p;
		flag = false;
		for (; neighborit != neighbors.end(); neighborit++)
		{	
			double Nr = (*neighborit)->VDWr;
			p.SetXYZ((*neighborit)->GetX(), (*neighborit)->GetY(), (*neighborit)->GetZ());
			//ȫ���ڲ���ɾ
			if (p.distance(a) < Nr && p.distance(b) < Nr && p.distance(c) < Nr) 
			{
				flag = true;
				break;
			}
			//�������ڲ������Ϊ�߽�����Ƭ
			if (p.distance(a) < Nr || p.distance(b) < Nr || p.distance(c) < Nr)
			{
				it->isBorder = true;
				//���������Ƭ���ڵ�ԭ�ӱ�ż��������Ƭ���ڽ�ԭ���б�
				it->neighborAtom.push_back((*neighborit)->atomNum);
			}
		}
		if (!flag) //��������Ϊfalse�����ʾ�ǺϷ�����������
			triArray.push_back(*it);
	}
	triangle = triArray;
}

/************************************************************************/
/* ����ÿ������ķ���                                                    */
/************************************************************************/
void Atom::CalculateVertexNormal()
{
	Triangle t;
	Vertex v;
	double x1, y1, z1, r1;
	for (int i = 0; i < triangle.size(); i++)
	{
		//��ÿ�������εĶ�����ȡ���㷨��
		t = triangle[i];
		x1 = t.GetA().p.x - x;
		y1 = t.GetA().p.y - y;
		z1 = t.GetA().p.z - z;
		r1 = sqrt(x1 * x1 + y1 * y1 + z1 * z1);
		v.SetCoord(t.GetA().p);
		v.SetNormal(x1 / r1, y1 / r1, z1 / r1);
		t.SetA(v);
		x1 = t.GetB().p.x - x;
		y1 = t.GetB().p.y - y;
		z1 = t.GetB().p.z - z;
		r1 = sqrt(x1 * x1 + y1 * y1 + z1 * z1);
		v.SetCoord(t.GetB().p);
		v.SetNormal(x1 / r1, y1 / r1, z1 / r1);
		t.SetB(v);
		x1 = t.GetC().p.x - x;
		y1 = t.GetC().p.y - y;
		z1 = t.GetC().p.z - z;
		r1 = sqrt(x1 * x1 + y1 * y1 + z1 * z1);
		v.SetCoord(t.GetC().p);
		v.SetNormal(x1 / r1, y1 / r1, z1 / r1);
		t.SetC(v);
		triangle[i] = t;
	}
}
