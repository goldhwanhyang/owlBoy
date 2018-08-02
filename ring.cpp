#include "stdafx.h"
#include "ring.h"

HRESULT ring::init()
{
	frontRing = IMAGEMANAGER->findImage("FRONT_RING");
	backRing = IMAGEMANAGER->findImage("BACK_RING");

	_x = _y = 0; _z = 5;

	return S_OK;
}

void ring::release()
{
}
void ring::update()
{
}

void ring::render()
{
}
void ring::renderFront()
{
}
void ring::renderBack()
{
}
void ring::damaged(actor * e)
{
}

void ring::collide()
{
}


