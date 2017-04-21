#ifndef __Mesh_H__
#define __Mesh_H__

#include "Triangle.h"
#include "Edge.h"
#include <vector>
using namespace std;

class Mesh
{
public:
	//顶点索引，边索引，三角片索引
	vector<Vertex> vertexVec;
	vector<Edge> edgeVec;
	vector<Triangle> triVec;
	//构造函数
	Mesh()
	{
		vertexVec.clear();
		edgeVec.clear();
		triVec.clear();
	}
	//增加三角形，并增加相应的顶点，边
	void AddTri(Triangle& tri)
	{
		
	}
};

#endif