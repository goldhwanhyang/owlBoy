#include "stdafx.h"
#include "boss1.h"

HRESULT boss1::init(float x, float y)
{
	enemy::init(x, y);
	_count = _index = 0;
	_dir = RIGHT;
	_angle = 0;
	_speed = 2.0f;
	_speed2 = 3.4f;
	_boss1Image[TURN] = IGM->findImage("보스1꺽기");
	_boss1Image[WALK] = IGM->findImage("보스1걷기");
	_boss1Image[WALK_SHINING] = IGM->findImage("보스1걷기빛");
	_boss1Image[ATTACK] = IGM->findImage("보스1쏘기");
	//IGM->addImage("Null불릿", 10, 10);
	//_bullet = new bullet;

	_hitBox = RectMakeCenter(_x, _y+60, 230, 160);
	_isShield = true;
	_isAttack = false;
	_attackCount = 0;
	_state = WALK;

	return S_OK;
}

void boss1::update()
{
	enemy::update();
	//공격과 걷기 상태를 바꾸어주어야함
	//공격시 한발쏘고 WALK상태로 다시 ATTACK상태로 번갈아간다
	bool aniDone;
	if (TURN != _state)
	{
		aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 7, _dir);
	}
	else
	{
		aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 12, _dir);
	}

	switch (_state)
	{
	case READY:
		break;
	case TURN:
		if (aniDone) turn();
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

	if (_state == WALK && aniDone)
	{
		//WALK상태에서 ATTACK상태가 되거나 WALK_SHINING상태가 된다
		if(RND->getInt(4) > 1) _state = ATTACK;
		else _state = WALK_SHINING;
	}
	else if (_state == WALK_SHINING && aniDone)
	{
		//WALK_SHINING상태에서 WALK상태가 된다
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
		_boss1Image[_state]->frameRender(getMemDC(), _x - 135 - CAM->getX(), _y - 110 - CAM->getY(), _index, _dir);
	}
	else if (_dir == 1)
	{
		_boss1Image[_state]->frameRender(getMemDC(), _x - 120 - CAM->getX(), _y - 110 - CAM->getY(), _index, _dir);
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
	if (_dir == RIGHT)
	{
		if (_playerX > _hitBox.right)
		{
			//감지범위에 플레이어가 있으면 getAngle(보스위치,플레이어위치)로 쏜다.
		}
		else
		{
			//감지범위에 플레이어가 없으면 50도로 쏜다.
		}
	}
	else if (_dir == LEFT)
	{
		if (_playerX < _hitBox.left)
		{
			//감지범위에 플레이어가 있으면 getAngle(보스위치,플레이어위치)로 쏜다.
		}
		else
		{
			//감지범위에 플레이어가 없으면 50도로 쏜다.
		}
	}
	++_attackCount;
	//불릿을 쏠때 카운트를 올리고 8발 쏘면 상태변화
	if (_attackCount > 8*10)
	{
		_state = WALK_SHINING;
		_attackCount = 0;
	}
}

void boss1::move()
{
	//방향에따라 다른 동작
	//_x끝점이면 턴상태가 될것, x좌표 업데이트를 멈출것->_angle을 90으로
	//턴상태가 끝나면 방향 바꾸게 할것
	if (_dir == RIGHT)
	{
		if (_x > IGM->findI("보스방1")->getWidth() - 310)
		{
			_state = TURN;
			_index = 0;
			_count = 0;
			_dir = LEFT;
		}
	}
	else if (_dir == LEFT)
	{
		if (_x < 310)
		{
			_state = TURN;
			_index = 0;
			_count = 0;
			_dir = RIGHT;
		}
	}

	if (_isShield)
	{
		_x += _speed * cosf(_angle * 0.017);
	}
	else
	{
		_x += _speed2 * cosf(_angle * 0.017);
	}
}

void boss1::turn()
{
	//방향바꿈
	if (_dir == RIGHT)
	{
		_state = WALK;
		_angle = 0;
	}
	else if (_dir == LEFT)
	{
		_state = WALK;
		_angle = 180;
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
