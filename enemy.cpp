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
		//���߿� �÷��̾� ��ȣ������ ��ġ�� �޾ƿð�
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
