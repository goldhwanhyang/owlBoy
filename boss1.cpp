#include "stdafx.h"
#include "boss1.h"

HRESULT boss1::init()
{
	return S_OK;
}

HRESULT boss1::init(float x, float y)
{
	_count = _index = 0;
	_dir = 0;
	_x = x;
	_y = y;
	_speed = 2.0f;
	IGM->addFrameImage("보스1걷기", "Texture/Enemies/Boss1/bossWalk_3936x504_16x2.bmp", 3936, 504, 16, 2);
	IGM->addFrameImage("보스1쏘기", "Texture/Enemies/Boss1/bossShoot_1968x504_8x2.bmp", 1968, 504, 8, 2);

	_hitBox = RectMakeCenter(_x, _y+60, 230, 160);

	return S_OK;
}

void boss1::update()
{
	move();
	IGM->frameMake("보스1걷기", _count, _index, 1, 0, 7, _dir);
	_hitBox = RectMakeCenter(_x, _y + 60, 220, 160);
}

void boss1::render()
{
	IGM->frameRender("보스1걷기", getMemDC(), _x-135, _y-110);

	if (_isDebug)
	{
		RectangleLine(getMemDC(), _hitBox);
		wsprintf(_debug, "%d", _index);
		TextOut(getMemDC(), 100, 100, _debug, strlen(_debug));
		TextOut(getMemDC(), _x, _y, "X", strlen("X"));
	}
}

void boss1::release()
{
}

void boss1::attack()
{

}

void boss1::move()
{
	if (_x > IGM->findI("보스방1")->getWidth() - 200 && _dir == 0)
	{
		_dir = 1;
		_x = IGM->findI("보스방1")->getWidth() - 200 + 1;
	}
	else if (_x < 200 && _dir == 1)
	{
		_dir = 0;
		_x = 200 + 1;
	}

	if (_dir == 0) //오른쪽으로 간다
	{
		_x += _speed;
	}
	else if (_dir == 1) //왼쪽으로 간다
	{
		_x -= _speed;
	}
}
