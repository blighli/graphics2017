class Bound
{
private:
	int start, end;//ָ���ü�ʼĩԭ�ӱ��
	int boundType;//ָ���ǵ�����˫����������

public:
	Bound();
	Bound(int start, int end, int boundType); //��׼���캯��
	
	void setBounds(int start, int end, int boundType);
	int getStart();
	int getEnd();
	int getType();
};