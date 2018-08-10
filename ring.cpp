#include "stdafx.h"
#include "ring.h"
#include "player.h"

HRESULT ring::init(float x, float y, int dir)
{
	frontRing = IMAGEMANAGER->findImage("FRONT_RING");
	backRing = IMAGEMANAGER->findImage("BACK_RING");

	_isActive = true;

	_x = x;
	_y = y;
	_z = 5;
	_dir = dir;
	_dirY = RND->getInt(300) / 10.0f;

	_hitBox = RectMakeCenter(x, y, 40, 40);
	_count = 0;

	_alpha = 255;

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
	_dirY += 0.05f;
	if (_dirY > 2 * PI)
	{
		_dirY -= 2 * PI;
	}
	_y += -sin(_dirY) * 0.1;

	if (_curFrameX != 0)
	{
		_count = (_count + 1) % 10;
		if (_count == 0)
		{
			_curFrameX++;
			if (_curFrameX > frontRing->getMaxFrameX())
			{
				_curFrameX = frontRing->getMaxFrameX();
			}
		}
		if (_curFrameX == frontRing->getMaxFrameX())
		{
			_alpha -= 5;
			if (_alpha <= 0)
			{
				_alpha = 0;
				_isActive = false;
			}
		}
	}
}

void ring::render()
{

}
void ring::renderFront()
{
	if (_curFrameX == 0)
		frontRing->frameRender(getMemDC(), _x - frontRing->getFrameWidth() / 2 - CAM->getX(), _y - frontRing->getFrameHeight() / 2 - CAM->getY(), _curFrameX, _dir);
	else
		frontRing->alphaFrameRender(getMemDC(), _x - frontRing->getFrameWidth() / 2 - CAM->getX(), _y - frontRing->getFrameHeight() / 2 - CAM->getY(), _curFrameX, _dir, _alpha);
}
void ring::renderBack()
{
	if (_curFrameX == 0)
		backRing->frameRender(getMemDC(), _x - backRing->getFrameWidth() / 2 - CAM->getX(), _y - backRing->getFrameHeight() / 2 - CAM->getY(), _curFrameX, _dir);
	else
		backRing->alphaFrameRender(getMemDC(), _x - frontRing->getFrameWidth() / 2 - CAM->getX(), _y - frontRing->getFrameHeight() / 2 - CAM->getY(), _curFrameX, _dir, _alpha);
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
}
void ring::damaged(player * e)
{
}

void ring::collide()
{
}

bool ring::collide(player * a)
{
	RECT temp;

	if (_curFrameX == 0 && IntersectRect(&temp, &a->getHitbox(), &_hitBox))
	{
		_curFrameX = 1;
		SOUNDMANAGER->play("고리충돌", _effectVolume*2);
		a->sumCoin(5);
		return true;
	}
	return false;
}


