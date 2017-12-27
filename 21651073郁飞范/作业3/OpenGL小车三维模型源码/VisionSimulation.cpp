//视景类
//原始作者为朱慧君
//张沛轩对代码进行了进一步精简和完善
//樊丽加入的场景显示的功能

#include "VisionSimulation.h"
#include "3DS.H"

CLoad3DS *m_3ds;
GLuint  m_lilisit;

GameScene::GameScene(void)
{
	this->start = 0;
	this->Cars=NULL;
	this->rotx = -60;
	this->roty = 0;
	this->rotz = 40;
	this->deep = -15;
	this->scene = new CEnvironment;


	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; // 环境光参数
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // 漫射光参数

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); // 设置环境光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); // 设置漫射光
}

GameScene::~GameScene(void)
{
}

bool GameScene::DrawGL(void)
{

		glLoadIdentity(); // 重置当前的模型观察矩阵

		glEnable(GL_DEPTH_TEST); //
		glDisable(GL_BLEND); // 关闭混合
		glEnable(GL_COLOR_MATERIAL); // 使用颜色材质-------------->

		/////////----------------
		/////-------------------------视角处理
		if (this->Cars->viewangle==0)//////////////-----------------世界视角
		{
			///////------------------------------------
			glTranslatef(0.0f,0.0f,deep); // 移入屏幕------
			glRotatef(rotx,1.0f,0.0f,0.0f);
			glRotatef(roty,0.0f,1.0f,0.0f);
			glRotatef(rotz,0.0f,0.0f,1.0f);
		}
		if (this->Cars->viewangle==3)//当视角为3时，第三人称视角
		{			
			glTranslatef(0.0f,0.0f,deep); // 移入屏幕------
			//////////---------------------------------------视角变化
			glRotatef(- 70,  1.0f,0.0f,0.0f);
			glRotatef(  0,   0.0f,1.0f,0.0f);
			glRotatef(  0,   0.0f,0.0f,1.0f);
		}
		/////////-------绘制世界
		{
			////////////////////--------------------------------------------------------------------世界位置
			glPushMatrix();
			if (this->Cars->viewangle==0)
			{
				//////////------------------------------------------------绘制光源
				///////----------------
				glPushMatrix();
					glTranslatef(5,5,10);// 光源位置
					GLfloat LightPosition[]= { 0, 0, 0, 1.0f }; 
					glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // 设置光源位置
					glEnable(GL_LIGHTING); // 启用光源
					glEnable(GL_LIGHT1); // 启用一号光源
				glPopMatrix();
			}
			if (this->Cars->viewangle==3)//当视角为第三人称视角时，世界位置反转
			{
				glRotatef(  -this->Cars->rotx,   1.0f,0.0f,0.0f);
				glRotatef(  -this->Cars->roty,   0.0f,1.0f,0.0f);
				glRotatef(  -this->Cars->rotz,   0.0f,0.0f,1.0f);
				//////////------------------------------------------------绘制光源
				///////----------------
				glPushMatrix();
					glTranslatef(5,5,10);// 光源位置
					GLfloat LightPosition[]= { 0, 0, 0, 1.0f }; 
					glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // 设置光源位置
					glEnable(GL_LIGHTING); // 启用光源
					glEnable(GL_LIGHT1); // 启用一号光源
				glPopMatrix();
				//////////-------------光源写在世界修正之前是太阳------，写在之后是灯泡
				glTranslatef(- this->Cars->position.x,- this->Cars->position.y,- this->Cars->position.z);
			}
			///------------------------------------------////////////////----------------------------绘制地面
			{
				glPushMatrix();
				glTranslatef(0.0,0.0,-0.475);
				glTranslatef(-MAP,MAP,0.0);
				this->scene->DrawSand();
				glTranslatef(MAP,-MAP,0.0);

				glPopMatrix();
			}
			glPopMatrix();
		}
		if(this->Cars!=NULL)
		{
			this->Cars->DrawGL();
		}
	return false;
}
