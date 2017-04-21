#include "stdafx.h"
#include "Bound.h"

Bound::Bound()
{
	this->start = 0;
	this->end = 0;
	this->boundType = 0;
}

Bound::Bound(int s, int e, int t)
{
	this->start = s;
	this->end = e;
	this->boundType = t;
}

void Bound::setBounds(int s, int e, int t)
{
	this->start = s;
	this->end = e;
	this->boundType = t;
}

int Bound::getStart()
{
	return this->start;
}

int Bound::getEnd()
{
	return this->end;
}

int Bound::getType()
{
	return this->boundType;
}