#include "stdafx.h"
#include "liftableActor.h"
#include "player.h"

HRESULT liftableActor::init()
{
	_mapPixel = IMAGEMANAGER->findImage("TownPixel");
	_isActive = true;
	return S_OK;
}

void liftableActor::release()
{
}

void liftableActor::update()
{
}

void liftableActor::render()
{
}


void liftableActor::lifted(player * _player)
{
	if (!_isLiftable) return;
	_z = 15;

	if (_state == BURIED)
	{ 
		_player->setX(_x);
		_player->setY(_hitBox.top);
		//_player->setState(FLY);

		_count = (_count + 1);
		if (_count % LIFT_TIME == 0)
		{
			_y -= 10;
			_height += 10;
			if (_height > _maxHeight)
			{
				_height = _maxHeight;
				_state = HANG;
				_y -= 10;
				EFFECT->generateEffect("ģ���������Ʈ", _player->getX(), _player->getY());
			}
		}

		return;
	}

	_state = HANG;
	_x = _player->getX();
	_y = _player->getY() + _maxHeight / 2;
}

void liftableActor::collide()
{
	COLORREF color;
	int r, g, b;
	//���� �˻�
	for (int i = _hitBox.top + 5; i >= _hitBox.top; --i)
	{
		color = GetPixel(_mapPixel->getMemDC(), _x, i);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if ((r == 0 && g == 0 && b == 0)) // �������� �˻�
		{
			_y = i + (_hitBox.bottom - _hitBox.top) / 2 + 1;
			_angle = 2 * PI - _angle;
			_speed /= 2;
			_hitBox = RectMakeCenter(_x, _y, _maxWidth, _maxHeight);
			//break;
		}
	}
	//�Ʒ� �˻�

	for (int i = _hitBox.bottom - 5; i <= _hitBox.bottom; ++i)
	{
		color = GetPixel(_mapPixel->getMemDC(), _x, i);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255) && _state != HANG)	// ����Ÿ�� �ƴϸ� �˻�
		{
			_y = i - (_hitBox.bottom - _hitBox.top) / 2 - 1;
			_state = ON_GROUND;
			if (_weight >= 50)
			{
				CAM->setShakeInfo(10, 20);
			}
			_z = 9;
			_hitBox = RectMakeCenter(_x, _y, _maxWidth, _maxHeight);
		}
	}
	//���� �˻�

	for (int i = _hitBox.left + 5; i >= _hitBox.left; --i)
	{
		color = GetPixel(_mapPixel->getMemDC(), i, _y);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if ((r == 0 && g == 0 && b == 0)) // ����Ÿ�� �ƴϸ� �˻� ���ٰ� �������̸� �˻�
		{
			_x = i + (_hitBox.right - _hitBox.left) / 2 + 1;
			//_angle = PI - _angle;
			_speed = 0;
			if (_weight >= 50)
			{
				CAM->setShakeInfo(10, 10);
			}
			_hitBox = RectMakeCenter(_x, _y, _maxWidth, _maxHeight);
			//break;
		}
	}

	//������ �˻�

	for (int i = _hitBox.right - 5; i <= _hitBox.right; ++i)
	{
		color = GetPixel(_mapPixel->getMemDC(), i, _y);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if ((r == 0 && g == 0 && b == 0))	// ����Ÿ�� �ƴϸ� �˻� ����Ÿ�� ����
											//�������� ���������� �ʷϻ��� ����ġ�� �ҷ��� 
		{
			_x = i - (_hitBox.right - _hitBox.left) / 2 - 1;
			//_angle = PI - _angle;
			_speed = 0;
			if (_weight >= 50)
			{
				CAM->setShakeInfo(10, 10);
			}
			_hitBox = RectMakeCenter(_x, _y, _maxWidth, _maxHeight);
			//break;
		}
	}
}

bool liftableActor::collide(actor * _player)
{
	RECT temp;
	if (IntersectRect(&temp, &_player->getHitbox(), &_hitBox))
	{
		return true;
	}
	return false;
}

void liftableActor::move()
{
	_gravity += 0.1f;
	_x += cos(_angle) * _speed;
	_y += -sin(_angle) * _speed + _gravity;
}

void liftableActor::throwed(float speed, float angle)
{
	actor::throwed(speed, angle);
	_state = ON_AIR;
}

