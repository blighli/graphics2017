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
		bool isBorder;//是否是边界三角片
		vector<int> neighborAtom; // 邻近原子列表：供边界三角片求交所用
		vector<Line> intersectLines; //交线列表，供边界三角片求交重新三角化所用
		vector<POINT3D> interP; //三角片内交点列表，供边界三角片求交重新三角化所用

		POINT3D center; //三角形中心
		double r; //大致半径

		Line ab, bc, ca; //三角片三条边
		Plane3D plane; //三角片所在的平面对象
		int atomNum;//该三角片所属原子编号
		POINT3D normal;
		int drawAtomWeight;
		//构造函数
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
			//存放边的向量
			POINT3D edge1(b.p.x - a.p.x, b.p.y - a.p.y, b.p.z - a.p.z);
			POINT3D edge2(c.p.x - a.p.x, c.p.y - a.p.y, c.p.z - a.p.z);
			//两条边向量的叉积即是三角形法向量
			normal = POINT3D::crossProduct(edge1, edge2);
 			//归一化
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
		inline bool isStandard(){return Area2(a.p, b.p, c.p) < 0;}//判断点C是不是在AB的左边
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
		//判断此三角形是否与给定直线相交
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
		/* 得到此三角片所在的平面                                                 */
		/************************************************************************/
		void CalculatePlane()
		{
			Plane3D p(normal, a.p);
			plane = p;
		}
		/************************************************************************/
		/* 求三角形的中心以及外接圆半径                                           */
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
		/* 求该三角片与指定三角片的交线段                                         */
		/************************************************************************/
		Line IntersectLine(Triangle& tri)
		{
			Line interL;
			POINT3D v[2][3]; 
			//先得到两平面交线
			Line l = plane.IntersectWithPlane(tri.plane);
			//首先求得三角片ｔｒｉ相对于当前三角片位于同一侧的两个顶点
			//求ｔｒｉ三个顶点到当前三角片所在平面的有向距离
			double d[2][3];
			POINT3D p0 = tri.GetA().p;
			POINT3D p1 = tri.GetB().p;
			POINT3D p2 = tri.GetC().p;
			d[0][0] = plane.distance(p0); d[0][1] = plane.distance(p1); d[0][2] = plane.distance(p2);
			//如果全在同侧，则无交线，返回不可能线
			if (d[0][0] < 0 && d[0][1] < 0 && d[0][2] < 0 || d[0][0] > 0 && d[0][1] > 0 && d[0][2] > 0)
				return IMPOSSBLELINE;
			//判断符号,其中符号相同的两点在同侧(不会有都在同侧的情况)
			if (d[0][0] * d[0][1] > 0) {v[0][0] = p0; v[0][1] = p1; v[0][2] = p2;}
			else if (d[0][0] * d[0][2] > 0) {v[0][0] = p0; v[0][1] = p2; v[0][2] = p1;}
			else if (d[0][1] * d[0][2] > 0) {v[0][0] = p1; v[0][1] = p2; v[0][2] = p0;}
			//再判断是否有一个顶点在对方三角形平面上的情况
			if (d[0][0] == 0) {v[0][0] = p0; v[0][1] = p1; v[0][2] = p2;}
			else if (d[0][1] == 0) {v[0][0] = p1; v[0][1] = p0; v[0][2] = p2;}
			else if (d[0][2] == 0) {v[0][0] = p2; v[0][1] = p0; v[0][2] = p1;}
			//求当前三角片顶点相对于ｔｒｉ所在平面的有向距离
			tri.CalculatePlane();
			d[1][0] = tri.plane.distance(a.p); d[1][1] = tri.plane.distance(b.p); d[1][2] = tri.plane.distance(c.p);
			//如果全在同侧，则无交线，返回不可能线
			if (d[1][0] < 0 && d[1][1] < 0 && d[1][2] < 0 || d[1][0] > 0 && d[1][1] > 0 && d[1][2] > 0)
				return IMPOSSBLELINE;
			//判断符号,其中符号相同的两点在同侧(不会有都在同侧的情况)
			if (d[1][0] * d[1][1] > 0) {v[1][0] = a.p; v[1][1] = b.p; v[1][2] = c.p;}
			else if (d[1][0] * d[1][2] > 0) {v[1][0] = a.p; v[1][1] = c.p; v[1][2] = b.p;}
			else if (d[1][1] * d[1][2] > 0) {v[1][0] = b.p; v[1][1] = c.p; v[1][2] = a.p;}
			//再判断是否有一个顶点在对方三角形平面上的情况
			if (d[1][0] == 0) {v[1][0] = a.p; v[1][1] = b.p; v[1][2] = c.p;}
			else if (d[1][1] == 0) {v[1][0] = b.p; v[1][1] = a.p; v[1][2] = c.p;}
			else if (d[1][2] == 0) {v[1][0] = c.p; v[1][1] = a.p; v[1][2] = b.p;}
			//将两个三角形的三个顶点分别向交线上投影
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
				//投影确定已被正确求出!!!
// 				for (j = 0; j < 2; j++)
// 				{
// 					//求t00，t01,t10,t11
// 					//t[i][j] = v1[i][j] + (v1[i][2] - v1[i][j]) * d[i][j] / (d[i][j] - d[i][2]);
// 					t[i][j] = (fabs(d[i][j]) * v1[i][2] + fabs(d[i][2]) * v1[i][j]) / 
// 						(fabs(d[i][j]) + fabs(d[i][2]));
// 					sortT[count] = t[i][j];
// 					count++;
// 				}
			}
// 			//对ｓｏｒｔＴ由小到大数组排序
// 			for (i = 0; i < 3; i++)
// 				for (int j = i + 1; j < 4; j++)
// 					if (sortT[j] < sortT[i])
// 					{
// 						double temp = sortT[i];
// 						sortT[i] = sortT[j];
// 						sortT[j] = temp;
// 					}
// 			//取中间两个为共有区间
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
			//四个交点
			POINT3D pInter[4];
			count = 0;
			double maxT[2], minT[2]; //两条线段各两个交点Ｔ值
			maxT[0] = maxT[1] = -9999;
			minT[0] = minT[1] = 9999;
			for (i = 0; i < 2; i++)
			{
				//ｉ指示线段编号
				for (int j = 0; j < 2; j++)
				{
					lTemp.sP.p = v[i][j];
					lTemp.eP.p = v[i][2];
					lTemp.calculateDirection();
					pInter[count] = lTemp.IntersectWithLine(l);
					//计算交点在交线上的ｔ参数值
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
			//如果没有两条线段没有交集，则证明两块三角片不相交，返回不可能直线
			if (minT[0] > maxT[1] || maxT[0] < minT[1])
			{
				return IMPOSSBLELINE;
			}
			//对ｓｏｒｔＴ由小到大数组排序
			for (i = 0; i < 3; i++)
				for (int j = i + 1; j < 4; j++)
					if (sortT[j] < sortT[i])
					{
						double temp = sortT[i];
						sortT[i] = sortT[j];
						sortT[j] = temp;
					}
			//取中间两个为共有区间
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