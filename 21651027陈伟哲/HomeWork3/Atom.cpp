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
	vertex.resize(13); //把０号空缺
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
		//与顶角相连的
		t.SetT(vertex[1], vertex[i], vertex[i + 1]);
		triangle.push_back(t);
		//与下一排点相连的
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
		//与顶角相连的
		t.SetT(vertex[12], vertex[i + 1], vertex[i]);
		triangle.push_back(t);
		//与上排相连的
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
/* 求两点在球面上的交点                                                  */
/************************************************************************/
POINT3D Atom::mid(POINT3D p1, POINT3D p2)
{ 
	double x1, y1, z1, r1;
	POINT3D p;
	p1.SetXYZ(p1.x - x, p1.y - y, p1.z - z);
	p2.SetXYZ(p2.x - x, p2.y - y, p2.z - z);
	x1 = p1.x + p2.x; y1 = p1.y + p2.y; z1 = p1.z + p2.z;  
	//单位化
	r1 = sqrt(x1 * x1 + y1 * y1 + z1 * z1);
	x1 = x1 / r1 * VDWr; y1 = y1 / r1 * VDWr; z1 = z1 / r1 * VDWr; 
	p.SetXYZ(x1 + x, y1 + y, z1 + z);
	return p;
}

void Atom::divide(Triangle t)
{
	//求三条边各自在球面上的中点
	Vertex a, b, c;
	Vertex ab, ac, bc;
	Triangle tri1, tri2, tri3, tri4;
	a = t.GetA(); b = t.GetB(); c = t.GetC(); 
	//先分别求三个顶点向量两两之间的和，作为中点向量的方向
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
	for (int i = 0; i < size; i++) //将目前每一个三角形都细分为４个
	{
		Triangle tri = triangle[i];
		divide(tri);
	}
	//调整现在的三角形数目，删掉在内部的三角形
	for (i = size; i < triangle.size(); i++) triangle[i - size] = triangle[i];
	triangle.resize(triangle.size() - size);
}

//范德华球形表面
void Atom::constructVDWSurface()
{
	int i;
	initVertex();
	initTriangles();
	//将三角形顶点平移至原子周边指定位置
	translation();
	for (i = 0; i < 2; i++)
	{
		//细分
		tessellation();
		//删除隐藏在内部的三角片，减少未来细分负担
		//通过比较该三角片与自己所属原子以及邻近原子的距离来判定
		DeleteHiddenTri();
	}
	//计算法向量以及所在平面方程为求交及绘制使用
	for (i = 0; i < triangle.size(); i++) 
	{
		triangle[i].SetNormal();	
		triangle[i].CalculatePlane();
	}
	//求每个顶点的法向
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
/* 删除隐藏在表面内部的三角形                                             */
/************************************************************************/
void Atom::DeleteHiddenTri()
{
	POINT3D p;
	int counter = 0; //记录三角片总数目
	bool flag = false; //如果flag为true，则表示不添加到表面三角形中
	vector<Atom*>::iterator neighborit;
	vector<Triangle>::iterator it = triangle.begin();
	vector <Triangle> triArray;
	triArray.clear();
	for (; it != triangle.end(); it++)
	{	
		neighborit = neighbors.begin();
		//去掉在形体内部的三角形
		//如果该三角形顶点距离其它原子更近则删掉它
		POINT3D a = it->GetA().p;
		POINT3D b = it->GetB().p;
		POINT3D c = it->GetC().p;
		flag = false;
		for (; neighborit != neighbors.end(); neighborit++)
		{	
			double Nr = (*neighborit)->VDWr;
			p.SetXYZ((*neighborit)->GetX(), (*neighborit)->GetY(), (*neighborit)->GetZ());
			//全在内部，删
			if (p.distance(a) < Nr && p.distance(b) < Nr && p.distance(c) < Nr) 
			{
				flag = true;
				break;
			}
			//部分在内部，标记为边界三角片
			if (p.distance(a) < Nr || p.distance(b) < Nr || p.distance(c) < Nr)
			{
				it->isBorder = true;
				//将与该三角片相邻的原子编号加入该三角片的邻近原子列表
				it->neighborAtom.push_back((*neighborit)->atomNum);
			}
		}
		if (!flag) //如果ｆｌａｇ为false，则表示是合法表面三角形
			triArray.push_back(*it);
	}
	triangle = triArray;
}

/************************************************************************/
/* 计算每个顶点的法向                                                    */
/************************************************************************/
void Atom::CalculateVertexNormal()
{
	Triangle t;
	Vertex v;
	double x1, y1, z1, r1;
	for (int i = 0; i < triangle.size(); i++)
	{
		//对每个三角形的顶点求取顶点法向
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
