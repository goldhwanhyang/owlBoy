#include "stdafx.h"
#include "enemy.h"

HRESULT enemy::init(float x, float y)
{
	_playerX = 0;
	_playerY = 0;

	_x = x;
	_y = y;

	return S_OK;
}

void enemy::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		//나중에 플레이어 상호참조로 위치를 받아올것
		_playerX = _ptMouse.x;
		_playerY = _ptMouse.y;
	}
}

void enemy::render()
{

}

void enemy::release()
{

}
