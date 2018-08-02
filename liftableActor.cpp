#include "stdafx.h"
#include "liftableActor.h"
#include "player.h"

HRESULT liftableActor::init()
{
	_mapPixel = IMAGEMANAGER->findImage("TownPixel");
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

	if (_state == BURIED)
	{
		_player->setX(_x);
		_player->setY(_hitBox.top);

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
	//���� �˻�
	//for (int i = _hitBox.top; i < _hitBox.top + 1; i++)

	COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.top);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0)) // �������� �˻�
	{
		_y = _hitBox.top + (_hitBox.bottom - _hitBox.top) / 2;
		_angle = 3 * PI / 2;
		//break;
	}

	//�Ʒ� �˻�
	color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if (!(r == 255 && g == 0 && b == 255) && _state != HANG)	// ����Ÿ�� �ƴϸ� �˻�
	{
		_y = _hitBox.bottom - (_hitBox.bottom - _hitBox.top) / 2;
		_state = ON_GROUND;
	}

	//���� �˻�
	color = GetPixel(_mapPixel->getMemDC(), _hitBox.left, _y);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0)) // ����Ÿ�� �ƴϸ� �˻� ���ٰ� �������̸� �˻�
	{
		_x = _hitBox.left + (_hitBox.right - _hitBox.left) / 2;
		_angle = 3 * PI / 2;
		//break;
	}

	//������ �˻�
	color = GetPixel(_mapPixel->getMemDC(), _hitBox.right, _y);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0))	// ����Ÿ�� �ƴϸ� �˻� ����Ÿ�� ����
										//�������� ���������� �ʷϻ��� ����ġ�� �ҷ��� 
	{
		_x = _hitBox.right - (_hitBox.right - _hitBox.left) / 2;
		_angle = 3 * PI / 2;
		//break;
	}

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
