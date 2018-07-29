#include "stdafx.h"
#include "fruit.h"

HRESULT fruit::init()
{
	_img = IMAGEMANAGER->findImage("과일1");


	_state = BURIED;
	_height = _img->getHeight() / 3;
	_maxHeight = _img->getHeight();
	_maxWidth = _img->getWidth();

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
	if (_state == HANG)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
		{
			_y = _hitBox.bottom - (_hitBox.bottom - _hitBox.top) / 2;
			_state = ON_GROUND;
		}
	}
}

void fruit::render()
{
	_img->render(getMemDC(), _x - _img->getWidth() / 2 - CAM->getX(),
		_y - _img->getHeight() / 2 - CAM->getY(),
		0, 0, _img->getFrameWidth(), _height);
}

void fruit::used(player * _player)
{
}
