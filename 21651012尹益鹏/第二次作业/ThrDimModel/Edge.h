#ifndef __Edge_H__
#define __Edge_H__

class Edge
{
public:
	//����ֹ���ڵ���е�����ֵ
	int sV, eV;
	int GetAnotherVIndex(int v)
	{
		if (v == sV) return eV;
		if (v == eV) return sV;
		//���û��һ����ͬ�����أ�����־��ѯ����
		else return -1;
	}
};

#endif