#include "stdafx.h"
#include "ring.h"

HRESULT ring::init(float x, float y, int dir)
{
	frontRing = IMAGEMANAGER->findImage("FRONT_RING");
	backRing = IMAGEMANAGER->findImage("BACK_RING");

	_x = x;
	_y = y;
	_z = 5;
	_dir = dir;

	_hitBox = RectMakeCenter(x, y, 20, 20);

	return S_OK;
}

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
	frontRing->frameRender(getMemDC(), _x -frontRing->getFrameWidth()/2 - CAM->getX(), _y - frontRing->getFrameHeight() / 2 - CAM->getY(), _curFrameX, _dir);
}
void ring::renderBack()
{
	backRing->frameRender(getMemDC(), _x - backRing->getFrameWidth() / 2 - CAM->getX(), _y - backRing->getFrameHeight() / 2 - CAM->getY(), _curFrameX, _dir);

	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
}
void ring::damaged(actor * e)
{
}

void ring::collide()
{
}


