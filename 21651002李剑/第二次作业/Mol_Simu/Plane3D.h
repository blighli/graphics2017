/************************************************************************/
/*  ��ά��һ��ƽ����                                                     */
/************************************************************************/
#ifndef __Plane3D_H__
#define __Plane3D_H__

#include "Line.h"

class Plane3D
{
public:
	POINT3D n; //��һ��������
	POINT3D p; //ƽ����һ����
	double c; //���߳�����ʽ�µĳ���-----nX = c, c=nP
	double d; //ƽ����ʽ��ʾ��ʽ�µĳ���
	//���캯��
	Plane3D(){}
	//�ɵ㷨ʽȷ���Ĺ��캯��
	Plane3D(POINT3D& n, POINT3D& p)
	{
		//������ɷ�������һ��
		n.nomalize();
		this->n = n;
		this->p = p;
		//���㳣����ͣ�
		c = POINT3D::innerProduct(n, p);
		d = -c;
	}
	//�ɷ��߳���ȷ���Ĺ��캯��
	Plane3D(POINT3D& n, double& c)
	{
		//������ɷ�������һ��
		n.nomalize();
		this->n = n;
		this->c = c;
		this->d = -c;
	}
	//����㵽��ƽ�������(����)����
	double distance(POINT3D& point)
	{
		n.nomalize();
		//POINT3D pTemp(point.x - p.x, point.y - p.y, point.z - p.z);
		return POINT3D::innerProduct(n, point) + d;
	}
	//����ƽ�潻��
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