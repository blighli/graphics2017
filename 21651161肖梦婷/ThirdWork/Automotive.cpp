//原始作者：朱慧君，提供了初步思路及简易的运动方程
//修改者：樊丽，精简并修改现有功能，以匹配AutoPart类

#include "Automotive.h"

myCar::myCar(void)
{
	this->normal3f	  = CVertex(0,1,0);
	this->position    = CVertex(0,0,0);
	this->ready       = 0;
	this->rotx        = 0;
	this->roty        = 0;
	this->rotz        = 0;
	this->wheel       = NULL;
	this->wheel_num   = 0;
	this->viewangle   = 0;
	m_carbody = new AutoPartBody("jeep.3ds");
}

myCar::~myCar(void)
{
}

bool myCar::CreateDemoCar(void)
{
	CVertex * newV;
	CVertex * zeroV;
	CVertex * defV;//标准精确度
	AutoPartWheel  * newP;
	int i,j;
	newV = NULL;
	newP = NULL;
	i=0;
	j=0;
	/////------------------------------------配置标准值
	double d_w1_2 = 1.0;////左右轮距的一半
	double d_w2_2 = 1.6;////前后轮距的一半---------轴距一半

	zeroV = new CVertex(0,0,0);
	defV = new CVertex(32,32,0);
	////////////////////原点
	this->Setpos(* zeroV);
	///////////////////方向

	this->CreateDemo4Wheel(d_w1_2, d_w2_2);
	m_carbody->DrawGL();
	this->ready=1;
	return true;
}

bool myCar::CreateWheel(int n)
{
	if (n>0)
	{
		if (this->wheel=new AutoPartWheel[n])
		{
			this->wheel_num = n;
			return true;
		}
	}
	return false;
}

bool myCar::Setpos(CVertex p)
{
	this->position = p;
	return false;
}

bool myCar::SetOri(CVertex ori)
{
	this->normal3f = ori;
	return false;
}

bool myCar::Rotxyz(double x, double y, double z)
{
	this->rotx += x;
	this->roty += y;
	this->rotz += z;
	while (rotz>360)    rotz -= 360;
	while (rotz<-360)   rotz += 360;
	return false;
}

bool myCar::ResetRot(void)
{
	this->rotx=0;
	this->roty=0;
	this->rotz=0;
	return false;
}

bool myCar::CreateDemo4Wheel(double dx, double dy)
{
	CVertex * newV;
	CVertex * newV2;
	CVertex * zeroV;
	CVertex * maxV;
	CVertex * defV;//标准精确度
	AutoPartWheel  * newP;
//	double ds2;
	int i,j;
	newV=NULL;
	newV2=NULL;
	newP=NULL;
	i=0;
	j=0;
	zeroV = new CVertex(0,0,0);
	maxV = new CVertex(1,1,1);
	defV = new CVertex(32,32,0);
//	ds2=ds/2;
	double r=0;
	double xx=0;
	double h1=0;
	double h2=0;
//		xx=ds/2 + 0.005;//厚度
//		r=dz/2;//三角半径半径
//		h1=r/2;
//		h2=sqrt(3.0)*h1;

	/////-----------------
	this->CreateWheel(4);


	newV = new CVertex(-dx, dy,0);
	this->wheel[0].Set(* newV);
	this->wheel[0].rotable=1;
	newV = new CVertex( dx, dy,0);
	this->wheel[1].Set(* newV);
	this->wheel[1].rotable=1;
	newV = new CVertex(-dx,-dy,0);
	this->wheel[2].Set(* newV);
	newV = new CVertex( dx,-dy,0);
	this->wheel[3].Set(* newV);
	return false;
}

bool myCar::DrawGL(void)
{
	int i=0;
	double rott=0;//////////轮子偏向，决定的车身偏向基量
	double ws=0;///////////轮子速度决定
	if (this->ready == 1)
	{
		glPushMatrix();

		glEnable(GL_DEPTH_TEST); //
		glDisable(GL_BLEND); // 关闭混合
		////-------------
		if (this->viewangle==0)////viewangle世界视角
		{
			glTranslatef(position.x,position.y,position.z);
			if (rotz>360) rotz-=360;
			if (rotz<0) rotz+=360;
			glRotatef(rotx,1.0f,0.0f,0.0f);
			glRotatef(roty,0.0f,1.0f,0.0f);
			glRotatef(rotz,0.0f,0.0f,1.0f);
		}
		else if (this->viewangle==3)
		{
			////////////-----------------------无移动效果，只有旋转效果
			glRotatef(0,  1.0f,0.0f,0.0f);
			glRotatef(0,  0.0f,1.0f,0.0f);
			//////////////----------------------车身偏向效果由轮子偏向和车的前进速度决定
			rott = this->wheel[0].rotz/2;
			ws   = this->wheel[0].speed;////
			rott = rott*ws/50;
			glRotatef(rott,0.0f,0.0f,1.0f);
		}
		m_carbody->DrawGL();
		for (i=0;i<this->wheel_num;++i)
		{
			//////////-------------------将车的偏向传给轮子
			if (this->viewangle==3)
			{				
				this->wheel[i].rot_fa.z=rott;
			}
			this->wheel[i].DrawGL();
		}
		glPopMatrix();
	}
	return true;
}

bool myCar::MoveVector(CVertex v)//向某个方向移动一定距离//只改变车原点
{
	this->position = this->position + v;
	return false;
}

bool myCar::WheelAllRot(double wrs, double wori)
{
	int i;
	for (i=0;i<this->wheel_num;++i)
	{
		this->wheel[i].distance -= wrs;/////车向与轮相反，所以减去
		this->wheel[i].speed = wrs;////轮速
		this->wheel[i].rotz = wori;/////范围控制在传入之前
		///////------------------范围控制
		while (this->wheel[i].rotx>360)
		{
			this->wheel[i].rotx -= 360;
		}
		while (this->wheel[i].rotx<0)
		{
			this->wheel[i].rotx += 360;
		}
	}
	return false;
}

void myCar::MovingCalc(double * wrs, double * wori, myCar * car)//////正常由公式提供相关数据，直接移动,这里作为测试特例
{
	double speed;
	double pi=3.1415926;
	if ((* wrs == 0)&&(* wori==0)) return;//无任何变化
	speed = pi*(* wrs)/180;//前进量
	speed = speed/1.15;              //速度修正,让轮子看起来更快---------->必要时考虑轮子方向与车的方向差

	//////////////----------------------进行旋转修正
	double x,y,z;
	double xx,yy,zz;
	double rx,ry,rz;
	/////////////-----------------------获取车的正向
	x = car->normal3f.x;
	y = car->normal3f.y;
	z = car->normal3f.z;
	/////////以车轮修正其前进方向--------------正向绕R转轮子偏差
	double wrot = (*wori)*pi/180/3;
	xx=x*cos(wrot)-y*sin(wrot);
	yy=x*sin(wrot)+y*cos(wrot);
	x=xx;
	y=yy;
	////////////------------------------计算得到车的实际前进方向
	rx= car->rotx;
	ry= car->roty;
	rz= car->rotz;
	////-------绕z转
	double rtz=rz*pi/180;
	xx=x*cos(rtz)-y*sin(rtz);
	yy=x*sin(rtz)+y*cos(rtz);
	x=xx;
	y=yy;
	////---------x
	double rtx=rx*pi/180;
	yy=y*cos(rtx)-z*sin(rtx);
	zz=y*sin(rtx)+z*cos(rtx);
	y=yy;
	z=zz;
	/////---------y
	double rty=ry*pi/180;
	xx=x*cos(rty)+z*sin(rty);
	zz=-x*sin(rty)+z*cos(rty);
	x=xx;
	z=zz;
	/////////-------------xyz为新的方向矢量
	/////////----------------得到前进矢量
	x=x*speed;
	y=y*speed;
	z=z*speed;
	////////-----------------------------------修正车身位置
	car->MoveVector(CVertex(x,y,z));
	///////------------------------------------修正车的方向
	if (fabs(*wrs)>2)
	{
		car->Rotxyz(0,0,(*wori)*(*wrs)/600);
	}
	///////////------------------修正车轮
	car->WheelAllRot(* wrs,* wori);
	///////////////----------------------------速度，方向，回弹
	if (*wrs>0)
	{
		* wrs = (* wrs)*0.98 - 0.001;////3由地面摩擦力决定
		if (* wrs < 0) 
		{
			(*wrs)=0;
		}
	}
	else if (*wrs<0)
	{
		* wrs =(* wrs)*0.95 + 0.01;////
		if (*wrs > 0)
		{
			(*wrs)= 0;
		}
	}
	if (*wori < 0)
	{
		*wori = (*wori)*0.98 + 0.2;
		if (*wori>0)
		{
			*wori=0;
		}
	}
	else if (*wori > 0)
	{
		*wori = (*wori)*0.98 - 0.2;
		if (*wori<0)
		{
			*wori=0;
		}
	}
}