#include "stdafx.h"
#include "fruit.h"

HRESULT fruit::init()
{
	liftableActor::init();
	
	if (RND->getInt(2) == 0)
		_img = IMAGEMANAGER->findImage("과일1");
	else
		_img = IMAGEMANAGER->findImage("과일2");


	_state = BURIED;
	_height = _img->getHeight() / 2;
	_maxHeight = _img->getHeight();
	_maxWidth = _img->getWidth();
	_isLiftable = true; _weight = 10;
	return S_OK;
}

void fruit::release()
{
}

void fruit::update()
{
	_hitBox = RectMakeCenter(_x, _y, _maxWidth, _maxHeight);
	if (_state == ON_AIR)
	{
		move();
		_hitBox = RectMakeCenter(_x, _y, _maxWidth, _maxHeight);
		collide();
	}//아래 검사
	
}

void fruit::render()
{
	_img->render(getMemDC(), _x - _img->getWidth() / 2 - CAM->getX(),
		_y - _img->getHeight() / 2 - CAM->getY(),
		0, 0, _img->getWidth(), _height);
}

liftableActor* fruit::use(actor * _player)
{
	int temp = _player->getHp() + 5;
	if (temp > _player->getMaxHp())
		temp = _player->getMaxHp();
	_player->setHp(temp);

	_isActive = false;

	return NULL;
}
