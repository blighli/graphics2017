/*************************************************************************/
/*						ֱ�߶���                */
/************************************************************************/
#ifndef __LINE_H__
#define __LINE_H__

#include "Vertex.h"

class Line
{
public:
	//����
	Vertex sP, eP; //�����յ�
	POINT3D direction; //ֱ�߶εķ�������
	//���캯��
	Line(){}
	Line(Vertex sP, Vertex eP) 
	{
		this->sP.SetCoord(sP.p); 
		this->eP.SetCoord(eP.p);
		//����������Ҫ��һ���õ���������L = sP + t * direction, t[0, 1]
		calculateDirection();
	}
	void calculateDirection()
	{
		//����������Ҫ��һ���õ���������L = sP + t * direction, t[0, 1]
		double dx = eP.p.x - sP.p.x;
		double dy = eP.p.y - sP.p.y;
		double dz = eP.p.z - sP.p.z;
		double inversedd = 1 / sqrt(dx * dx + dy * dy + dz * dz);
		this->direction.SetXYZ(dx * inversedd, dy * inversedd, dz * inversedd);
	}
	//ֱ����ֱ�ߵĽ���
	POINT3D IntersectWithLine(Line& l)
	{
		POINT3D p;
		double x1 = sP.p.x; double y1 = sP.p.y; double z1 = sP.p.z;
		double x2 = l.sP.p.x; double y2 = l.sP.p.y; double z2 = l.sP.p.z;
		double dx1 = direction.x; double dy1 = direction.y; double dz1 = direction.z;
		double dx2 = l.direction.x; double dy2 = l.direction.y; double dz2 = l.direction.z;
		double t2;
		if (dx2 * dy1 - dy2 * dx1 != 0) 
			t2 = ((y2 - y1) * dx1 - (x2 - x1) * dy1) / (dx2 * dy1 - dy2 * dx1);
		else if (dx2 * dz1 - dz2 * dx1 != 0)
			t2 = ((z2 - z1) * dx1 - (x2 - x1) * dz1) / (dx2 * dz1 - dz2 * dx1);
		else
			t2 = ((z2 - z1) * dy1 - (y2 - y1) * dz1) / (dy2 * dz1 - dz2 * dy1);
		p.SetXYZ(l.sP.p.x + t2 * dx2, l.sP.p.y + t2 * dy2, l.sP.p.z + t2 * l.direction.z);
		return p;
	}
};

#endif