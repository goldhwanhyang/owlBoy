#include "stdafx.h"
#include "enemy.h"
#include "player.h"

HRESULT enemy::init(float x, float y)
{
	_x = x;
	_y = y;
	_gravity = 0;

	return S_OK;
}

void enemy::update()
{

}

void enemy::render()
{

}

void enemy::release()
{

}
