#include "stdafx.h"
#include "boss1.h"

HRESULT boss1::init()
{
	return S_OK;
}

HRESULT boss1::init(float x, float y)
{
	_count = _index = 0;
	_dir = 0;
	_x = x;
	_y = y;
	_angle = 0;
	_speed = 2.0f;
	_speed2 = 3.4f;
	IGM->addFrameImage("보스1걷기", "Texture/Enemies/Boss1/bossWalk_3936x504_16x2.bmp", 3936, 504, 16, 2);
	IGM->addFrameImage("보스1쏘기", "Texture/Enemies/Boss1/bossShoot_1968x504_8x2.bmp", 1968, 504, 8, 2);

	_bullet = new bullet;
	//_bullet->init();

	_hitBox = RectMakeCenter(_x, _y+60, 230, 160);
	_isShield = true;
	_isAttack = false;
	_state1 = ATTACK;

	return S_OK;
}

void boss1::update()
{
	//공격과 걷기 상태를 바꾸어주어야함
	//공격시 한발쏘고 WALK상태로 다시 ATTACK상태로 번갈아간다
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		_state1 = WALK;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		_state1 = ATTACK;
	}
	switch (_state1)
	{
	case READY:
		break;
	case WALK:
		move();
		IGM->frameMake("보스1걷기", _count, _index, 1, 0, 7, _dir);
		break;
	case ATTACK:
		move();
		attack();
		IGM->frameMake("보스1쏘기", _count, _index, 1, 0, 7, _dir);
		break;
	case TAKE_SHIELD:
		break;
	case OFF_SHIELD:
		break;
	case OFF_WALK:
		break;
	}

	_hitBox = RectMakeCenter(_x, _y + 60, 220, 160);
	_shieldHitBox = RectMakeCenter(_x, _y+20, 220, 240);
}

void boss1::render()
{
	//히트박스와 텍스쳐 위치 맞추기위해 방향에 따라 렌더바꿈
	if (_dir == 0)
	{
		switch (_state1)
		{
		case READY:
			break;
		case WALK:
			IGM->frameRender("보스1걷기", getMemDC(), _x - 135, _y - 110);
			break;
		case ATTACK:
			IGM->frameRender("보스1쏘기", getMemDC(), _x - 135, _y - 110);
			break;
		case TAKE_SHIELD:
			break;
		case OFF_SHIELD:
			break;
		case OFF_WALK:
			break;
		}
	}
	else if (_dir == 1)
	{
		switch (_state1)
		{
		case READY:
			break;
		case WALK:
			IGM->frameRender("보스1걷기", getMemDC(), _x - 120, _y - 110);
			break;
		case ATTACK:
			IGM->frameRender("보스1쏘기", getMemDC(), _x - 120, _y - 110);
			break;
		case TAKE_SHIELD:
			break;
		case OFF_SHIELD:
			break;
		case OFF_WALK:
			break;
		}
	}

	if (_isDebug)
	{
		RectangleLine(getMemDC(), _hitBox);
		RectangleLine(getMemDC(), _shieldHitBox);
		wsprintf(_debug, "%d", _isAttack);
		TextOut(getMemDC(), 100, 100, _debug, strlen(_debug));
		TextOut(getMemDC(), _x, _y, "X", strlen("X"));
	}
}

void boss1::release()
{
}

void boss1::attack()
{
	//공격중에도 이동이 계속되며 렌더할 프레임만 바뀐다.
	//감지범위에 플레이어가 없으면 50도로 쏜다.
	//감지범위에 플레이어가 있으면 getAngle(보스위치,플레이어위치)로 쏜다.
}

void boss1::move()
{
	//방향바꿈
	if (_x > IGM->findI("보스방1")->getWidth() - 310 && _dir == 0)
	{
		_dir = 1;
		_angle = 180;
	}
	else if (_x < 310 && _dir == 1)
	{
		_dir = 0;
		_angle = 0;
	}

	if (_isShield)
	{
		if (_dir == 0) //오른쪽으로 간다
		{
			_x += _speed * cosf(_angle * 0.017);
		}
		else if (_dir == 1) //왼쪽으로 간다
		{
			_x += _speed * cosf(_angle * 0.017);
		}
	}
	else
	{
		if (_dir == 0) //오른쪽으로 간다
		{
			_x += _speed2 * cosf(_angle * 0.017);
		}
		else if (_dir == 1) //왼쪽으로 간다
		{
			_x += _speed2 * cosf(_angle * 0.017);
		}
	}
}
