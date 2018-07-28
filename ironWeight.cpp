#include "stdafx.h"
#include "ironWeight.h"
#include "player.h"

HRESULT ironWeight::init()
{
	stuff::init();
	//BURIED, ON_GROUND, HANG, FLY
	_img[BURIED] = IMAGEMANAGER->findImage("������");
	_img[ON_GROUND] = IMAGEMANAGER->findImage("������");
	_img[HANG] = IMAGEMANAGER->findImage("������");
	_img[ON_AIR] = IMAGEMANAGER->findImage("�����߰���");

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
	}//�Ʒ� �˻�
	if (_state == HANG)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			_y = _hitBox.bottom - (_hitBox.bottom - _hitBox.top) / 2;
			_state = ON_GROUND;
		}
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

	_img[_state]->frameRender(getMemDC(), _x - _img[_state]->getFrameWidth()/2 - CAM->getX(),
		_y - _img[_state]->getFrameHeight()/2 - CAM->getY(),
		0, 0, _img[_state]->getFrameWidth(), _height, _curFrameX, 0);
}


void ironWeight::move()
{
	_gravity += 0.1f;
	_x += cos(_angle) * _speed;
	_y += -sin(_angle) * _speed + _gravity;
}

void ironWeight::throwed(float speed, float angle)
{
	actor::throwed(speed, angle);
	_state = ON_AIR;
}

void ironWeight::collide()
{
	stuff::collide();
}


//
//void inputKey()
//{
//	enum {LEFT, RIGHT, UP, DOWN, NONE};
//	int xLine;
//	int yLine;
//
//	xLine = NONE;
//	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
//	{
//		xLine = LEFT;
//	}
//	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
//	{
//		xLine = RIGHT;
//	}
//
//	yLine = NONE;
//	if (KEYMANAGER->isStayKeyDown(VK_UP))
//	{
//		yLine = UP;
//	}
//	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
//	{
//		yLine = DOWN;
//	}
//
//	if (xLine != NONE && yLine != NONE)
//	{
//		_angle = getAngleByDir(xLine, yLine);
//		move()
//	}
//	
//
//	// Ű �Է�Ȯ���� ���� �� ���� Ű���� �Է� ������ �� �� �ִ�.
//}
//
//float getAngleByDir(int xLine, int yLine)
//{
//	enum { LEFT, RIGHT, UP, DOWN, NONE };
//	float angle;
//	if (xLine == LEFT)
//	{
//		if (yLine == NONE)
//			angle = 180;
//		else if (yLine == UP)
//			angle = 135;
//		else if (yLine == DOWN)
//			angle = 225;
//	}
//	else if (xLine == RIGHT)
//	{
//		if (yLine == NONE)
//			angle = 0;
//		else if (yLine == UP)
//			angle = 45;
//		else if (yLine == DOWN)
//			angle = 315;
//	}
//
//	return angle * PI / 180;
//}