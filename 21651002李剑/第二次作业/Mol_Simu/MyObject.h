/************************************************************************/
/*						自定义三维数据结构                       　 　　　*/
/************************************************************************/
#include <iostream>
#include "Vertex.h"
#include "Triangle.h"
#include "FixedArray.h"

using namespace std;

class MyObject
{
	MyObject(){}
public:
	//顶点数组
	FixedArray<Vertex> vArray;
	//三角片数组
	FixedArray<Triangle> triArray;
};