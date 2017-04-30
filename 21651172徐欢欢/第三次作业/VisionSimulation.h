
#pragma once

#include "Automotive.h"
#include "StdAfx.h"
#include "Vertex.h"
#include "AutoPart.h"
#include "Scene.h"


class GameScene
{
public:
	GameScene(void);
	~GameScene(void);
	
	int start;
	myCar * Cars;
	bool DrawGL(void);
	
	CEnvironment * scene;
	double rotx,roty,rotz;
	double deep;
};

/***如下类暂时未用***/
class VisionSimulation {
public:
	VisionSimulation(void);
	~VisionSimulation(void);
	BOOL DrawGL(void);
protected:
//	Automotive * Cars;
	myCar * Cars;
	CEnvironment * scene;
	double rotx,roty,rotz;
	double deep;
private:
	;
};