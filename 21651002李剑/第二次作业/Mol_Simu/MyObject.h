/************************************************************************/
/*						�Զ�����ά���ݽṹ                       �� ������*/
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
	//��������
	FixedArray<Vertex> vArray;
	//����Ƭ����
	FixedArray<Triangle> triArray;
};