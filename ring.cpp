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
	frontRing->frameRender(getMemDC(), _x - CAM->getX(), _y - CAM->getY(), _curFrameX, _dir);
}
void ring::renderBack()
{
	backRing->frameRender(getMemDC(), _x - CAM->getX(), _y - CAM->getY(), _curFrameX, _dir);
}
void ring::damaged(actor * e)
{
}

void ring::collide()
{
}


