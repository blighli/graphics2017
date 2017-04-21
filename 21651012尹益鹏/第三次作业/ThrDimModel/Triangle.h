#ifndef __Triangle_H__
#define __Triangle_H__

#include "Line.h"
#include "Plane3D.h"

class Triangle
{
	private:
		Vertex a, b, c;
	public:
		Line IMPOSSBLELINE;
		bool isBorder;//�Ƿ��Ǳ߽�����Ƭ
		vector<int> neighborAtom; // �ڽ�ԭ���б����߽�����Ƭ������
		vector<Line> intersectLines; //�����б����߽�����Ƭ���������ǻ�����
		vector<POINT3D> interP; //����Ƭ�ڽ����б����߽�����Ƭ���������ǻ�����

		POINT3D center; //����������
		double r; //���°뾶

		Line ab, bc, ca; //����Ƭ������
		Plane3D plane; //����Ƭ���ڵ�ƽ�����
		int atomNum;//������Ƭ����ԭ�ӱ��
		POINT3D normal;
		int drawAtomWeight;
		//���캯��
		Triangle(){}
		Triangle(POINT3D p1, POINT3D p2, POINT3D p3)
		{
			a.p = p1; b.p = p2; c.p = p3;
			Line l1(a, b), l2(b, c), l3(c, a);
			ab = l1; bc = l2; ca = l3;
			CalculatePlane();
			neighborAtom.clear();
			intersectLines.clear();
			Line IL(Vertex(POINT3D(9999, 9999, 9999), POINT3D(9999, 9999, 9999)),
					Vertex(POINT3D(9999, 9999, 9999), POINT3D(9999, 9999, 9999)));
			IMPOSSBLELINE = IL;
			CalculateCR();
		}
		inline void SetNormal()
		{
			//��űߵ�����
			POINT3D edge1(b.p.x - a.p.x, b.p.y - a.p.y, b.p.z - a.p.z);
			POINT3D edge2(c.p.x - a.p.x, c.p.y - a.p.y, c.p.z - a.p.z);
			//�����������Ĳ�����������η�����
			normal = POINT3D::crossProduct(edge1, edge2);
 			//��һ��
 			normal.nomalize();
		}
		inline Vertex GetA(){return a;}
		inline Vertex GetB(){return b;}
		inline Vertex GetC(){return c;}
		inline void SetA(Vertex p){a = p;}
		inline void SetB(Vertex p){b = p;}
		inline void SetC(Vertex p){c = p;}
		inline void SetT(Vertex p1, Vertex p2, Vertex p3)
		{
			a = p1; b = p2; c = p3;
		}
		inline double Area2(POINT3D a, POINT3D b, POINT3D c)
		{
			return (b.x - a.x) * (c.y - a.y)
				 - (c.x - a.x) * (b.y - a.y);
		}
		inline bool isStandard(){return Area2(a.p, b.p, c.p) < 0;}//�жϵ�C�ǲ�����AB�����
		inline void Standardize()
		{
			if (!isStandard()) 
			{
				Vertex v = b;
				this->SetB(c);
				this->SetC(v);
			}
		}
		inline bool isCross(POINT3D p1, POINT3D p2, POINT3D p3, POINT3D p4)
		{
			if (Area2(p1, p2, p3) * Area2(p1, p2, p4) < 0 && 
				Area2(p3, p4, p1) * Area2(p3, p4, p2) < 0) return true;
			else return false;
		}
		//�жϴ��������Ƿ������ֱ���ཻ
		inline bool isIntersect(POINT3D p1, POINT3D p2)
		{
			if (isCross(a.p, b.p, p1, p2) ||
				isCross(a.p, c.p, p1, p2) ||
				isCross(b.p, c.p, p1, p2))
			return true;
			else return false;
		}
		inline bool isOverlap(Triangle& t)
		{
			if (isIntersect(t.a.p, t.b.p) ||
				isIntersect(t.a.p, t.c.p) ||
				isIntersect(t.b.p, t.c.p))
			return true;
			else return false;
		}
		inline bool havePoint(POINT3D p)
		{
			if (p.x == a.p.x && p.y == a.p.y && p.z == a.p.z || 
				p.x == b.p.x && p.y == b.p.y && p.z == b.p.z || 
				p.x == c.p.x && p.y == b.p.y && p.z == b.p.z) 
				return true;
			else return false;
		}
		/************************************************************************/
		/* �õ�������Ƭ���ڵ�ƽ��                                                 */
		/************************************************************************/
		void CalculatePlane()
		{
			Plane3D p(normal, a.p);
			plane = p;
		}
		/************************************************************************/
		/* �������ε������Լ����Բ�뾶                                           */
		/************************************************************************/
		void CalculateCR()
		{
			double x, y, z;
			x = 0.333 * (a.p.x + b.p.x + c.p.x);
			y = 0.333 * (a.p.y + b.p.y + c.p.y);
			z = 0.333 * (a.p.z + b.p.z + c.p.z);
			center.SetXYZ(x, y, z);
			r = center.distance(a.p);
		}
		/************************************************************************/
		/* �������Ƭ��ָ������Ƭ�Ľ��߶�                                         */
		/************************************************************************/
		Line IntersectLine(Triangle& tri)
		{
			Line interL;
			POINT3D v[2][3]; 
			//�ȵõ���ƽ�潻��
			Line l = plane.IntersectWithPlane(tri.plane);
			//�����������Ƭ���������ڵ�ǰ����Ƭλ��ͬһ�����������
			//�������������㵽��ǰ����Ƭ����ƽ����������
			double d[2][3];
			POINT3D p0 = tri.GetA().p;
			POINT3D p1 = tri.GetB().p;
			POINT3D p2 = tri.GetC().p;
			d[0][0] = plane.distance(p0); d[0][1] = plane.distance(p1); d[0][2] = plane.distance(p2);
			//���ȫ��ͬ�࣬���޽��ߣ����ز�������
			if (d[0][0] < 0 && d[0][1] < 0 && d[0][2] < 0 || d[0][0] > 0 && d[0][1] > 0 && d[0][2] > 0)
				return IMPOSSBLELINE;
			//�жϷ���,���з�����ͬ��������ͬ��(�����ж���ͬ������)
			if (d[0][0] * d[0][1] > 0) {v[0][0] = p0; v[0][1] = p1; v[0][2] = p2;}
			else if (d[0][0] * d[0][2] > 0) {v[0][0] = p0; v[0][1] = p2; v[0][2] = p1;}
			else if (d[0][1] * d[0][2] > 0) {v[0][0] = p1; v[0][1] = p2; v[0][2] = p0;}
			//���ж��Ƿ���һ�������ڶԷ�������ƽ���ϵ����
			if (d[0][0] == 0) {v[0][0] = p0; v[0][1] = p1; v[0][2] = p2;}
			else if (d[0][1] == 0) {v[0][0] = p1; v[0][1] = p0; v[0][2] = p2;}
			else if (d[0][2] == 0) {v[0][0] = p2; v[0][1] = p0; v[0][2] = p1;}
			//��ǰ����Ƭ��������ڣ��������ƽ����������
			tri.CalculatePlane();
			d[1][0] = tri.plane.distance(a.p); d[1][1] = tri.plane.distance(b.p); d[1][2] = tri.plane.distance(c.p);
			//���ȫ��ͬ�࣬���޽��ߣ����ز�������
			if (d[1][0] < 0 && d[1][1] < 0 && d[1][2] < 0 || d[1][0] > 0 && d[1][1] > 0 && d[1][2] > 0)
				return IMPOSSBLELINE;
			//�жϷ���,���з�����ͬ��������ͬ��(�����ж���ͬ������)
			if (d[1][0] * d[1][1] > 0) {v[1][0] = a.p; v[1][1] = b.p; v[1][2] = c.p;}
			else if (d[1][0] * d[1][2] > 0) {v[1][0] = a.p; v[1][1] = c.p; v[1][2] = b.p;}
			else if (d[1][1] * d[1][2] > 0) {v[1][0] = b.p; v[1][1] = c.p; v[1][2] = a.p;}
			//���ж��Ƿ���һ�������ڶԷ�������ƽ���ϵ����
			if (d[1][0] == 0) {v[1][0] = a.p; v[1][1] = b.p; v[1][2] = c.p;}
			else if (d[1][1] == 0) {v[1][0] = b.p; v[1][1] = a.p; v[1][2] = c.p;}
			else if (d[1][2] == 0) {v[1][0] = c.p; v[1][1] = a.p; v[1][2] = b.p;}
			//�����������ε���������ֱ�������ͶӰ
			double v1[2][3], sortT[4];
			//double t[2][2];
			int count = 0;
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					POINT3D pTemp(v[i][j].x - l.sP.p.x, v[i][j].y - l.sP.p.y, v[i][j].z - l.sP.p.z);
					v1[i][j] = POINT3D::innerProduct(l.direction, pTemp);
				}
				//ͶӰȷ���ѱ���ȷ���!!!
// 				for (j = 0; j < 2; j++)
// 				{
// 					//��t00��t01,t10,t11
// 					//t[i][j] = v1[i][j] + (v1[i][2] - v1[i][j]) * d[i][j] / (d[i][j] - d[i][2]);
// 					t[i][j] = (fabs(d[i][j]) * v1[i][2] + fabs(d[i][2]) * v1[i][j]) / 
// 						(fabs(d[i][j]) + fabs(d[i][2]));
// 					sortT[count] = t[i][j];
// 					count++;
// 				}
			}
// 			//�ԣ��������С������������
// 			for (i = 0; i < 3; i++)
// 				for (int j = i + 1; j < 4; j++)
// 					if (sortT[j] < sortT[i])
// 					{
// 						double temp = sortT[i];
// 						sortT[i] = sortT[j];
// 						sortT[j] = temp;
// 					}
// 			//ȡ�м�����Ϊ��������
// 			double x = l.sP.p.x + t[0][0] * l.direction.x;
// 			double y = l.sP.p.y + t[0][0] * l.direction.y;
// 			double z = l.sP.p.z + t[0][0] * l.direction.z;
// 			interL.sP.SetCoord(x, y, z);
// 			x = l.sP.p.x + t[0][1] * l.direction.x;
// 			y = l.sP.p.y + t[0][1] * l.direction.y;
// 			z = l.sP.p.z + t[0][1] * l.direction.z;
// 			interL.eP.SetCoord(x, y, z);
 			interL.direction = l.direction;
			Line lTemp;
			double x, y, z;
			//�ĸ�����
			POINT3D pInter[4];
			count = 0;
			double maxT[2], minT[2]; //�����߶θ����������ֵ
			maxT[0] = maxT[1] = -9999;
			minT[0] = minT[1] = 9999;
			for (i = 0; i < 2; i++)
			{
				//��ָʾ�߶α��
				for (int j = 0; j < 2; j++)
				{
					lTemp.sP.p = v[i][j];
					lTemp.eP.p = v[i][2];
					lTemp.calculateDirection();
					pInter[count] = lTemp.IntersectWithLine(l);
					//���㽻���ڽ����ϵģ�����ֵ
					if (l.direction.x != 0) 
						sortT[count] = (pInter[count].x - l.sP.p.x) / l.direction.x;
					else if (l.direction.y != 0)
						sortT[count] = (pInter[count].y - l.sP.p.y) / l.direction.y;
						else
							sortT[count] = (pInter[count].z - l.sP.p.z) / l.direction.z;
					if (maxT[i] < sortT[count]) maxT[i] = sortT[count];
					if (minT[i] > sortT[count]) minT[i] = sortT[count];
					count++;
				}
			}
			//���û�������߶�û�н�������֤����������Ƭ���ཻ�����ز�����ֱ��
			if (minT[0] > maxT[1] || maxT[0] < minT[1])
			{
				return IMPOSSBLELINE;
			}
			//�ԣ��������С������������
			for (i = 0; i < 3; i++)
				for (int j = i + 1; j < 4; j++)
					if (sortT[j] < sortT[i])
					{
						double temp = sortT[i];
						sortT[i] = sortT[j];
						sortT[j] = temp;
					}
			//ȡ�м�����Ϊ��������
			x = l.sP.p.x + sortT[1] * l.direction.x;
			y = l.sP.p.y + sortT[1] * l.direction.y;
			z = l.sP.p.z + sortT[1] * l.direction.z;
			interL.sP.SetCoord(x, y, z);
			x = l.sP.p.x + sortT[2] * l.direction.x;
			y = l.sP.p.y + sortT[2] * l.direction.y;
			z = l.sP.p.z + sortT[2] * l.direction.z;
			interL.eP.SetCoord(x, y, z);
			return interL;
		}
};

#endif