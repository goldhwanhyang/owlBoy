#include "stdafx.h"
#include "boss1.h"
#include "player.h"

HRESULT boss1::init(float x, float y)
{
	//  ===============추가할 이미지=================
	IGM->addFrameImage("보스1떨어진방패", "Texture/Enemies/Boss1/bossShieldDrop_3672x366_17_2.bmp", 3672, 366, 17, 2);
	IGM->addFrameImage("보스1불릿", "Texture/Enemies/Boss1/bossBullet_525x138_7x2.bmp", 525, 138, 7, 2);
	IGM->addFrameImage("보스1걷기방패없이", "Texture/Enemies/Boss1/bossOffWalk_1968x504_8x2.bmp", 1968, 504, 8, 2);
	IGM->addFrameImage("보스1방패떨어뜨리기", "Texture/Enemies/Boss1/bossOffShield_522x504_2x2.bmp", 522, 504, 2, 2);

	//  ============================================
	
	enemy::init(x, y);
	_count = _index = 0;
	_dir = RIGHT;
	_angle = 0;
	_speed = 2.0f;
	_boss1Image[TURN] = IGM->findImage("보스1꺽기");
	_boss1Image[WALK] = IGM->findImage("보스1걷기");
	_boss1Image[WALK_SHINING] = IGM->findImage("보스1걷기빛");
	_boss1Image[ATTACK] = IGM->findImage("보스1쏘기");
	_boss1Image[OFF_SHIELD] = IGM->findImage("보스1방패떨어뜨리기");
	_boss1Image[OFF_WALK] = IGM->findImage("보스1걷기방패없이");
	_shieldDropImage = IGM->findImage("보스1떨어진방패");
	_delayCount = 0;

	_hitBox = RectMakeCenter(_x, _y + 60, 230, 160);
	_isAttack = false;
	_attackCount = 0;
	_state = WALK;

	ZeroMemory(&_shield, sizeof(Shield));
	_shield.on = true;
	_shield.offSpeed = 3.4f;

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
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		damaged(_player);
	}

	_playerX = _player->getX();
	_playerY = _player->getY();
	//프레임 돌려줌
	bool aniDone;
	if (TURN != _state || OFF_SHIELD != _state)	aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 7, _dir);
	else aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 12, _dir);
	
	if (!_shield.on && _shield.index != _shieldDropImage->getMaxFrameX()) //맥스프레임이면 프레임 안돈다.
	{
		frameMake(_shieldDropImage, _shield.count, _shield.index, 1, 0, 7, _dir);
	}

	switch (_state)
	{
	case READY:
		break;
	case TURN:					//회전
		if (aniDone) turn();	//TURN상태가 되면 이미지프레임이 돌고 다돌고나면(aniDone==true) 방향바꿈
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
		shieldOff();
		break;
	case OFF_WALK:				//방패없이 걷기
		moveOff();
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

	if (_shield.on) //실드가 달리면 보스위치를 따라감
	{
		_shield.x = _x;
		_shield.y = _y;
	}
	_hitBox = RectMakeCenter(_x - CAM->getX(), _y + 60 - CAM->getY(), 220, 160);
	_shield.hitBox = RectMakeCenter(_shield.x - CAM->getX(), _shield.y + 20 - CAM->getY(), 220, 240); //실드가 보스에게 달려있음

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

	if (!_shield.on)
	{
		_shieldDropImage->frameRender(getMemDC(), _shield.x - CAM->getX(), _shield.y - CAM-> getY(), _shield.index, _dir);
	}
	
	//불릿렌더
	Brender();

	if (_isDebug)
	{
		RectangleLine(getMemDC(), _hitBox);
		RectangleLine(getMemDC(), _shield.hitBox);
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
			Bfire(utl::getAngle(_x, _y, _playerX, _playerY) * 58.8); //degree값으로 다시 바꾸기 위해 58.8를 곱했다.
			//감지범위에 플레이어가 있으면 getAngle(보스위치,플레이어위치)로 쏜다.
		}
		else
		{
			Bfire(35);
			//감지범위에 플레이어가 없으면 35도로 쏜다.
		}
	}
	else if (_dir == LEFT)
	{
		if (_playerX < _x)
		{
			Bfire(utl::getAngle(_x, _y, _playerX, _playerY) * 58.8); //degree값으로 다시 바꾸기 위해 58.8를 곱했다.
			//감지범위에 플레이어가 있으면 getAngle(보스위치,플레이어위치)로 쏜다.
		}
		else
		{
			Bfire(145);
			//감지범위에 플레이어가 없으면 145도로 쏜다.
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

	_x += _speed * cosf(_angle * 0.017);
}

void boss1::shieldOff()
{
	//차후에 actor::throwed(_speed, _angle)를 사용해서 움직임을 구현하자.
	
	//임시
	_x += 1;
	_shield.x -= 1;

	COLORREF color = GetPixel(IGM->findImage("보스방1픽셀")->getMemDC(), _x, _hitBox.bottom);
}

void boss1::moveOff()
{
	_angle = utl::getAngle(_x, _y, _shield.x, _shield.y);
	_x += _shield.offSpeed * cosf(_angle);
}

void boss1::damaged(actor * e)
{
	//플레이어에 의해 공격당했을때 보스가 해야할 액션

	//임시
	if (PtInRect(&_hitBox, _ptMouse) && _x > _ptMouse.x) //왼쪽에서 오른쪽에 있는 보스를 때림
	{
		_state = OFF_SHIELD;
		_shield.on = false;
	}
	if (PtInRect(&_hitBox, _ptMouse) && _x < _ptMouse.x) //오른쪽에서 왼쪽에 있는 보스를 때림
	{
		_state = OFF_SHIELD;
		_shield.on = false;
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
	//=============

	for (int i = 0; i < _vBullet.size(); ++i)
	{
		if (_vBullet[i].getIsActive()) continue;

		_vBullet[i].setIsActive(true);
		_vBullet[i].setFireCenter(_x, _y - 45);
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
		_vBullet[i].collide("보스방1픽셀");
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