#include "stdafx.h"
#include "tortoisePhase2.h"
#include "player.h"

//hit박스의 크기, 이미지 좌표 보정값등을 숫자가 아니라 WINSIZEX, _image->getWidth() 등을 사용해서 표현할수 있게 해주자.

HRESULT tortoisePhase2::init(float x, float y, int dir)
{
	enemy::init(x, y);
	_count = _index = 0;
	_dir = dir;
	if(dir == RIGHT) _angle = 0;
	else _angle = 180;
	_speed = 2.2f;
	_tortoiseImage[READY] = IGM->findImage("거북이_페이즈2_준비");
	_tortoiseImage[FLY] = IGM->findImage("거북이_페이즈2_날기");
	_tortoiseImage[ATTACK] = IGM->findImage("거북이_페이즈2_쏘기");
	_tortoiseImage[TAKE_SHIELD] = IGM->findImage("거북이_페이즈2_방패줍기");
	_tortoiseImage[OFF_SHIELD] = IGM->findImage("거북이_페이즈2_방패떨어뜨리기");
	_tortoiseImage[OFF_STUN] = IGM->findImage("거북이_페이즈2_스턴");
	_tortoiseImage[OFF_FLY] = IGM->findImage("거북이_페이즈2_날기방패없이");

	_delayCount = 0;

	_hitBox = RectMakeCenter(_x, _y + 60, 230, 160);
	_isAttack = false;
	_attackCount = 0;
	_state = FLY;

	_offSpeed = PHASE2_CONST::DEFAULT_OFF_SPEED;

	bullet blt;
	blt.init(10, 5, IGM->findImage("보스방1")->getWidth(), "거북이_불릿");
	for (int i = 0; i < 7; ++i)
	{
		_vBullet.push_back(blt);
	}

	_maxHp = 100;
	_hp = 100;

	return S_OK;
}

void tortoisePhase2::update()
{

	
	//TODO : 임시
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		damaged(_player);
	}

	_playerX = _player->getX();
	_playerY = _player->getY();
	_isActiveShield = _shield->getIsActive();

	//프레임 돌려줌
	bool aniDone;
	if (OFF_SHIELD != _state || OFF_STUN != _state)	aniDone = frameMake(_tortoiseImage[_state], _count, _index, 1, 0, 7, _dir);
	else aniDone = frameMake(_tortoiseImage[_state], _count, _index, 1, 0, 12, _dir);

	switch (_state)
	{
	case READY:
		break;
	case FLY:						//날기
		move();
		break;
	case ATTACK:					//총쏨
		move();
		attack();
		break;
	case TAKE_SHIELD:
		if (aniDone) takeShield();	//방패줍기
		break;
	case OFF_SHIELD:				//방패 떨어뜨리기
		shieldOff();
		break;
	case OFF_STUN:					//스턴
		if (stun()) _state = OFF_FLY;
		break;
	case OFF_FLY:					//방패없이 날기
		moveOff();
		break;
	}

	//상태변경
	if (_state == FLY && aniDone)
	{
		//WALK상태에서 ATTACK상태가 되거나 WALK_SHINING상태가 된다
		if (RND->getInt(4) > 1)
		{
			_state = ATTACK;
		}
		else _state = FLY;
	}

	_hitBox = RectMakeCenter(_x, _y + 60, 220, 160);

	//실드를 활성하면 보스위치를 따라감
	if (_isActiveShield)
	{
		_shield->setX(_x);
		_shield->setY(_y);
	}

	//불릿무브
	Bmove();
}

void tortoisePhase2::render()
{
	//히트박스와 텍스쳐 위치 맞추기위해 방향에 따라 렌더바꿈
	//기존의 frameMake를 사용했더니 움찔거림이 생겼기 때문에 currentX , Y에 _index와 _dir을 직접 써넣었다.  //_boss1Image[_state]->setFrameY(_dir);

	if (_isDebug)
	{
		IMAGEMANAGER->findImage("보스방1픽셀")->render(getMemDC(), 0, 0, CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
	}
	if (_state != TAKE_SHIELD)
	{
		if (_dir == 0)
		{
			_tortoiseImage[_state]->frameRender(getMemDC(), _x - 135 - CAM->getX(), _y - 110 - CAM->getY(), _index, _dir);
		}
		else if (_dir == 1)
		{
			_tortoiseImage[_state]->frameRender(getMemDC(), _x - 120 - CAM->getX(), _y - 110 - CAM->getY(), _index, _dir);
		}
	}
	else
	{
		if (_dir == 0)
		{
			_tortoiseImage[_state]->frameRender(getMemDC(), _x - 135 - CAM->getX(), _y - 180 - CAM->getY(), _index, _dir);
		}
		else if (_dir == 1)
		{
			_tortoiseImage[_state]->frameRender(getMemDC(), _x - 120 - CAM->getX(), _y - 180 - CAM->getY(), _index, _dir);
		}
	}

	//불릿렌더
	Brender();

	if (_isDebug)
	{
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
		_stprintf_s(_debug, "angle: %f, offSpeed: %f", _angle, _offSpeed);
		TextOut(getMemDC(), 100, 100, _debug, strlen(_debug));
		TextOut(getMemDC(), _x, _y, "X", strlen("X"));
	}
}

void tortoisePhase2::release()
{
}

void tortoisePhase2::attack()
{
	//공격중에도 이동이 계속되며 렌더할 프레임만 바뀐다.
	//2페이즈에서는 뒤로도 쏜다
	Bfire(utl::getAngle(_x, _y, _playerX, _playerY) * 58.8); //degree값으로 다시 바꾸기 위해 58.8를 곱했다.

	//불릿을 쏠때 카운트를 올리고 8발 쏘면 상태변화, count++은 Bfire에 있음
	if (_attackCount > 8)
	{
		_state = FLY;
		_attackCount = 0;
	}
}

void tortoisePhase2::move()
{
	_angle = utl::getAngle(_x, _y, _playerX, _playerY);
	_x += _speed * cosf(_angle);
	_y += _speed * -sinf(_angle);

	if (_playerX > _x)
	{
		_dir = RIGHT;
	}
	else if (_playerX < _x)
	{
		_dir = LEFT;
	}
}

void tortoisePhase2::collide()
{
	//TODO : 플레이어랑 보스몸체랑 충돌했을때?
}

void tortoisePhase2::shieldOff()
{
	_gravity += 0.05;
	float temp = utl::getAngle(_playerX, _playerY, _x, _y);
	if (301 < _x && _x < IMAGEMANAGER->findImage("보스방1")->getWidth() - 310)
	{
		_x += 3 * cosf(temp);
	}
	_y += -2 * -sinf(temp) + _gravity;

	_shield->throwed(6, temp); //TODO : 진동, 통통 버그

	//보스몸체 픽셀충돌
	COLORREF color = GetPixel(IMAGEMANAGER->findImage("보스방1픽셀")->getMemDC(), _x, _hitBox.bottom);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	if (!(r == 255 && g == 0 && b == 255))
	{
		_y = _hitBox.bottom - 160;
		_gravity = 0;
		_state = OFF_STUN;
	}
}

bool tortoisePhase2::stun()
{
	_delayCount = (_delayCount + 1) % 120;
	if (_delayCount != 0) return false;

	return true;
}

void tortoisePhase2::moveOff()
{
	_angle = utl::getAngle(_x, _y, _shield->getX(), _shield->getY());
	//if (_offSpeed < PHASE1_CONST::MAX_OFF_SPEED) _offSpeed += 0.1f; // 2페이즈는 가속없다.
	
	_x += _offSpeed * cosf(_angle);
	_y += _offSpeed * -sinf(_angle);

	if (_shield->getX() > _x)
	{
		_dir = RIGHT;
	}
	else if (_shield->getX() < _x)
	{
		_dir = LEFT;
	}
	//실드의 폭보다 가까워지면 실드를 줍줍
	if (utl::getDistance(_x, _y, _shield->getX(), _shield->getY()) < _shield->getWidth() && _state == OFF_FLY)
	{
		_shield->setIsActive(true);
		_state = TAKE_SHIELD;
	}

	//OFF_TURN을 넣기 위해서는 턴하는 조건을 변경해야함
}

void tortoisePhase2::takeShield()
{
	//실드 주울때 방향에 따라 앵글값 다시지정
	//가속도 초기화
	if (_dir == RIGHT)
	{
		_angle = 0;
		//_offSpeed = PHASE2_CONST::DEFAULT_OFF_SPEED;
	}
	else if (_dir == LEFT)
	{
		_angle = 180;
		//_offSpeed = PHASE2_CONST::DEFAULT_OFF_SPEED;
	}
	_state = FLY;
}

void tortoisePhase2::damaged(actor * e)
{
	//플레이어에 의해 공격당했을때 보스가 해야할 액션

	//TODO : 임시
	POINT t = { _ptMouse.x + CAM->getX() , _ptMouse.y + CAM->getY() }; //마우스위치를 LEVEL의 전역좌표로 바꾼다.

	if (_isActiveShield)
	{
		if (PtInRect(&_hitBox, t))
		{
			_state = OFF_SHIELD;
			_shield->setIsActive(false);
		}
	}
	else
	{
		//TODO : 실드가 없으면 데미지 받음
		//_hp -= e->getPower();
		_hp -= 100;
	}
}

void tortoisePhase2::Bfire(float angle)
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

void tortoisePhase2::Bmove()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].update();
		_vBullet[i].collide("보스방1픽셀");
	}
}

void tortoisePhase2::Brender()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].render();
	}

}

bool tortoisePhase2::frameMake(image * bmp, int & count, int & index, int frameY1, int frameY2, int cooltime, bool renderDir)
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

