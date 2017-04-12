/************************************************************************/
/* 三维点或向量                                                          */
/************************************************************************/
#ifndef __POINT3D_H__
#define __POINT3D_H__

#include "Math.h"
#include <vector>
using namespace std;

class POINT3D
{
	public:
		double r, theta, fai;
		double x, y, z;
	public:
		//构造函数
		POINT3D(){}
		POINT3D(double x, double y, double z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		inline void SetPolarCoord(double r, double theta, double fai)
		{
			this->r = r;
			this->theta = theta;
			this->fai = fai;
			SetXYZCoord();
		}
		inline void SetXYZCoord()
		{
			this->x = r * cos(theta) * sin(fai);
			this->y = r * sin(theta) * sin(fai);
			this->z = r * cos(fai);
		}
		inline void SetXYZ(double x, double y, double z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		inline double distance(POINT3D& p)
		{
			double dx = this->x - p.x;
			double dy = this->y - p.y;
			double dz = this->z - p.z;
			return sqrt(dx * dx + dy * dy + dz * dz);
		}
		//向量归一化
		void nomalize()
		{
			double d = x * x + y * y + z * z;
			//除法变乘法加快运算速度
			double denominator = 1 / sqrt(d);
			x = x * denominator;
			y = y * denominator;
			z = z * denominator;
		}
		//求向量点积
		static double innerProduct(POINT3D& p1, POINT3D& p2)
		{
			return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
		}
		//求向量叉积
		static POINT3D crossProduct(POINT3D& p1, POINT3D& p2)
		{
			POINT3D pAns(p1.y * p2.z - p2.y * p1.z, p2.x * p1.z - p1.x * p2.z, p1.x * p2.y - p2.x * p1.y);
			return pAns;
		}
		//判断是否为相同点
		bool IsSamePnt(POINT3D& p)
		{
			if (p.x == x && p.y == y && p.z == z) return true;
			else return false;
		}
};

#endif 