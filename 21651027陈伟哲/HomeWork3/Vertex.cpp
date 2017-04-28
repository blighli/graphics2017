// Vertex.cpp: implementation of the Vertex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vertex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//			Construction/Destruction
//////////////////////////////////////////////////////////////////////

Vertex::Vertex()
{

}

Vertex::Vertex(POINT3D p, POINT3D normal)
{
	this->p = p;
	this->normal = normal;
}

Vertex::~Vertex()
{

}

void Vertex::SetCoord(double x, double y, double z)
{
	this->p.SetXYZ(x, y, z);
}

void Vertex::SetCoord(POINT3D p)
{
	this->p = p;
}

void Vertex::SetNormal(double nx, double ny, double nz)
{
	this->normal.SetXYZ(nx, ny, nz); 
}

void Vertex::SetNormal(POINT3D n)
{
	this->normal = n;
}

Vertex IMPOSSIBLEP(POINT3D(9999, 9999, 9999), POINT3D(9999, 9999, 9999));