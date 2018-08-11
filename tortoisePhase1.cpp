#include "stdafx.h"
#include "tortoisePhase1.h"
#include "player.h"

//hit박스의 크기, 이미지 좌표 보정값등을 숫자가 아니라 WINSIZEX, _image->getWidth() 등을 사용해서 표현할수 있게 해주자.

HRESULT tortoisePhase1::init(float x, float y, int dir)
{
	enemy::init(x, y, dir);
	_count = _index = 0;
	_dir = RIGHT;
	_angle = 0;
	_speed = 2.0f;
	_tortoiseImage[READY] = IMAGEMANAGER->findImage("거북이_페이즈1_레디");
	_tortoiseImage[TURN] = IMAGEMANAGER->findImage("거북이_페이즈1_꺽기");
	_tortoiseImage[WALK] = IMAGEMANAGER->findImage("거북이_페이즈1_걷기");
	_tortoiseImage[WALK_SHINING] = IMAGEMANAGER->findImage("거북이_페이즈1_걷기빛");
	_tortoiseImage[ATTACK] = IMAGEMANAGER->findImage("거북이_페이즈1_쏘기");

	_tortoiseImage[TAKE_SHIELD] = IMAGEMANAGER->findImage("거북이_페이즈1_방패줍기");

	_tortoiseImage[OFF_SHIELD] = IMAGEMANAGER->findImage("거북이_페이즈1_방패떨어뜨리기");
	_tortoiseImage[OFF_STUN] = IMAGEMANAGER->findImage("거북이_페이즈1_스턴");
	_tortoiseImage[OFF_TURN] = IMAGEMANAGER->findImage("거북이_페이즈1_꺽기방패없이");
	_tortoiseImage[OFF_WALK] = IMAGEMANAGER->findImage("거북이_페이즈1_걷기방패없이");

	//피해받음
	_tortoiseDamagedImg[READY] = _tortoiseImage[READY];
	_tortoiseDamagedImg[TURN] = IMAGEMANAGER->findImage("거북이_피해_페이즈1_꺽기");
	_tortoiseDamagedImg[WALK] = IMAGEMANAGER->findImage("거북이_피해_페이즈1_걷기");
	_tortoiseDamagedImg[WALK_SHINING] = IMAGEMANAGER->findImage("거북이_피해_페이즈1_걷기빛");
	_tortoiseDamagedImg[ATTACK] = IMAGEMANAGER->findImage("거북이_피해_페이즈1_쏘기");

	_tortoiseDamagedImg[TAKE_SHIELD] = IMAGEMANAGER->findImage("거북이_피해_페이즈1_방패줍기");

	_tortoiseDamagedImg[OFF_SHIELD] = IMAGEMANAGER->findImage("거북이_피해_페이즈1_방패떨어뜨리기");
	_tortoiseDamagedImg[OFF_STUN] = IMAGEMANAGER->findImage("거북이_피해_페이즈1_스턴");
	_tortoiseDamagedImg[OFF_TURN] = IMAGEMANAGER->findImage("거북이_피해_페이즈1_꺽기방패없이");
	_tortoiseDamagedImg[OFF_WALK] = IMAGEMANAGER->findImage("거북이_피해_페이즈1_걷기방패없이");

	alphaInit();

	_delayCount = 0;

	_hitBox = RectMakeCenter(_x, _y + 60, PHASE1_CONST::HITBOX_WIDTH, PHASE1_CONST::HITBOX_HEIGHT);
	_isAttack = false;
	_attackCount = 0;
	_attPerCount = 0;

	_state = READY;

	_offSpeed = PHASE1_CONST::DEFAULT_OFF_SPEED;

	bullet blt;
	blt.init(10, 5, 5, 1920, "거북이_불릿");
	for (int i = 0; i < 16; ++i)
	{
		_vBullet.push_back(blt);
	}

	_maxHp = 100;
	_hp = 100;

	_power = 3;

	_isStandby = false;

	return S_OK;
}

void tortoisePhase1::update()
{
	_playerX = _player->getX();
	_playerY = _player->getY();
	_isActiveShield = !_shield->getIsActive();

	stateUpdate();
	alphaUpdate();

	_hitBox = RectMakeCenter(_x, _y + 60, PHASE1_CONST::HITBOX_WIDTH, PHASE1_CONST::HITBOX_HEIGHT);

	collide();

	//실드를 활성하면 보스위치를 따라감
	if (_isActiveShield)
	{
		_shield->setX(_x);
		_shield->setY(_y);
	}

	//불릿무브
	Bmove();
	Bcollide();
}

void tortoisePhase1::render()
{
	//히트박스와 텍스쳐 위치 맞추기위해 방향에 따라 렌더바꿈
	//기존의 frameMake를 사용했더니 움찔거림이 생겼기 때문에 currentX , Y에 _index와 _dir을 직접 써넣었다.  //_boss1Image[_state]->setFrameY(_dir);

	float tempX, tempY;
	switch (_state)
	{
	case READY:
		tempX = _x - 135;
		tempY = _y - 110;
		break;
	case TURN:
	case WALK:
	case WALK_SHINING:
	case ATTACK:
	case OFF_SHIELD:
	case OFF_STUN:
	case OFF_TURN:
	case OFF_WALK:
		if (_dir == RIGHT)
		{
			tempX = _x - 135;
			tempY = _y - 110;
		}
		else
		{
			tempX = _x - 120;
			tempY = _y - 110;
		}
		break;
	case TAKE_SHIELD:
		if (_dir == RIGHT)
		{
			tempX = _x - 135;
			tempY = _y - 180;
		}
		else
		{
			tempX = _x - 120;
			tempY = _y - 180;
		}
		break;
	}

	_tortoiseImage[_state]->frameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir);
	if (_isAlpha && !_isActiveShield) _tortoiseDamagedImg[_state]->alphaFrameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir, _damAlpha);
	else if(_isAlpha && _isActiveShield) _tortoiseDamagedImg[_state]->alphaFrameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir, _damAlphaBlue);
	
	//불릿렌더
	Brender();

	if (_isDebug)
	{
		//char debug[64];
		//Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
		//_stprintf_s(debug, "angle: %f, hp: %d", _angle, _hp);
		//TextOut(getMemDC(), 100, 100, debug, strlen(debug));
		//TextOut(getMemDC(), _x, _y, "X", strlen("X"));
	}
}

void tortoisePhase1::release()
{
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

	//불릿을 쏠때 카운트를 올리고 8*2발 쏘면 상태변화
	if (_attackCount >= 16)
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
		if (_x > 1920 - 310)
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
		_attackCount = 0;
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
		_attackCount = 0;
	}
}

void tortoisePhase1::collide()
{
	//플레이어랑 보스몸체랑 충돌했을때
	if ((_state != OFF_STUN) || (_state != OFF_SHIELD))
	{
		RECT tempRc;
		if (IntersectRect(&tempRc, &_player->getHitbox(), &_hitBox))
		{
			_player->damaged(this); //this는 자기자신을 가리키는 포인터
		}
	}
}

void tortoisePhase1::shieldOff()
{
	_gravity += 0.05;
	float temp = utl::getAngle(_playerX, _playerY, _x, _y);
	if (301 < _x && _x < 1920 - 310)
	{
		_x += 3 * cosf(temp);
	}
	_y += 4 * -sinf(temp) + _gravity;

	_shield->throwed(8, PI-temp);

	//보스몸체 픽셀충돌
	COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
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
	_delayCount = (_delayCount + 1) % 90;
	if (_delayCount != 0) return false;

	 return true;
}

void tortoisePhase1::moveOff()
{
	_angle = utl::getAngle(_x, _y, _shield->getX(), _shield->getY());
	_angle *= 58.8; // degree값으로 변경하기 위해 58.8을 곱했다.
	if(_offSpeed < PHASE1_CONST::MAX_OFF_SPEED) _offSpeed += 0.04f; // 가속한다.
	_x += _offSpeed * cosf(_angle * 0.017); // cosf에 넣기위해 다시 radian으로 변경
	//앵글값 변경에 따른 문워크 방지
	if (_shield->getX() > _x)
	{
		_dir = RIGHT;
	}
	else if (_shield->getX() < _x)
	{
		_dir = LEFT;
	}
	//실드의 폭보다 가까워지면 실드를 줍줍
	if (_shield->getState() != HANG)
	{
		if (utl::getDistance(_x,_y, _shield->getX(),_shield->getY()) < _shield->getWidth()/2 && _state == OFF_WALK)
		{
			_shield->setIsActive(false);
			_state = TAKE_SHIELD;
		}
	}

	//OFF_TURN을 넣기 위해서는 턴하는 조건을 변경해야함
}

void tortoisePhase1::takeShield()
{
	//실드없을때 체력이0이되면 실드를 줍고 페이즈를 넘어간다
	if (_hp <= 0)
	{
		_isActive = false;
	}
	else
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
}

void tortoisePhase1::damaged(actor * e)
{
	if (_state != READY)
	{
		//CHECK 오터스의 공격과 게디의 공격을 판정하는 방법 -> 데미지로 체크
		//e->getPower() 값에 따라 액션 선택
		//0이면 오터스 회전공격 나머지면 불릿
		//플레이어에 의해 공격당했을때 보스가 해야할 액션
		_isAlpha = true;
		//실드가 있고 액터 공격판정이랑 충돌했으면
		if (e->getPower() == 100)
		{
			_hp -= 100; //무게추공격은 실드관계없이 한방에
			_isActive = false; //실드를 줍는동작없이 페이즈 넘김
		}
		else if (_isActiveShield && e->getPower() == 0)
		{
			_state = OFF_SHIELD;
			_shield->setIsActive(true);
		}
		else if (!_isActiveShield)
		{
			//TODO : 실드없을때 회전공격 받으면 뒤로 밀쳐지고 스턴됨, 보스가 보스방 픽셀충돌 없어서 작동안함
			//if (e->getPower() == 0)
			//{
			//	_gravity += 0.05;
			//	float temp = utl::getAngle(_playerX, _playerY, _x, _y);
			//	if (301 < _x && _x < 1920 - 310)
			//	{
			//		_x += 3 * cosf(temp);
			//	}
			//	_y += 4 * -sinf(temp) + _gravity;

			//	//보스몸체 픽셀충돌
			//	COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
			//	int r = GetRValue(color);
			//	int g = GetGValue(color);
			//	int b = GetBValue(color);

			//	if (!(r == 255 && g == 0 && b == 255))
			//	{
			//		_y = _hitBox.bottom - 160;
			//		_gravity = 0;
			//		_state = OFF_STUN;
			//	}
			//}
			_hp -= e->getPower();
		}
	}
}

void tortoisePhase1::Bfire(float angle)
{
	//딜레이 주는방법
	_delayCount = (_delayCount + 1) % 30;
	if (_delayCount != 0) return;
	//=============

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < _vBullet.size()/2; ++j)
		{
			if (_vBullet[i*_vBullet.size()/2 + j].getIsActive()) continue;

			_vBullet[i*_vBullet.size()/2 + j].setIsActive(true);
			if(i == 0) _vBullet[i*_vBullet.size()/2 + j].setFireCenter(_x, _y - 45);
			else
			{
				if(_dir == LEFT)
					_vBullet[i*_vBullet.size() / 2 + j].setFireCenter(_x - 70, _y - 45);
				else
					_vBullet[i*_vBullet.size() / 2 + j].setFireCenter(_x + 70, _y - 45);
			}
			_vBullet[i*_vBullet.size()/2 + j].setAngle(angle);
			_vBullet[i*_vBullet.size()/2 + j].setDir(_dir);
			++_attackCount;
			break;
		}
	}
}

void tortoisePhase1::Bmove()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].update();
		if (_vBullet[i].getIsActive()) // isActive조건검사 없이 충돌검사해서 계속 벽에다 충돌하는 상태가 되있었다.
		{
			if (_vBullet[i].collide(_mapPixel))
			{
				float tempAngle = _vBullet[i].getAngle() + 180;
				EFFECTMANAGER->play("거북이_불릿폭발", _vBullet[i].getX(), _vBullet[i].getY(), tempAngle*0.017);
			}
		}
	}
}

void tortoisePhase1::Bcollide()
{
	RECT tempRc;
	//플레이어 몸체랑 충돌했을때로 조건을 주어야한다.
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		if (IntersectRect(&tempRc, &_player->getHitbox(), &_vBullet[i].getHitbox()) && _vBullet[i].getIsActive())
		{
			float tempAngle = _vBullet[i].getAngle() + 180;
			EFFECTMANAGER->play("거북이_불릿폭발", _vBullet[i].getX(), _vBullet[i].getY(), tempAngle*0.017);
			_player->damaged(&_vBullet[i]);
			_vBullet[i].setIsActive(false);
			break;
		}
	}
}

void tortoisePhase1::Brender()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].render(true);
	}
}

bool tortoisePhase1::attPercent(int num, int* count)
{
	int seed = num / 3;
	if (RND->getInt(100) < seed * (*count))
	{
		*count = 0;
		return true;
	}
	else
	{
		++*count;
		return false;
	}
}

void tortoisePhase1::stateUpdate()
{

	//프레임 돌려줌
	bool aniDone = false;
	if (READY == _state)
	{
		if(_isStandby) aniDone = frameMake(_tortoiseImage[_state], _count, _index, 40);
		//TODO : 시간나면 석상 흔들기도 하자
	}
	else if (TURN != _state || OFF_SHIELD != _state || OFF_TURN != _state || OFF_STUN != _state) aniDone = frameMake(_tortoiseImage[_state], _count, _index, 7);
	else aniDone = frameMake(_tortoiseImage[_state], _count, _index, 12);

	switch (_state)
	{
	case READY:
		if (aniDone) 
		{
			SOUNDMANAGER->playBgm("보스_페이즈1",_soundVolume);
			_state = WALK;
		}
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
		if (stun()) _state = OFF_WALK;
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
		if (attPercent(50, &_attPerCount))
		{
			_state = ATTACK;
		}
		else if (_state != ATTACK) _state = WALK_SHINING;
	}
	else if (_state == WALK_SHINING && aniDone)
	{
		//WALK_SHINING상태에서 WALK상태가 된다
		_state = WALK;
	}
}

void tortoisePhase1::alphaInit()
{
	_damAlpha = 255;
	_damAlphaBlue = 150;
	_alphaDelay = 0;
	_alphaCount = 0;
	_isAlpha = false;
}

void tortoisePhase1::alphaUpdate()
{
	if (_isAlpha)
	{
		_alphaDelay = (_alphaDelay + 1) % 5;
		if (_alphaDelay == 0)
		{
			_damAlpha = -_damAlpha;
			_damAlphaBlue = -_damAlphaBlue;
			++_alphaCount;
			if (_alphaCount >= 2)
			{
				_isAlpha = false;
				_alphaCount = 0;
			}
		}
	}
}

bool tortoisePhase1::frameMake(image * bmp, int & count, int & index, int cooltime)
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