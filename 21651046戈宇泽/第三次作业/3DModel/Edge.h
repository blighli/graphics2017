#ifndef __Edge_H__
#define __Edge_H__

class Edge
{
public:
	//边起止点在点表中的索引值
	int sV, eV;
	int GetAnotherVIndex(int v)
	{
		if (v == sV) return eV;
		if (v == eV) return sV;
		//如果没有一个相同，返回－１标志查询错误
		else return -1;
	}
};

#endif