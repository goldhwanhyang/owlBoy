#include "stdafx.h"
#include "boss1.h"

HRESULT boss1::init(float x, float y)
{
	enemy::init(x, y);
	_count = _index = 0;
	_dir = 0;
	_angle = 0;
	_speed = 2.0f;
	_speed2 = 3.4f;
	_boss1Image[WALK] = IGM->addFrameImage("보스1걷기", "Texture/Enemies/Boss1/bossWalk_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_boss1Image[WALK_SHINING] = IGM->addFrameImage("보스1걷기빛", "Texture/Enemies/Boss1/bossWalkShining_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_boss1Image[ATTACK] = IGM->addFrameImage("보스1쏘기", "Texture/Enemies/Boss1/bossShoot_1968x504_8x2.bmp", 1968, 504, 8, 2);
	IGM->addImage("Null불릿", 10, 10);
	_bullet = new bullet;
	_bullet->init("Null불릿",5,1920);

	_hitBox = RectMakeCenter(_x, _y+60, 230, 160);
	_isShield = true;
	_isAttack = false;
	_state = WALK;

	return S_OK;
}

void boss1::update()
{
	enemy::update();
	//공격과 걷기 상태를 바꾸어주어야함
	//공격시 한발쏘고 WALK상태로 다시 ATTACK상태로 번갈아간다
	if (KEYMANAGER->isOnceKeyDown('G'))
	{
		if (_state == ATTACK)
		{
			_state = WALK;
		}
		else if (_state == WALK)
		{
			_state = ATTACK;
		}
	}

	switch (_state)
	{
	case READY:
		break;
	case WALK:
		move();
		break;
	case WALK_SHINING:
		move();
		break;
	case ATTACK:
		move();
		attack();
		break;
	case TAKE_SHIELD:
		break;
	case OFF_SHIELD:
		break;
	case OFF_WALK:
		break;
	}
	bool aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 7, _dir);

	if (_state == WALK && aniDone)
	{
		_state = WALK_SHINING;
	}
	else if (_state == WALK_SHINING && aniDone)
	{
		_state = WALK;
	}
	_hitBox = RectMakeCenter(_x, _y + 60, 220, 160);
	_shieldHitBox = RectMakeCenter(_x, _y+20, 220, 240);
}

void boss1::render()
{
	//히트박스와 텍스쳐 위치 맞추기위해 방향에 따라 렌더바꿈
	//기존의 frameMake를 사용했더니 움찔거림이 생겼기 때문에 currentX , Y에 _index와 _dir을 직접 써넣었다.  //_boss1Image[_state]->setFrameY(_dir);
	if (_dir == 0)
	{
		_boss1Image[_state]->frameRender(getMemDC(), _x - 135, _y - 110, _index, _dir);
	}
	else if (_dir == 1)
	{
		_boss1Image[_state]->frameRender(getMemDC(), _x - 120, _y - 110, _index, _dir);
	}

	if (_isDebug)
	{
		RectangleLine(getMemDC(), _hitBox);
		RectangleLine(getMemDC(), _shieldHitBox);
		wsprintf(_debug, "%d", _index);
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
	//WALK 8번 프레임과 ATTACK의 1번 프레임이 같은동작임
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

bool boss1::frameMake(image * bmp, int & count, int & index, int frameY1, int frameY2, int cooltime, bool renderDir)
{
	//if (renderDir)
	{
		//왼쪽방향으로 렌더
		count++;
		//bmp->setFrameY(frameY1);
		if (count % cooltime == 0)
		{
			index++;
			if (index > bmp->getMaxFrameX())
			{
				index = 0;
				return true;
			}
		//	bmp->setFrameX(index);
		}
	}
	return false;
}
