#include "stdafx.h"
#include "actor.h"

void actor::move()
{
	_x = cos(_angle)*_speed;
	_y = -sin(_angle)*_speed;
}

void actor::throwed(float speed, float angle)
{
	_speed = speed;
	_angle = angle;
	_gravity = 0;
}

bool actor::collide(actor * a)
{
	RECT temp;

	return IntersectRect(&temp, &a->getHitbox(), &_hitBox);
}

HRESULT actor::init()
{
	_x = _y = 0;
	return S_OK;
}

void actor::update()
{
}

void actor::render()
{
}

void actor::release()
{
}

actor::actor()
{
	_x = _y = _z = _weight = _gravity = _speed = _power = _angle = 0;
	_hp = _maxHp = _state = _dir = _count = _index = _curFrameX = _curFrameY = 0;
	_hitBox = { 0, 0, 0, 0 };
	_isActive = false;
}
