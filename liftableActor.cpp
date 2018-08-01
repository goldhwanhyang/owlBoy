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
				EFFECT->generateEffect("친구들기이펙트", _player->getX(), _player->getY());
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
	//위에 검사
	//for (int i = _hitBox.top; i < _hitBox.top + 1; i++)

	COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.top);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0)) // 검은색만 검사
	{
		_y = _hitBox.top + (_hitBox.bottom - _hitBox.top) / 2;
		_angle = 3 * PI / 2;
		//break;
	}

	//아래 검사
	color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if (!(r == 255 && g == 0 && b == 255) && _state != HANG)	// 마젠타가 아니면 검사
	{
		_y = _hitBox.bottom - (_hitBox.bottom - _hitBox.top) / 2;
		_state = ON_GROUND;
	}

	//왼쪽 검사
	color = GetPixel(_mapPixel->getMemDC(), _hitBox.left, _y);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0)) // 마젠타가 아니면 검사 였다가 검은색이면 검사
	{
		_x = _hitBox.left + (_hitBox.right - _hitBox.left) / 2;
		_angle = 3 * PI / 2;
		//break;
	}

	//오른쪽 검사
	color = GetPixel(_mapPixel->getMemDC(), _hitBox.right, _y);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0))	// 마젠타가 아니면 검사 마젠타를 무시
										//검은색은 안지나가고 초록색은 지나치게 할려면 
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

