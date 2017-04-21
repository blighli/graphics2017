class Bound
{
private:
	int start, end;//指出该键始末原子编号
	int boundType;//指出是单键，双键还是三键

public:
	Bound();
	Bound(int start, int end, int boundType); //标准构造函数
	
	void setBounds(int start, int end, int boundType);
	int getStart();
	int getEnd();
	int getType();
};