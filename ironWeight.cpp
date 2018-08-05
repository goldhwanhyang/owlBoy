#include "stdafx.h"
#include "ironWeight.h"
#include "player.h"

HRESULT ironWeight::init()
{
	liftableActor::init();
	//BURIED, ON_GROUND, HANG, FLY
	_img[BURIED] = IMAGEMANAGER->findImage("무게추");
	_img[ON_GROUND] = IMAGEMANAGER->findImage("무게추");
	_img[HANG] = IMAGEMANAGER->findImage("무게추");
	_img[ON_AIR] = IMAGEMANAGER->findImage("무게추공중");

	_state = BURIED;
	_height = _img[HANG]->getFrameHeight() / 3;
	_maxHeight = _img[HANG]->getFrameHeight();
	_maxWidth = _img[HANG]->getFrameWidth();

	return S_OK;
}

void ironWeight::release()
{
}

void ironWeight::update()
{
	_hitBox = RectMakeCenter(_x, _y, _maxWidth, _maxHeight);
	if (_state == ON_AIR)
	{
		move();
		_hitBox = RectMakeCenter(_x, _y, _maxWidth, _maxHeight);
		collide();
	}//아래 검사
	//if (_state == HANG)
	//{
	//	COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
	//	int r = GetRValue(color);
	//	int g = GetGValue(color);
	//	int b = GetBValue(color);
	//
	//	if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
	//	{
	//		_y = _hitBox.bottom - (_hitBox.bottom - _hitBox.top) / 2;
	//		_state = ON_GROUND;
	//	}
	//}
}

void ironWeight::render()
{
	switch (_state)
	{
	case HANG:
		_count = (_count + 1) % 20;
		if (_count == 0)
		{
			_curFrameX = (_curFrameX + 1) % (_img[HANG]->getMaxFrameX() + 1);
		}
		break;
	default:
		_curFrameX = 1;
		break;
	}

	if (_state != ON_AIR)
	{
		_img[_state]->frameRender(getMemDC(), _x - _img[_state]->getFrameWidth() / 2 - CAM->getX(),
			_y - _img[_state]->getFrameHeight() / 2 - CAM->getY(),
			0, 0, _img[_state]->getFrameWidth(), _height, _curFrameX, 0);
	}
	else
	{
		_img[_state]->render(getMemDC(),
			_x - _img[_state]->getFrameWidth() / 2 - CAM->getX(),
			_y - _img[_state]->getFrameHeight() / 2 - CAM->getY());
	}
}
