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

	_lifted = IMAGEMANAGER->findImage("LIFT2");

	_state = BURIED;
	_height = _img[HANG]->getFrameHeight() / 2;
	_maxHeight = _img[HANG]->getFrameHeight() - 30;
	_maxWidth = _img[HANG]->getFrameWidth() - 20;

	_power = 100;



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
	}
	if (_state != BURIED)
	{
		_height = _img[HANG]->getFrameHeight();
	}
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
			_x - _img[_state]->getWidth() / 2 - CAM->getX(),
			_y - _img[_state]->getHeight() / 2 - CAM->getY());
	}
	if (_state == HANG && _lifted->getX() != -1)
	{
		_lifted->frameRender(getMemDC(), _lifted->getX() - _lifted->getFrameWidth() / 2 - CAM->getX(), _lifted->getY() - _lifted->getFrameHeight() / 2 - CAM->getY());

	}

	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
}


