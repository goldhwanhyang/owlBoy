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
	_speed = 4.0f;
	_dir = dir;

	_gawkImage[IDLE] = IGM->findImage("고크_기본");
	_gawkImage[FLY] = IGM->findImage("고크_날기");
	_gawkImage[DAMAGED] = IGM->findImage("고크_아픔");

	_hitBox = RectMakeCenter(_x, _y, GAWK_CONST::HITBOX_WIDTH, GAWK_CONST::HITBOX_HEIGHT);
	_scanRc = RectMakeCenter(_x, _y, GAWK_CONST::HITBOX_WIDTH * 2, GAWK_CONST::HITBOX_HEIGHT * 2);

	_state = FLY;
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
	
	bool aniDone;
	if (_isFall)
	{
		_index = _gawkImage[FLY]->getMaxFrameX();
	}
	else
	{
		aniDone = frameMake(_gawkImage[_state]);
	}

	switch (_state)
	{
	case IDLE:
		search();
		break;
	case FLY:
		move();
		break;
	case DAMAGED:
		break;
	}
	turn();
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
	int jumpDelay = 30;
	_isFall = false;

	if (_playerY + RND->getInt(40) > _y)
	{
		jumpDelay = 800;
		_isFall = true;
	}
	else if (_playerY + RND->getInt(40) < _y)
	{
		jumpDelay = 10;
	}
	//중력가속도
	_gravity += RND->getFromFloatTo(0.02f, 0.05f);
	//딜레이
	_delayCount = (_delayCount + 1) % jumpDelay;
	if (_delayCount == 0)
	{
		_gravity = 0;
	}
	if (_dir == RIGHT)
	{
		_x += (_speed + RND->getFloat(1.5f)) * cosf(0);
	}
	else if (_dir == LEFT)
	{
		_x += (_speed + RND->getFloat(1.5f)) * cosf(PI);
	}
	_y += _speed * -sinf(1.57) + _gravity;

	//플레이어가 높으면 더 빨리점프
	//플레이어가 낮으면 점프X, fly index를 getMaxFrameX로 고정
	//플레이어와 높이가 비슷하면 점프하며 다가옴

}

void gawk::search()
{
	//_angle = utl::getAngle(_x, _y, _playerX, _playerY);
	//POINT temp = { _playerX,_playerY };
	//if (PtInRect(&_scanRc, temp))
	//{
	//	_state = FLY;
	//}
}

void gawk::turn()
{
	if (_playerX >= _x)
	{
		_dir = RIGHT;
	}
	else
	{
		_dir = LEFT;
	}
}

bool gawk::frameMake(image * bmp)
{
	++_count;
	if (_count % RND->getFromIntTo(5,12) == 0)
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

