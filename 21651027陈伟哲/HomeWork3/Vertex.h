// Vertex.h: interface for the Vertex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERTEX_H__17FDCA03_C7AF_4803_B11B_687C157E46D0__INCLUDED_)
#define AFX_VERTEX_H__17FDCA03_C7AF_4803_B11B_687C157E46D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "POINT3D.H"

class Vertex  
{
public:
	//顶点位置以及法向
	POINT3D p, normal;
public:
	void SetNormal(POINT3D);
	void SetNormal(double, double, double);
	void SetCoord(POINT3D);
	void SetCoord(double, double, double);
	Vertex();
	Vertex(POINT3D, POINT3D);
	virtual ~Vertex();
};

#endif // !defined(AFX_VERTEX_H__17FDCA03_C7AF_4803_B11B_687C157E46D0__INCLUDED_)
