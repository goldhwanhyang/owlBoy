#include "stdafx.h"
#include "boss1.h"
#include "player.h"

HRESULT boss1::init(float x, float y)
{
	//  ===============추가할 이미지=================
	IGM->addFrameImage("보스1떨어진방패", "Texture/Enemies/Boss1/bossShieldDrop_3672x366_17_2.bmp", 3672, 366, 17, 2);
	IGM->addFrameImage("보스1불릿", "Texture/Enemies/Boss1/bossBullet_525x138_7x2.bmp", 525, 138, 7, 2);
	IGM->addFrameImage("보스1걷기방패없이", "Texture/Enemies/Boss1/bossWalkOff_1968x504_8x2.bmp", 1968, 504, 8, 2);

	//  ============================================
	
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
	_shieldDropImage = IGM->findImage("보스1떨어진방패");
	_delayCount = 0;

	_hitBox = RectMakeCenter(_x, _y + 60, 230, 160);
	_isShield = true;
	_isAttack = false;
	_attackCount = 0;
	_state = WALK;

	bullet blt;
	blt.init(10, 5,IGM->findImage("보스방1")->getWidth(), "보스1불릿");
	for (int i = 0; i < 7; ++i)
	{
		_vBullet.push_back(blt);
	}

	return S_OK;
}

void boss1::update()
{
	_playerX = _player->getX();
	_playerY = _player->getY();
	//프레임 돌려줌
	bool aniDone;
	if (TURN != _state)	aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 7, _dir);
	else aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 12, _dir);

	switch (_state)
	{
	case READY:
		break;
	case TURN:					//회전
		if (aniDone) turn();
		break;
	case WALK:					//걷기
		move();
		break;
	case WALK_SHINING:			//걷다가 빛남
		move();
		break;
	case ATTACK:				//총쏨
		move();
		attack();
		break;
	case TAKE_SHIELD:			//방패줍기
		break;
	case OFF_SHIELD:			//방패 떨어뜨리기
		break;
	case OFF_WALK:				//방패없이 걷기
		break;
	}

	//상태변경
	if (_state == WALK && aniDone)
	{
		//WALK상태에서 ATTACK상태가 되거나 WALK_SHINING상태가 된다
		if (RND->getInt(4) > 1)
		{
			_state = ATTACK;
		}
		else _state = WALK_SHINING;
	}
	else if (_state == WALK_SHINING && aniDone)
	{
		//WALK_SHINING상태에서 WALK상태가 된다
		_state = WALK;
	}

	_hitBox = RectMakeCenter(_x, _y + 60, 220, 160);
	_shieldHitBox = RectMakeCenter(_x, _y+20, 220, 240);

	//불릿무브
	Bmove();
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
	
	Brender();

	if (_isDebug)
	{
		RectangleLine(getMemDC(), _hitBox);
		RectangleLine(getMemDC(), _shieldHitBox);
		_stprintf_s(_debug, "PX: %f, X: %f", _playerX , _x);
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
		if (_playerX > _x)
		{
			Bfire(utl::getAngle(_x, _y, _playerX, _playerY) * 58.8);
			//감지범위에 플레이어가 있으면 getAngle(보스위치,플레이어위치)로 쏜다.
		}
		else
		{
			Bfire();
			//감지범위에 플레이어가 없으면 50도로 쏜다.
		}
	}
	else if (_dir == LEFT)
	{
		if (_playerX < _x)
		{
			Bfire(utl::getAngle(_x, _y, _playerX, _playerY) * 58.8);
			//감지범위에 플레이어가 있으면 getAngle(보스위치,플레이어위치)로 쏜다.
		}
		else
		{
			Bfire();
			//감지범위에 플레이어가 없으면 50도로 쏜다.
		}
	}

	//불릿을 쏠때 카운트를 올리고 8발 쏘면 상태변화
	if (_attackCount > 8)
	{
		_state = WALK_SHINING;
		_attackCount = 0;
	}
}

void boss1::move()
{
	if (_dir == RIGHT)
	{
		//턴상태가 될때 index와 count를 0으로해야 Trun의 프레임이 작동한다.
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


void boss1::Bfire(float angle)
{
	//딜레이 주는방법
	_delayCount = (_delayCount + 1) % 30;
	if (_delayCount != 0) return;

	for (int i = 0; i < _vBullet.size(); ++i)
	{
		if (_vBullet[i].getIsActive()) continue;

		_vBullet[i].setIsActive(true);
		_vBullet[i].setFireLocationXY(_x, _y - 45);
		_vBullet[i].setAngle(angle);
		_vBullet[i].setDir(_dir);
		++_attackCount;
		break;
	}
}

void boss1::Bmove()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].update();
	}
}

void boss1::Brender()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].render();
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
