#include "stdafx.h"
#include "torque.h"
#include "player.h"

HRESULT torque::init(float x, float y, int dir)
{
	//TODO : ∑πµ,¡∂¡ÿ ªÛ≈¬ø°º≠ ∫“∏¥ ∂ÁøˆµŒ±‚
	//CHECK √ﬂ∞° ¿ÃπÃ¡ˆ
	IGM->addFrameImage("≈‰≈©_±‚∫ª", "Texture/Enemies/Torque/idle_882x258_6x2.bmp", 882, 258, 6, 2);
	IGM->addFrameImage("≈‰≈©_∑πµ", "Texture/Enemies/Torque/ready_165x258_1x2.bmp", 165, 258, 1, 2);
	IGM->addFrameImage("≈‰≈©_∑πµº’", "Texture/Enemies/Torque/readyOver_165x258_1x2.bmp", 165, 258, 1, 2);
	IGM->addFrameImage("≈‰≈©_¡∂¡ÿ", "Texture/Enemies/Torque/aiming_390x282_2x2.bmp", 390, 282, 2, 2);
	IGM->addFrameImage("≈‰≈©_¡∂¡ÿº’", "Texture/Enemies/Torque/aimingOver_390x282_2x2.bmp", 390, 282, 2, 2);
	IGM->addFrameImage("≈‰≈©_ΩÓ±‚", "Texture/Enemies/Torque/shoot_1386x348_6x2.bmp", 1386, 348, 6, 2);
	IGM->addFrameImage("≈‰≈©_æ∆«ƒ", "Texture/Enemies/Torque/damaged_456x294_2x2.bmp", 456, 294, 2, 2);

	IGM->addFrameImage("¿œπ›∏˜_¡◊¿Ω", "Texture/Effect/enemyExplode_4200x340_10x1.bmp", 4200, 340, 10, 1);

	IGM->addFrameImage("≈‰≈©_∫“∏¥", "Texture/Enemies/Torque/bullet_69x136_1x2.bmp", 69, 136, 1, 2);
	IGM->addFrameImage("µπµ¢¿Ã_∆¯πﬂ¿Ã∆Â∆Æ", "Texture/Effect/stonExplode_1344x214_8x1.bmp", 1344, 214, 8, 1);
	EFFECTMANAGER->addEffect("µπµ¢¿Ã_∆¯πﬂ", "µπµ¢¿Ã_∆¯πﬂ¿Ã∆Â∆Æ", 0.2, 7);
	
	enemy::init(x, y);

	_torqueImage[IDLE] = IMAGEMANAGER->findImage("≈‰≈©_±‚∫ª");
	_torqueImage[READY] = IMAGEMANAGER->findImage("≈‰≈©_∑πµ");
	_torqueImage[AIMING] = IMAGEMANAGER->findImage("≈‰≈©_¡∂¡ÿ");
	_torqueImage[SHOOT] = IMAGEMANAGER->findImage("≈‰≈©_ΩÓ±‚");
	_torqueImage[STUN] = IMAGEMANAGER->findImage("≈‰≈©_æ∆«ƒ");
	_torqueImage[DEAD] = IMAGEMANAGER->findImage("¿œπ›∏˜_¡◊¿Ω");

	_torqueHand[READY] = IMAGEMANAGER->findImage("≈‰≈©_∑πµº’");
	_torqueHand[AIMING] = IMAGEMANAGER->findImage("≈‰≈©_¡∂¡ÿº’");
	
	_count = _index = 0;

	_hitBox = RectMakeCenter(_x, _y, TORQUE_CONST::HITBOX_WIDTH, TORQUE_CONST::HITBOX_HEIGHT);
	_scanRc = RectMakeCenter(_x, _y, TORQUE_CONST::HITBOX_WIDTH * 6, TORQUE_CONST::HITBOX_HEIGHT * 6);

	_maxHp = 3;
	_hp = 3;

	_speed = 2;

	_readyCount = _aimingCount = _stunCount = _attackCount = 0;
	_isKnockBack = false;
	_knockBackDistance = 15;

	bullet blt;
	blt.init(20, 8, IGM->findImage("¥¯¿¸∏ ")->getWidth(),"≈‰≈©_∫“∏¥");
	blt.setPower(3);
	for (int i = 0; i < 1; i++)
	{
		_vBullet.push_back(blt);
	}

	return S_OK;
}

void torque::update()
{
	if (_hp <= 0)
	{
		_state = DEAD;
		_hp = 0;
	}

	_playerX = _player->getX();
	_playerY = _player->getY();

	_hitBox = RectMakeCenter(_x, _y, TORQUE_CONST::HITBOX_WIDTH, TORQUE_CONST::HITBOX_HEIGHT);
	_scanRc = RectMakeCenter(_x, _y, TORQUE_CONST::HITBOX_WIDTH * 10, TORQUE_CONST::HITBOX_HEIGHT * 10);
	
	bool aniDone;
	if (_state == DEAD) aniDone = frameMake(_torqueImage[_state], 14);
	else aniDone = frameMake(_torqueImage[_state],7);

	switch (_state)
	{
	case IDLE:
		search();
		turn();
		break;
	case READY:
		if (_readyCount >= 2)
		{
			_state = AIMING;
			_readyCount = 0;
		}
		if (aniDone) ++_readyCount;
		turn();
		break;
	case AIMING:
		if (_aimingCount >= 3)
		{
			_state = SHOOT;
			_aimingCount = 0;
		}
		if (aniDone) ++_aimingCount;
		turn();
		break;
	case SHOOT:
		if(_attackCount < 1) Bfire(utl::getAngle(_x, _y, _playerX, _playerY)*58.8);
		if (aniDone)
		{
			_state = IDLE;
			_attackCount = 0;
		}
		turn();
		break;
	case STUN:
		if (_stunCount >= 3)
		{
			_state = IDLE;
			_stunCount = 0;
		}
		if (aniDone) ++_stunCount;
		break;
	case DEAD:
		if (aniDone) _isActive = false;
		break;
	}
	if (_isKnockBack && _state == STUN)
	{
		knockBack();
	}

	Bmove();
	Bcollide();
	collide();
}

void torque::render()
{
	Brender();
	//¿ßƒ°∫∏¡§
	float tempX, tempY;
	if (_dir == RIGHT)
	{
		switch (_state)
		{
		case IDLE:
			tempX = _x - 75;
			tempY = _y - 60;
			break;
		case READY:
			tempX = _x - 85;
			tempY = _y - 60;
			break;
		case AIMING:
			tempX = _x - 105;
			tempY = _y - 75;
			break;
		case SHOOT:
			tempX = _x - 105;
			tempY = _y - 110;
			break;
		case STUN:
			tempX = _x - 108;
			tempY = _y - 70;
			break;
		case DEAD:
			tempX = _x - 200;
			tempY = _y - 190;
			break;
		}
	}
	else if (_dir == LEFT)
	{
		switch (_state)
		{
		case IDLE:
			tempX = _x - 65;
			tempY = _y - 60;
			break;
		case READY:
			tempX = _x - 65;
			tempY = _y - 60;
			break;
		case AIMING:
			tempX = _x - 75;
			tempY = _y - 75;
			break;
		case SHOOT:
			tempX = _x - 115;
			tempY = _y - 110;
			break;
		case STUN:
			tempX = _x - 108;
			tempY = _y - 70;
			break;
		case DEAD:
			tempX = _x - 200;
			tempY = _y - 190;
			break;
		}
	}

	_torqueImage[_state]->frameRender(getMemDC(), tempX- CAM->getX(), tempY - CAM->getY(), _index, _dir);

	//¡ÿ∫Òµø¿€ø°º≠ µπµ¢¿Ã∑ª¥ı
	if (_state == READY)
	{
		switch (_dir)
		{
		case RIGHT:
			IMAGEMANAGER->findImage("≈‰≈©_∫“∏¥")->frameRender(getMemDC(), tempX+25 - CAM->getX(), tempY+33 - CAM->getY(), 0, _dir);
			break;
		case LEFT:
			IMAGEMANAGER->findImage("≈‰≈©_∫“∏¥")->frameRender(getMemDC(), tempX+70 - CAM->getX(), tempY+33 - CAM->getY(), 0, _dir);
			break;
		}
		_torqueHand[_state]->frameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir);
	}
	if (_state == AIMING)
	{
		switch (_dir)
		{
		case RIGHT:
			IMAGEMANAGER->findImage("≈‰≈©_∫“∏¥")->frameRender(getMemDC(), tempX+10 - CAM->getX(), tempY+30 - CAM->getY(), 0, _dir);
			break;
		case LEFT:
			IMAGEMANAGER->findImage("≈‰≈©_∫“∏¥")->frameRender(getMemDC(), tempX+110 - CAM->getX(), tempY+30 - CAM->getY(), 0, _dir);
			break;
		}
		_torqueHand[_state]->frameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir);
	}

	if (_isDebug)
	{
		char debug[128];
		_stprintf_s(debug, "angle :%f", utl::getAngle(_x, _y, _playerX, _playerY)*58.8);
		//TextOut(getMemDC(), _x-CAM->getX(), _y-CAM->getY(), debug, strlen(debug));
		TextOut(getMemDC(), _x-CAM->getX(), _y-CAM->getY(), "X", strlen("X"));
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
}

void torque::release()
{

}


void torque::damaged(actor * e)
{
	_state = STUN;
	_hp -= e->getPower();
	if(_hp > 0 && e->getPower() == 0) _isKnockBack = true;
}
void torque::knockBack()
{
	_knockBackDistance -= 0.8f;

	if(_dir == LEFT) _x += _knockBackDistance;
	else _x -= _knockBackDistance;

	if (_knockBackDistance < 0)
	{
		_knockBackDistance = 15;
		_isKnockBack = false;
	}
}

void torque::collide()
{
	RECT tempRc;
	if (IntersectRect(&tempRc, &_player->getHitbox(), &_hitBox))
	{
		_player->damaged(this);
	}
}

void torque::search()
{
	RECT tempRc;
	if (IntersectRect(&tempRc, &_player->getHitbox(), &_scanRc))
	{
		_state = READY;
	}
}

void torque::turn()
{
	if (_playerX <= _x)
	{
		_dir = LEFT;
	}
	else
	{
		_dir = RIGHT;
	}
}

void torque::Bfire(float angle)
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		if (_vBullet[i].getIsActive()) continue;

		_vBullet[i].setIsActive(true);
		_vBullet[i].setFireCenter(_x, _y);
		_vBullet[i].setAngle(angle);
		++_attackCount;
		break;
	}
}

void torque::Bmove()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].update();
		if (_vBullet[i].getIsActive())
		{
			if(_vBullet[i].collide(_mapPixel))
			{
				//TODO : ∫“∏¥¿Ã∆Â∆Æ
				EFFECTMANAGER->play("µπµ¢¿Ã_∆¯πﬂ", _vBullet[i].getX()+50, _vBullet[i].getY()+50);
			}
		}
	}
}

void torque::Bcollide()
{
	RECT tempRc;
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		if (IntersectRect(&tempRc, &_player->getHitbox(), &_vBullet[i].getHitbox()) && _vBullet[i].getIsActive())
		{
			EFFECTMANAGER->play("µπµ¢¿Ã_∆¯πﬂ", _vBullet[i].getX() + 50, _vBullet[i].getY() + 50);
			_vBullet[i].setIsActive(false);
			_player->damaged(&_vBullet[i]);
			break;
		}
	}
}

void torque::Brender()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].render();
	}
}

bool torque::frameMake(image * bmp, int cooltime)
{
	++_count;
	if (_count % cooltime == 0)
	{
		++_index;
		if (_index > bmp->getMaxFrameX())
		{
			_index = 0;
			return true;
		}
	}
	return false;
}
