

#pragma once

#include "StdAfx.h"
#include "Vertex.h"
#include "AutoPart.h"


class myCar {
public:
	myCar(void);
	~myCar(void);
	
	double ready;//设定完毕
	CVertex position;//位置
	CVertex normal3f;//原始朝向//法向量//
	double rotx,roty,rotz;//旋转角
	
	int wheel_num;//轮子数量
	AutoPartWheel * wheel;//轮子部件
	int viewangle;
	
	bool CreateDemoCar(void);
	bool CreateWheel(int n);
	bool Setpos(CVertex p);      
	bool SetOri(CVertex ori);    
	bool Rotxyz(double x, double y, double z);
	bool ResetRot(void);          //复位车向
	bool CreateDemo4Wheel(double dx, double dy);
	bool DrawGL(void);
	
	bool MoveVector(CVertex v);
	bool WheelAllRot(double wrs, double wori);
	
	AutoPartBody * m_carbody;				//显示列表
	
	void MovingCalc(double * wrs, double * wori, myCar * car);//////正常由公式提供相关数据，直接移动,这里作为测试特例
};



class Automotive {
public:
	Automotive();
	~Automotive();
protected:
	;
private:
	;
};

