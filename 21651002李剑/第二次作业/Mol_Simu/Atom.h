#include <vector>
#include "Triangle.h"

using namespace std;

class Atom
{
private:
	double x, y, z;
	double r, fai,thita;
	int atomWeight;
	vector <Atom*> neighbors;//������ԭ��
public:
	void CalculateVertexNormal();
	void DeleteHiddenTri();
	void RemoveNeighbors();
	void translation();
	Atom();
	Atom(double, double, double, int);
	void SetAtom(double, double, double, int);
	void AddNeighbor(Atom*);
	void constructVDWSurface();
	void initVertex();
	void initTriangles();
	POINT3D mid(POINT3D, POINT3D);
	void divide(Triangle);
	void tessellation();
	vector <Atom*> GetNeighbors();
	double GetX();
	double GetY();
	double GetZ();
	double GetR();
	double GetF();
	double GetTH();
	int GetWeight();
	double VDWr;
	//��ԭ��������ԭ�������еı��
	int atomNum;
	//�����б�
	vector <Vertex> vertex;
	//�������б�
	vector <Triangle> triangle;
};