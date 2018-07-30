#include "stdafx.h"
#include "tortoisePhase1.h"
#include "player.h"

//hit박스의 크기, 이미지 좌표 보정값등을 숫자가 아니라 WINSIZEX, _image->getWidth() 등을 사용해서 표현할수 있게 해주자.

HRESULT tortoisePhase1::init(float x, float y)
{	
	enemy::init(x, y);
	_count = _index = 0;
	_dir = LEFT;
	_angle = 180;
	_speed = 2.0f;
	_tortoiseImage[TURN] = IGM->findImage("거북이_페이즈1_꺽기");
	_tortoiseImage[WALK] = IGM->findImage("거북이_페이즈1_걷기");
	_tortoiseImage[WALK_SHINING] = IGM->findImage("거북이_페이즈1_걷기빛");
	_tortoiseImage[ATTACK] = IGM->findImage("거북이_페이즈1_쏘기");

	_tortoiseImage[TAKE_SHIELD] = IGM->findImage("거북이_페이즈1_방패줍기");

	_tortoiseImage[OFF_SHIELD] = IGM->findImage("거북이_페이즈1_방패떨어뜨리기");
	_tortoiseImage[OFF_STUN] = IGM->findImage("거북이_페이즈1_스턴");
	_tortoiseImage[OFF_TURN] = IGM->findImage("거북이_페이즈1_꺽기방패없이");
	_tortoiseImage[OFF_WALK] = IGM->findImage("거북이_페이즈1_걷기방패없이");

	_delayCount = 0;

	_hitBox = RectMakeCenter(_x, _y + 60, 230, 160);
	_isAttack = false;
	_attackCount = 0;
	_state = WALK;

	_offSpeed = PHASE1_CONST::DEFAULT_OFF_SPEED;

	bullet blt;
	blt.init(10, 5,IGM->findImage("보스방1")->getWidth(), "거북이_불릿");
	for (int i = 0; i < 7; ++i)
	{
		_vBullet.push_back(blt);
	}

	_maxHp = 100;
	_hp = 100;
	_hpBar = new progressBar;
	_hpBar->init("Texture/Loading/hpBarFront", "Texture/Loading/hpBarBack",WINSIZEX*0.6, WINSIZEY*0.1, 300, 30);
	_hpBar->setGauge(_hp, _maxHp);

	return S_OK;
}

void tortoisePhase1::update()
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
	if (TURN != _state || OFF_SHIELD != _state || OFF_TURN != _state || OFF_STUN != _state)	aniDone = frameMake(_tortoiseImage[_state], _count, _index, 1, 0, 7, _dir);
	else aniDone = frameMake(_tortoiseImage[_state], _count, _index, 1, 0, 12, _dir);

	switch (_state)
	{
	case READY:
		break;
	case TURN:					//꺽기
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
		if (aniDone) takeShield();
		break;
	case OFF_SHIELD:			//방패 떨어뜨리기
		shieldOff();
		break;
	case OFF_STUN:				//스턴
		if(stun()) _state = OFF_WALK;
		break;
	case OFF_TURN:				//방패없이 꺽기
		if (aniDone) turn();
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

	_hitBox = RectMakeCenter(_x, _y + 60, 220, 160);

	//실드를 활성하면 보스위치를 따라감
	if (_isActiveShield)
	{
		_shield->setX(_x);
		_shield->setY(_y);
	}

	//불릿무브
	Bmove();
	_hpBar->update();
	_hpBar->setGauge(_hp, _maxHp);
}

void tortoisePhase1::render()
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
	_hpBar->render();

	if (_isDebug)
	{
		//Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
		_stprintf_s(_debug, "angle: %f, hp: %d", _angle, _hp);
		TextOut(getMemDC(), 100, 100, _debug, strlen(_debug));
		TextOut(getMemDC(), _x, _y, "X", strlen("X"));
	}
}

void tortoisePhase1::release()
{
	_hpBar->release();
	SAFE_DELETE(_hpBar);
}

void tortoisePhase1::attack()
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

void tortoisePhase1::move()
{
	_x += _speed * cosf(_angle * 0.017);
	
	if (_dir == RIGHT)
	{
		//턴상태가 될때 index와 count를 0으로해야 Trun의 프레임이 작동한다.
		if (_x > IGM->findImage("보스방1")->getWidth() - 310)
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
}

void tortoisePhase1::turn()
{
	//방향바꿈
	if (_dir == RIGHT)
	{
		if (_isActiveShield) _state = WALK;
		//else
		//{
		//	_state = OFF_WALK;
		//	_shield.offSpeed = DEFAULT_OFF_SPEED; //턴할때 가속도 초기화
		//}
		_angle = 0;
	}
	else if (_dir == LEFT)
	{
		if (_isActiveShield) _state = WALK;
		//else
		//{
		//	_state = OFF_WALK;
		//	_shield.offSpeed = DEFAULT_OFF_SPEED; //턴할때 가속도 초기화
		//}
		_angle = 180;
	}
}

void tortoisePhase1::collide()
{
	//TODO : 플레이어랑 보스몸체랑 충돌했을때?
}

void tortoisePhase1::shieldOff()
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

bool tortoisePhase1::stun()
{
	_delayCount = (_delayCount + 1) % 120;
	if (_delayCount != 0) return false;

	 return true;
}

void tortoisePhase1::moveOff()
{
	_angle = utl::getAngle(_x, _y, _shield->getX(), _shield->getY());
	_angle *= 58.8; // degree값으로 변경하기 위해 58.8을 곱했다.
	if(_offSpeed < PHASE1_CONST::MAX_OFF_SPEED) _offSpeed += 0.1f; // 가속한다.
	_x += _offSpeed * cosf(_angle * 0.017); // cosf에 넣기위해 다시 radian으로 변경
	//앵글값 변경에 따른 문워크 방지
	// TODO : offTrun을 만들어서 문워크 방지해야함 아직도 버그가 있다.
	//if (-90 > _angle && _angle > -190 || _angle > 179)
	//{
	//	_dir = LEFT;
	//}
	//else if (_angle < 178 &&_angle > -28)
	//{
	//	_dir = RIGHT;
	//}
	if (_shield->getX() > _x)
	{
		_dir = RIGHT;
	}
	else if (_shield->getX() < _x)
	{
		_dir = LEFT;
	}
	//실드의 폭보다 가까워지면 실드를 줍줍
	if (utl::getDistance(_x,_y, _shield->getX(),_shield->getY()) < _shield->getWidth() && _state == OFF_WALK)
	{
		_shield->setIsActive(true);
		_state = TAKE_SHIELD;
	}

	//OFF_TURN을 넣기 위해서는 턴하는 조건을 변경해야함
}

void tortoisePhase1::takeShield()
{
	//실드 주울때 방향에 따라 앵글값 다시지정
	//가속도 초기화
	if (_dir == RIGHT)
	{
		_angle = 0;
		_offSpeed = PHASE1_CONST::DEFAULT_OFF_SPEED;
	}
	else if (_dir == LEFT)
	{
		_angle = 180;
		_offSpeed = PHASE1_CONST::DEFAULT_OFF_SPEED;
	}
	_state = WALK;
}

void tortoisePhase1::damaged(actor * e)
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

void tortoisePhase1::Bfire(float angle)
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

void tortoisePhase1::Bmove()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].update();
		_vBullet[i].collide("보스방1픽셀");
	}
}

void tortoisePhase1::Brender()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].render();
	}
	
}

bool tortoisePhase1::frameMake(image * bmp, int & count, int & index, int frameY1, int frameY2, int cooltime, bool renderDir)
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