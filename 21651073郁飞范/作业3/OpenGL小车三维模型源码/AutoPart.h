/* Copyright (c) 2009, 长春孔辉汽车科技有限公司
* All Rights reserved.
* 文件名称：StdAfx.h
* 摘要：车的部件
* 作者：张沛轩（电子邮件）
*/

#pragma once

#include "StdAfx.h"
#include "Vertex.h"
#include "3DS.h"

class AutoPart {
public:
	AutoPart(char *filename,int n=0){
		char appdir[666];
		GetCurrentDirectory(666,appdir);
		SetCurrentDirectory("Data");
		m_3ds=new CLoad3DS();
		m_3ds->Init(filename,n);
		SetCurrentDirectory(appdir);
	}
	virtual ~AutoPart(){
		delete m_3ds;
	}
	virtual BOOL DrawGL()=0;
protected:
	CLoad3DS *m_3ds;
	GLuint m_autoPartList;
private:
	;
};


class AutoPartBody:public AutoPart {
public:
	AutoPartBody(char *filename,int n=0);
	~AutoPartBody(){};
	BOOL DrawGL();
protected:
	;
private:
	;
};

class AutoPartWheel:public AutoPart {
public:
	AutoPartWheel(char *filename="wheel.3ds",int n=1);
	~AutoPartWheel(){};
	BOOL Set(CVertex newpos);////////////////相当于MOVE TO
	BOOL Rotxyz(double x, double y, double z);
	BOOL DrawGL();
public:
	GLdouble rotx,roty,rotz;
	GLdouble distance;
	GLdouble speed;
	CVertex pos;//位置，原点
	CVertex rot_fa;///父部件的旋转量
	int rotable;
	int blend;
	int blendf;
private:
	;
};




