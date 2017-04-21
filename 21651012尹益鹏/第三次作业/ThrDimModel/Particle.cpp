// Particle.cpp : implementation file
//

#include "stdafx.h"
#include "mol_simu.h"
#include "Particle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParticle

CParticle::CParticle()
{
}

CParticle::CParticle(double x, double y, double z, double r, int atom, int type)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->r = r;
	this->atom = atom;
	this->type = type;
}

CParticle::~CParticle()
{
}

/////////////////////////////////////////////////////////////////////////////
// CParticle message handlers

void CParticle::setParticle(double x, double y, double z, double r, int atom, int type)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->r = r;
	this->atom = atom;
	this->type = type;
}
