/************************************************************************/
/*  三维归一化平面类                                                     */
/************************************************************************/
#ifndef __Plane3D_H__
#define __Plane3D_H__

#include "Line.h"

class Plane3D
{
public:
	POINT3D n; //归一化法向量
	POINT3D p; //平面上一定点
	double c; //法线常数形式下的常数-----nX = c, c=nP
	double d; //平面隐式表示形式下的常数
	//构造函数
	Plane3D(){}
	//由点法式确定的构造函数
	Plane3D(POINT3D& n, POINT3D& p)
	{
		//首先完成法向量归一化
		n.nomalize();
		this->n = n;
		this->p = p;
		//计算常数ｃ和ｄ
		c = POINT3D::innerProduct(n, p);
		d = -c;
	}
	//由法线常数确定的构造函数
	Plane3D(POINT3D& n, double& c)
	{
		//首先完成法向量归一化
		n.nomalize();
		this->n = n;
		this->c = c;
		this->d = -c;
	}
	//计算点到该平面的有向(符号)距离
	double distance(POINT3D& point)
	{
		n.nomalize();
		//POINT3D pTemp(point.x - p.x, point.y - p.y, point.z - p.z);
		return POINT3D::innerProduct(n, point) + d;
	}
	//求两平面交线
	Line IntersectWithPlane(Plane3D& plane)
	{
		Line l;
		POINT3D normal = POINT3D::crossProduct(n, plane.n);
		l.direction = normal;
		double s1, s2, a, b;
		s1 = c; s2 = plane.c;
		double n1n2dot = POINT3D::innerProduct(n, plane.n);
		double n1normsqr = POINT3D::innerProduct(n, n);
		double n2normsqr = POINT3D::innerProduct(plane.n, plane.n);
		a = (s2 * n1n2dot - s1 * n2normsqr) / (n1n2dot * n1n2dot - n1normsqr * n2normsqr);
		b = (s1 * n1n2dot - s2 * n2normsqr) / (n1n2dot * n1n2dot - n1normsqr * n2normsqr);
		l.sP.SetCoord(a * n.x + b * plane.n.x, a * n.y + b * plane.n.y, a * n.z + b * plane.n.z);
		return l;
	}
};

#endif