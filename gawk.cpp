#include "stdafx.h"
#include "gawk.h"
#include "player.h"

HRESULT gawk::init(float x, float y, int dir)
{
	//===============이미지 추가============
	IGM->addFrameImage("고크_기본", "Texture/Enemies/Gawk/idle_174x288_1x2.bmp", 174, 288, 1, 2);
	IGM->addFrameImage("고크_날기", "Texture/Enemies/Gawk/fly_1044x288_6x2.bmp", 1044, 288, 6, 2);
	IGM->addFrameImage("고크_아픔", "Texture/Enemies/Gawk/damaged_348x288_2x2.bmp", 348, 288, 2, 2);
	//=====================================
	enemy::init(x, y);
	_angle = 0.0f;
	_speed = 2.0f;
	_dir = dir;

	_gawkImage[IDLE] = IGM->findImage("고크_기본");
	_gawkImage[FLY] = IGM->findImage("고크_날기");
	_gawkImage[DAMAGED] = IGM->findImage("고크_아픔");

	_hitBox = RectMakeCenter(_x, _y, GAWK_CONST::HITBOX_WIDTH, GAWK_CONST::HITBOX_HEIGHT);
	_scanRc = RectMakeCenter(_x, _y, GAWK_CONST::HITBOX_WIDTH * 2, GAWK_CONST::HITBOX_HEIGHT * 2);

	_state = IDLE;
	_count = _index = 0;

	_delayCount = 0;
	_shakeAngle = 0;

	_hp = 3;
	_maxHp = 3;

	return S_OK;
}

void gawk::update()
{
	//TODO : 임시
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		damaged(_player);
	}

	//switch enum 상태처리
	_playerX = _player->getX();
	_playerY = _player->getY();

	_hitBox = RectMakeCenter(_x, _y, GAWK_CONST::HITBOX_WIDTH, GAWK_CONST::HITBOX_HEIGHT);
	_scanRc = RectMakeCenter(_x, _y, GAWK_CONST::HITBOX_WIDTH * 10, GAWK_CONST::HITBOX_HEIGHT * 10);
	
	bool aniDone = frameMake(_gawkImage[_state]);
	if (_state == FLY && aniDone)
	{
		_state = IDLE;
	}

	switch (_state)
	{
	case IDLE:
		search();
		break;
	case FLY:				//TODO : 움직임 이상함, 중력도 너무 큼
		move();
		break;
	case FALL:
		break;
	case DAMAGED:
		break;
	}
	turn();

	//점프하는 느낌으로 fly프레임 돌리고
	//보통때는 마지막 프레임으로 고정
}

void gawk::render()
{
	//위치보정
	float tempX,tempY;
	if (_dir == RIGHT)
	{
		switch (_state)
		{
		case IDLE:
			tempX = _x - 105;
			tempY = _y - 90;
			break;
		case FLY:
			tempX = _x - 90;
			tempY = _y - 90;
			break;
		case DAMAGED:
			tempX = _x - 90;
			tempY = _y - 90;
			break;
		}		
	}
	else if (_dir == LEFT)
	{
		switch (_state)
		{
		case IDLE:
			tempX = _x - 70;
			tempY = _y - 90;
			break;
		case FLY:
			tempX = _x - 80;
			tempY = _y - 90;
			break;
		case DAMAGED:
			tempX = _x - 80;
			tempY = _y - 90;
			break;
		}
	}
	//렌더
	_gawkImage[_state]->frameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir);
	_gawkImage[_state]->frameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir);

	if (_isDebug)
	{
		//Rectangle(getMemDC(), _scanRc.left - CAM->getX(), _scanRc.top - CAM->getY(), _scanRc.right - CAM->getX(), _scanRc.bottom - CAM->getY());
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
		_stprintf_s(_debug, "%d", _state);
		TextOut(getMemDC(), 300, 300, _debug, strlen(_debug));
	}
}

void gawk::release()
{
}

void gawk::damaged(actor* e)
{
	int oldState = _state;
	_state = DAMAGED;
	//===데미지 받으면 흔들어줌====
	++_shakeAngle;
	_x += 2 * sinf(_shakeAngle);
	//===========================
	if (_shakeAngle > 6.28)		//쉐킷이 한바탕 끝나면
	{
		//_hp -= e->getPower();	//데미지받고
		_state = oldState;		//원래 상태로 돌려줌
		_shakeAngle = 0;
	}
}

void gawk::move()
{
	//딜레이
	_gravity += 0.1f;
	_delayCount = (_delayCount + 1) % 40;
	if (_delayCount == 0)
	{
		_gravity = 0;
	}
	if (_dir == RIGHT)
	{
		_x += _speed * cosf(0);
	}
	else if (_dir == LEFT)
	{
		_x += _speed * cosf(PI);
	}
	_y += _speed * -sinf(_angle) +_gravity;
	//방향에 따라 앵글값 getAngle로 뛴다(FLY)
	//FLY프레임 끝남(aniDone)
	//중력값 조금만 줘서 천천히 낙하(IDLE 고정)
	//반복
}

void gawk::search()
{
	_angle = utl::getAngle(_x, _y, _playerX, _playerY);
	POINT temp = { _playerX,_playerY };
	if (PtInRect(&_scanRc, temp))
	{
		_state = FLY;
	}

}

void gawk::turn()
{
	if (_playerX > _x)
	{
		_dir = RIGHT;
	}
	else if (_playerX < _x)
	{
		_dir = LEFT;
	}
}

bool gawk::frameMake(image * bmp)
{
	++_count;
	if (_count % 7 == 0)
	{
		++_index;
	}

	if (_index > bmp->getMaxFrameX())
	{
		_index = 0;
		return true;
	}
	return false;
}

