#ifndef __Mesh_H__
#define __Mesh_H__

#include "Triangle.h"
#include "Edge.h"
#include <vector>
using namespace std;

class Mesh
{
public:
	//����������������������Ƭ����
	vector<Vertex> vertexVec;
	vector<Edge> edgeVec;
	vector<Triangle> triVec;
	//���캯��
	Mesh()
	{
		vertexVec.clear();
		edgeVec.clear();
		triVec.clear();
	}
	//���������Σ���������Ӧ�Ķ��㣬��
	void AddTri(Triangle& tri)
	{
		
	}
};

#endif