#include "stdafx.h"
#include "tortoiseShield.h"
#include "player.h"

HRESULT tortoiseShield::init(float x, float y)
{
	_x = x;
	_y = y;
	_gravity = 0;
	_z = 10;

	_image = IGM->findImage("거북이_떨어진방패");
	_isActive = false;
	_gravity = 0;
	_index = _count = 0;

	_maxWidth = 100;
	_maxHeight = 100;
	return S_OK;
}

void tortoiseShield::update()
{
	if (_isActive) 
	{
		move();
		collide();
		_hitBox = RectMakeCenter(_x, _y, SHIELD_CONST::HITBOX_WIDTH, SHIELD_CONST::HITBOX_HEIGHT-80);
		//방패 프레임 돌리기, 맥스프레임이면 프레임 안돈다.
		if (_index != _image->getMaxFrameX())
		{
			_count++;
			if (_count % 7 == 0)
			{
				_index++;
			}
		}
	}

	if (!_isActive)
	{
		_hitBox = RectMakeCenter(_x, _y + 20, SHIELD_CONST::HITBOX_WIDTH, SHIELD_CONST::HITBOX_HEIGHT);
		_index = 0; //실드가 달려있을땐 인덱스를 0으로
		_gravity = 0;
	}
}

void tortoiseShield::render()
{
	//if (_isActive)
	//{
		_image->frameRender(getMemDC(), _x - 95 - CAM->getX(), _y - 50 - CAM->getY(), _index, _dir);
	//}
	if (_isDebug)
	{
		TextOut(getMemDC(), 140, 120, "방패", strlen("방패"));
		_stprintf_s(_debug, "angle: %f, speed: %f, gravity: %f", _angle, _speed, _gravity);
		TextOut(getMemDC(), 140, 140, _debug, strlen(_debug));
		//Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
	if (_state == HANG)
	{
		image *temp = IMAGEMANAGER->findImage("LIFT2");
		temp->frameRender(getMemDC(), temp->getX(), temp->getY());
	}
}

void tortoiseShield::release()
{
}

void tortoiseShield::damaged(actor * e)
{
	_index = 0;
	//떨어진 실드를 때렸을때의 반응
}

void tortoiseShield::move()
{
	_gravity += 0.05f;
	//if (310 < _x && _x < IMAGEMANAGER->findImage("보스방1")->getWidth() - 310)
	{
		_x += _speed * cos(_angle);
	}
	_y += _speed * -sin(_angle) + _gravity;
}

void tortoiseShield::collide()
{
	//_speed값만큼 게임오브젝트가 칸칸히 위치변경(이동)하기 때문에 _speed값 범위(벽을 최대로 뚫고 들어가는)만큼 검사해야함
	//위쪽
	for (int i = _hitBox.top + _speed; i >= _hitBox.top; i--) // 스피드만큼 검사해야 오차없이 깔끔하게, 움직이는 방향으로 검사해야 논리적인 오류가 없다.
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = i + SHIELD_CONST::HITBOX_HEIGHT/2;
			_speed = 0; //속도는 방패가 떨어질때 tortoise가 넘겨줌
			break;
		}
	}

	//왼쪽
	for (int i = _hitBox.left + _speed; i >= _hitBox.left; i--)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetRValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_x = i + SHIELD_CONST::HITBOX_WIDTH / 2;
			_angle = 4.71; //270도
			break;
		}
	}

	//오른쪽
	for(int i =_hitBox.right-_speed; i <= _hitBox.right; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetRValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_x = i - SHIELD_CONST::HITBOX_WIDTH / 2;
			_angle = 4.71;
			break;
		}
	}

	//아래쪽
	for (int i = _hitBox.bottom - _speed; i <= _hitBox.bottom; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = i - (SHIELD_CONST::HITBOX_HEIGHT-80) / 2;
			if(_state != HANG && _speed !=0) CAM->setShakeInfo(10, 20); // 흔들어줄 백그라운드 이미지에 getSX(), getSY()를 넣고 setShakeInfo로 쉐킷 실행
			_speed = 0;
			_gravity = 0;
			break;
		}
	}
}

void tortoiseShield::lifted(player * _player)
{
	if (_isActive)
	{
		_state = HANG;
		_gravity = 0;
		_index = 3;
		_x = _player->getX();
		_y = _player->getY() + _maxHeight / 2;
	}
}

void tortoiseShield::throwed(float speed, float angle)
{
	actor::throwed(speed, angle);
	_state = IDLE;
	_hitBox = RectMakeCenter(_x, _y, SHIELD_CONST::HITBOX_WIDTH, SHIELD_CONST::HITBOX_HEIGHT - 80);
}

