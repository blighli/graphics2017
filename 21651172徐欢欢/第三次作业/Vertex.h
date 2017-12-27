
#pragma once

#include "StdAfx.h"


class CVertex {
public:
	CVertex(GLdouble _x = 0.0 
		,GLdouble _y = 0.0
		,GLdouble _z = 0.0
		,GLdouble _w = 1.0)
		:x(_x),y(_y),z(_z),w(_w) {}
	~CVertex(){}
	CVertex operator+(const CVertex & _add)	{
		return CVertex(this->x+_add.x,this->y+_add.y,z+_add.z);
	}
	CVertex operator-(const CVertex & _sub)	{
		return CVertex(this->x-_sub.x,this->y-_sub.y,this->z-_sub.z);
	}
	GLdouble x,y,z,w;
};
