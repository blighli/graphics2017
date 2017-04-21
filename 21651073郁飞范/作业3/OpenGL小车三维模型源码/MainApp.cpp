/* Copyright (c) 2009, 长春孔辉汽车科技有限公司
 * All Rights reserved.
 * 文件名称：Main App
 * 文件标识
 * 摘要：主程序框架
 * 原作者：Jeff Molofee 于2000年
 * 感谢Fredric Echols For Cleaning Up
 * 原作者网址：nehe.gamedev.net
 * 朱慧君进行了第一版本修改，完善了键盘控制。
 * 最终修改：张沛轩，重新整理了并完善了代码
 */

#include "StdAfx.h"
#include <math.h>
#include "VisionSimulation.h"

HDC			hDC=NULL;		// OpenGL渲染描述表句柄
HGLRC		hRC=NULL;		// 窗口着色描述表句柄
HWND		hWnd=NULL;		// 保存我们的窗口句柄
HINSTANCE	hInstance;		// 保存程序的实例

BOOL	keys[256];			// 保存键盘按键状态的数组
BOOL	keyup[256];			// D
BOOL	active=TRUE;		// 窗口的活动标志，缺省为TRUE
BOOL	fullscreen=TRUE;	// 全屏缺省标志，缺省设定成全屏模式

//VisionSimulation VStest;
GameScene * GStest;
double w_roll_speed; //轮子转速（角度）
double w_ori;//轮子方向
void InitVisionSimulation();
void KeyControl();



LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// WndProc的定义

///////////////////////////////////////////////////////////////////////////////
//
// 重新设置OpenGL场景的大小
//
//
///////////////////////////////////////////////////////////////////////////////
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// 重新设置OpenGL窗口大小
{
	if (height==0)										// 防止被零除
	{
		height=1;										// 强行置1
	}
	
	glViewport(0,0,width,height);						// 重设当前的视口
	
	glMatrixMode(GL_PROJECTION);						// 选择投影矩阵
	glLoadIdentity();									// 重设投影矩阵
	
	// 计算并设置视口的大小
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	
	glMatrixMode(GL_MODELVIEW);							// 选择模型观察矩阵
	glLoadIdentity();									// 重置模型观察矩阵
}

///////////////////////////////////////////////////////////////////////////////
//
// 初始化OpenGL ，设置清除屏幕所用的颜色，打开深度缓存，启用阴影平滑等
//
//
///////////////////////////////////////////////////////////////////////////////

int InitGL(GLvoid)										// 此处开始对OpenGL进行所有设置
{
	glEnable(GL_TEXTURE_2D);							// 启用纹理映射
	glShadeModel(GL_SMOOTH);							// 启用阴影平滑
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// 背景色为 黑色
	glClearDepth(1.0f);									// 设置深度缓存
	glEnable(GL_DEPTH_TEST);							// 启用深度测试
	glDepthFunc(GL_LEQUAL);								// 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// 告诉系统对透视进行修正
	//////////////////////
	InitVisionSimulation();
	///////////////
	return TRUE;										// 返回初始化OK信息
}

///////////////////////////////////////////////////////////////////////////////
//
//包含了所有的绘图代码
//
//
///////////////////////////////////////////////////////////////////////////////

int DrawGLScene(GLvoid)									// 此过程中包括所有的绘制代码
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除屏幕及深度缓存
	glLoadIdentity();									// 重置当前的模型观察矩阵
/***************************/
	GStest->DrawGL();
	if (GStest->start==1)
		if (GStest->Cars->ready==1) 
			GStest->Cars->MovingCalc(&w_roll_speed,&w_ori,GStest->Cars);
	
/********************************/
	return TRUE;										// 返回绘图成功完成的消息
}

///////////////////////////////////////////////////////////////////////////////
//以下代码只在程序退出时调用。依次释放着色描述表，设备描述表和窗口句柄。
//并已加入相应的错误检查
//
//
///////////////////////////////////////////////////////////////////////////////
GLvoid KillGLWindow(GLvoid)								// 正常销毁窗口
{
	if (fullscreen)										// 是否处于全屏模式?
	{
		ChangeDisplaySettings(NULL,0);					// 如果出于全屏模式则回到桌面
		ShowCursor(TRUE);								// 显示鼠标指针
	}

	if (hRC)											// 我们拥有OpenGL渲染描述表么？
	{
		if (!wglMakeCurrent(NULL,NULL))					// 我们能否释放DC和RC描述表?
		{
			MessageBox(NULL,"释放DC或RC描述表失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// 能否删除RC?
		{
			MessageBox(NULL,"释放RC失败.","关闭错误",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// 设置RC为NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// 能否释放DC?
	{
		MessageBox(NULL,"释放DC失败.","关闭错误",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// 设置 DC 为 NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// 能否销毁窗口?
	{
		MessageBox(NULL,"销毁窗口失败.","关闭错误",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// 设置 hWnd 为 NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// 能否注销类？
	{
		MessageBox(NULL,"不能注销窗口类.","关闭错误",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// 设置 hInstance 为 NULL
	}
}


///////////////////////////////////////////////////////////////////////////////
// 以下代码创建我们的OpenGL窗口，提供5个参数
// title -窗口的标题栏
// width -窗口的宽度
// height -窗口的高度
// bits    -色彩位数 （16/24/32）
// fullscreenflag  -全屏幕标志  TRUE -全屏模式 FALSE -窗口模式
// 返回的BOOL值表示窗口是否成功创建
/////////////////////////////////////////////////////////////////////////////// 
BOOL CreateGLWindow(char* title, int width, int height, int bits, BOOL fullscreenflag)
{
	GLuint		PixelFormat;			// 保存查找匹配的结果
	WNDCLASS	wc;						// 窗口类结构
	DWORD		dwExStyle;				// 扩展窗口风格
	DWORD		dwStyle;				// 窗口风格
	RECT		WindowRect;				// 取得矩形的左上角和右下角的坐标值
	WindowRect.left=(long)0;			// 将Left设为0
	WindowRect.right=(long)width;		// 将Right设为要求的宽度
	WindowRect.top=(long)0;				// 将Top设为0
	WindowRect.bottom=(long)height;		// 将Bottom设为要求的高度

	fullscreen=fullscreenflag;			// 设置全局全屏标志

	hInstance			= GetModuleHandle(NULL);				// 取得我们窗口的实例
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// 移动时重画，并为窗口取得DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc处理消息
	wc.cbClsExtra		= 0;									// 无额外窗口数据
	wc.cbWndExtra		= 0;									// 无额外窗口数据
	wc.hInstance		= hInstance;							// 设置实例
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// 装入缺省图标
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// 装入鼠标指针
	wc.hbrBackground	= NULL;									// GL不需要背景
	wc.lpszMenuName		= NULL;									// 不需要菜单
	wc.lpszClassName	= "OpenGL";								// 设定类名字

	if (!RegisterClass(&wc))									// 尝试注册窗口类
	{
		MessageBox(NULL,"注册窗口类失败","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// 退出并返回FALSE
	}
	
	if (fullscreen)												// 尝试全屏模式吗?
	{
		DEVMODE dmScreenSettings;								// 设备模式
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// 确保内存清空为0
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Devmode结构的大小
		dmScreenSettings.dmPelsWidth	= width;				// 所选屏幕宽度
		dmScreenSettings.dmPelsHeight	= height;				// 所选屏幕高度
		dmScreenSettings.dmBitsPerPel	= bits;					// 每像素所选的色彩深度
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// 尝试设置显示模式并返回结果。（注：CDS_FULLSCREEN移去了状态条）
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// 若模式失败，提供两个选项：退出或窗口内运行
			if (MessageBox(NULL,"全屏模式在当前显卡上设置失败！\n使用窗口模式？","是否使用窗口模式？",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// 选择窗口模式：设置 Fullscreen = FALSE
			}
			else
			{
				// 弹出一个对话框，告诉用户程序结束
				MessageBox(NULL,"程序将被关闭.","错误",MB_OK|MB_ICONSTOP);
				return FALSE;									// 退出并返回 FALSE
			}
		}
	}

	if (fullscreen)												// 仍处于全屏模式么?
	{
		dwExStyle=WS_EX_APPWINDOW;								// 扩展窗体风格
		dwStyle=WS_POPUP;										// 窗体风格
		ShowCursor(FALSE);										// 隐藏鼠标指针
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// 扩展窗体风格
		dwStyle=WS_OVERLAPPEDWINDOW;							// 窗体风格
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// 调整窗口达到真正要求的大小

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// 扩展窗体风格
								"OpenGL",							// 类名字
								title,								// 窗口标题
								dwStyle |							// 选择的窗体属性
								WS_CLIPSIBLINGS |					// 必须的窗体风格属性
								WS_CLIPCHILDREN,					// 必须的窗体分割属性
								0, 0,								// 窗口位置
								WindowRect.right-WindowRect.left,	// 计算调整好的窗口宽度
								WindowRect.bottom-WindowRect.top,	// 计算调整好的窗口高度
								NULL,								// 无父窗口
								NULL,								// 无菜单
								hInstance,							// 实例
								NULL)))								// 不向WM_CREATE传递任何信息
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能创建一个窗口设备描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd 告诉窗口我们所希望的事情，即窗口使用的像素格式
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// 上述格式描述符的大小
		1,											// 版本号
		PFD_DRAW_TO_WINDOW |						// 格式支持窗口
		PFD_SUPPORT_OPENGL |						// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,							// 必须支持双缓冲
		PFD_TYPE_RGBA,								// 申请RGBA模式
		bits,										// 选定色彩深度
		0, 0, 0, 0, 0, 0,							// 忽略的色彩位
		0,											// 无Alpha缓存
		0,											// 忽略Shift Bit
		0,											// 无累加缓存
		0, 0, 0, 0,									// 忽略聚集位
		16,											// 16Bit Z缓存（深度缓存）  
		0,											// 无蒙版缓存
		0,											// 无辅助缓存
		PFD_MAIN_PLANE,								// 主绘图层
		0,											// Reserved
		0, 0, 0										// 忽略遮蔽罩
	};
	
	if (!(hDC=GetDC(hWnd)))							// 取得设备描述表了么?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能创建一种相匹配的像素格式.","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Windows找到相应的像素格式了么?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能设置像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回FASLE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// 是否可以设置像素格式?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"无法设置像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// 能否取得着色描述表?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能创建OpenGL渲染描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// 尝试激活着色描述表
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能激活当前的OpenGL渲染描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// 显示窗口
	SetForegroundWindow(hWnd);						// 略略提高优先级
	SetFocus(hWnd);									// 设置键盘的焦点至此窗口
	ReSizeGLScene(width, height);					// 设置透视GL屏幕

	if (!InitGL())									// 初始化新建的GL窗口
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"初始化失败","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回FALSE
	}

	return TRUE;									// 成功
}

///////////////////////////////////////////////////////////////////////////////
// 处理所有的窗口消息，当我们注册好窗口类之后，程序跳转到这部门代码处理窗口消息
//
//
//
///////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(	HWND	hWnd,			// 窗口的句柄
							UINT	uMsg,			// 窗口的消息
							WPARAM	wParam,			// 附加的消息内容
							LPARAM	lParam)			// 附加的消息内容
{
	switch (uMsg)									// 检查Windows消息
	{
		case WM_ACTIVATE:							// 监视窗口激活消息
		{
			if (!HIWORD(wParam))					// 检查最小化状态
			{
				active=TRUE;						// 令程序处于激活状态
			}
			else
			{
				active=FALSE;						// 令程序处于非激活状态
			}

			return 0;								// 返回消息循环
		}

		case WM_SYSCOMMAND:							// 系统中断命令
		{
			switch (wParam)							// 检查系统调用
			{
				case SC_SCREENSAVE:					// 屏幕保护要运行么?
				case SC_MONITORPOWER:				// 显示器要进入节电模式?
				return 0;							// 阻止以上情况的发生
			}
			break;									// 退出
		}

		case WM_CLOSE:								// 收到关闭消息?
		{
			PostQuitMessage(0);						// 发送一个退出消息
			return 0;								// 返回
		}

		case WM_KEYDOWN:							// 有键按下么?
		{
			keys[wParam] = TRUE;					// 如果是，标记为TRUE
			return 0;								// 返回
		}

		case WM_KEYUP:								// 有键放开么?
		{
			keys[wParam] = FALSE;					// 如果是，标记为FALSE
			return 0;								// 返回
		}

		case WM_SIZE:								// 调整OpenGL窗口大小
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// 返回
		}
	}

	// 向DefWindowProc传递所有未处理的消息。
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

///////////////////////////////////////////////////////////////////////////////
// Windows程序入口
// 调用窗口创建例程,处理窗口消息,并监视人机交互
//
//
//
///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE	hInstance,			// 当前窗口实例
				   HINSTANCE	hPrevInstance,		// 前一个窗口实例
				   LPSTR		lpCmdLine,			// 命令行参数
				   int			nCmdShow)			// 窗口显示状态
{
	MSG		msg;									// Windows消息结构
	BOOL	done=FALSE;								// 用来退出循环的BOOL变量
	
	// 提示用户选择运行模式
	if (MessageBox(NULL
		,"是否要以全屏方式运行程序?"
		, "启动全屏模式?"
		,MB_YESNO|MB_ICONQUESTION
		)==IDNO)
	{
		fullscreen=FALSE;							// 如果选否则启用窗口模式
	}
	
	// 创建OpenGL窗口
	if (!CreateGLWindow("汽车视景仿真开发框架",640,480,16,fullscreen))
	{
		return 0;									// 如果创建失败则退出
	}
	
	while(!done)									// 保持循环直到 done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// 有消息在等待么?
		{
			if (msg.message==WM_QUIT)				// 是否收到退出消息?
			{
				done=TRUE;							// 如果是则 done=TRUE
			}
			else									// 如果不是，处理窗口消息
			{
				TranslateMessage(&msg);				// 翻译消息
				DispatchMessage(&msg);				// 发送消息
			}
		}
		else										// 如果没有消息
		{
			// 绘制场景。监视ESC键和来自DrawGLScene()的退出消息
			if (active)								// 程序是激活的么?
			{
				if (keys[VK_ESCAPE])				// ESC 按下了么?
				{
					done=TRUE;						// ESC 发出退出信号
				}
				else								// 不是退出的时候，刷新屏幕
				{
					DrawGLScene();					// 绘制场景
					SwapBuffers(hDC);				// 交换缓存（双缓存）
					KeyControl();					// 处理键盘按键
				}
			}
			
			if (keys[VK_F1])						// F1键按下了么?
			{
				keys[VK_F1]=FALSE;					// 若是，使对应的Key数组中的值为FALSE
				KillGLWindow();						// 销毁当前的窗口
				fullscreen=!fullscreen;				// 切换 全屏/窗口 模式
				// 重建OpenGL窗口
				if (!CreateGLWindow("汽车视景仿真开发框架",640,480,16,fullscreen))
				{
					return 0;						// 如果无法创建窗口则退出程序
				}
			}
		}
	}
	
	// 关闭程序
	KillGLWindow();									// 销毁窗口
	return (msg.wParam);							// 退出程序
}

///////////////////////////////////////////////////////////////////////////////
// 初始化视景仿真
// 调用窗口创建例程,处理窗口消息,并监视人机交互
//
//
//
///////////////////////////////////////////////////////////////////////////////

void InitVisionSimulation()
{
	
	GStest = new GameScene();
	GStest->Cars=new myCar();
	GStest->Cars->CreateDemoCar();
	GStest->start = 1;
	w_roll_speed = 0;
	w_ori = 0;
}




void KeyControl()
{
			////////////////-------------------------------
			if (keys[VK_UP] && keyup[VK_UP]) // UP 键已按下并且松开了?
			{
				keyup[VK_UP]=false; // lp 设为 TRUE
		//		StartM001->ChoiceUp();
			}
			if (!keys[VK_UP]) // UP键松开了么?
			{
				keyup[VK_UP]=true; // 若是，则将lp设为FALSE
			}
			////////////////--------------------------------------
			if (keys[VK_DOWN] && keyup[VK_DOWN]) // DOWN键已按下并且松开了?
			{
				keyup[VK_DOWN]=false; // lp 设为 TRUE
			//	StartM001->ChoiceDown();
			}
			if (!keys[VK_DOWN]) // DOWN键松开了么?
			{
				keyup[VK_DOWN]=true; // 若是，则将lp设为FALSE
			}
			////////////-------------------------------回车键
			if (keys[VK_RETURN] && keyup[VK_RETURN]) 
			{
				keyup[VK_RETURN]=false;
			//	if (StartM001->ChoiceON())
			//	{
		//			if (StartM001->_choice==0)
			//		{
			//		GStest->start = 1;
			//		}
			//	}
			}
			if (!keys[VK_RETURN]) 
			{
				keyup[VK_RETURN]=true;
			}
			//////////--------------------
			if (keys[VK_NUMPAD8]) 
			{
				if (GStest->start==1)
				{
					GStest->rotx += 1;
				}
			}
			//////////--------------------
			if (keys[VK_NUMPAD2] )
			{
				if (GStest->start==1)
				{
					GStest->rotx -= 1;
				}
			}
			//////////--------------------
			if (keys[VK_NUMPAD4]) 
			{
				if (GStest->start==1)
				{
					GStest->roty -= 1;
				}
			}
			//////////--------------------
			if (keys[VK_NUMPAD6])
			{
				if (GStest->start==1)
				{
					GStest->roty += 1;
				}
			}
			//////////--------------------
			if (keys[VK_NUMPAD7])
			{
				if (GStest->start==1)
				{
					GStest->rotz -= 1;
				}
			}
			//////////--------------------
			if (keys[VK_NUMPAD9])
			{
				if (GStest->start==1)
				{
					GStest->rotz += 1;
				}
			}
			//////////-------------------
			if (keys[VK_PRIOR])
			{
				GStest->deep-= 0.1f;
			}
			if (keys[VK_NEXT])
			{
				GStest->deep+= 0.1f;
			}
			////----------------------
			if (keys[VK_UP]) // Up方向键按下了么?
			{
				if (GStest->start==1)
				if (GStest->Cars->ready == 1)
				{
					if (w_roll_speed<0)
					{
						w_roll_speed += 0.1;
					}
					else if (w_roll_speed<5)
					{
						w_roll_speed += 0.3;
					}
					else if (w_roll_speed<10)
					{
						w_roll_speed += 0.7;
					}
					else if (w_roll_speed<20)
					{
						w_roll_speed += 1.2;
					}
					else if (w_roll_speed<30)
					{
						w_roll_speed += 1.4;
					}
					else if (w_roll_speed<50) 
					{
						w_roll_speed += 1.0;
					}

					if (w_roll_speed>50) w_roll_speed =50;
				}
			}
			if (keys[VK_DOWN]) // Down方向键按下了么?
			{
				if (GStest->start==1)
				if (GStest->Cars->ready == 1)
				{
					if (w_roll_speed>0)
					{
						w_roll_speed -= 3;
					}
					else if (w_roll_speed>-10)
					{
						w_roll_speed -= 0.5;
					}
					else if (w_roll_speed>-20)
					{
						w_roll_speed -= 1.3;
					}
					else if (w_roll_speed>-30)
					{
						w_roll_speed -= 1.2;
					}
					if (w_roll_speed<-40) w_roll_speed =  -40;
				}
			}
			if (keys[VK_RIGHT]) // Right方向键按下了么?
			{
				if (GStest->start==1)
				{
					if (GStest->Cars->ready == 1)
					{
						::w_ori -= 1.8;
						if (::w_ori<-50)
						{
							::w_ori=-50;
						}
					}
				}
			}
			if (keys[VK_LEFT]) // Left方向键按下了么?
			{
				if (GStest->start==1)
				{
					if (GStest->Cars->ready == 1)
					{	
						::w_ori += 1.8;
						if (::w_ori>50)
						{
							::w_ori=50;
						}
					}
				}
			}
			/////////------------------------------------V   视角  viewangle
			if (keys['V'] && keyup['V']) 
			{
				keyup['V']=false;
				if (GStest->start==1)
				{
					if (GStest->Cars->ready == 1)
					{
						if (GStest->Cars->viewangle ==0)
						{
							GStest->Cars->viewangle = 3;
						}
						else if (GStest->Cars->viewangle ==3)
						{
							GStest->Cars->viewangle = 0;
						}
					}
				}
			}
			if (!keys['V']) 
			{
				keyup['V']=true;
			}

}

