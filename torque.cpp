#include "stdafx.h"
#include "torque.h"
#include "player.h"

HRESULT torque::init(float x, float y, int dir)
{	
	enemy::init(x, y);

	_torqueImage[IDLE] = IMAGEMANAGER->findImage("��ũ_�⺻");
	_torqueImage[READY] = IMAGEMANAGER->findImage("��ũ_����");
	_torqueImage[AIMING] = IMAGEMANAGER->findImage("��ũ_����");
	_torqueImage[SHOOT] = IMAGEMANAGER->findImage("��ũ_���");
	_torqueImage[STUN] = IMAGEMANAGER->findImage("��ũ_����");

	_torqueHand[READY] = IMAGEMANAGER->findImage("��ũ_�����");
	_torqueHand[AIMING] = IMAGEMANAGER->findImage("��ũ_���ؼ�");
	
	_count = _index = 0;

	_hitBox = RectMakeCenter(_x, _y, TORQUE_CONST::HITBOX_WIDTH, TORQUE_CONST::HITBOX_HEIGHT);
	_scanRc = RectMakeCenter(_x, _y, TORQUE_CONST::HITBOX_WIDTH * 6, TORQUE_CONST::HITBOX_HEIGHT * 6);

	_maxHp = 30;
	_hp = 30;

	_speed = 2;
	_power = 6;

	_readyCount = _aimingCount = _stunCount = _attackCount = 0;
	_isKnockBack = false;
	_knockBackSpeed = 15;

	bullet blt;
	blt.init(20, 8, 4, IGM->findImage("������")->getWidth(), "��ũ_�Ҹ�");
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
		EFFECTMANAGER->play("������", _x, _y);
		SOUNDMANAGER->play("������", _effectVolume);
		_isActive = false;
		_hp = 0;
	}

	_playerX = _player->getX();
	_playerY = _player->getY();

	_hitBox = RectMakeCenter(_x, _y, TORQUE_CONST::HITBOX_WIDTH, TORQUE_CONST::HITBOX_HEIGHT);
	_scanRc = RectMakeCenter(_x, _y, TORQUE_CONST::HITBOX_WIDTH * 10, TORQUE_CONST::HITBOX_HEIGHT * 10);
	
	bool aniDone = frameMake(_torqueImage[_state],7);

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
	//��ġ����
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
		}
	}

	_torqueImage[_state]->frameRender(getMemDC(), tempX- CAM->getX(), tempY - CAM->getY(), _index, _dir);

	//�غ��ۿ��� �����̷���
	if (_state == READY)
	{
		switch (_dir)
		{
		case RIGHT:
			IMAGEMANAGER->findImage("��ũ_�Ҹ�")->frameRender(getMemDC(), tempX+25 - CAM->getX(), tempY+33 - CAM->getY(), 0, _dir);
			break;
		case LEFT:
			IMAGEMANAGER->findImage("��ũ_�Ҹ�")->frameRender(getMemDC(), tempX+70 - CAM->getX(), tempY+33 - CAM->getY(), 0, _dir);
			break;
		}
		_torqueHand[_state]->frameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir);
	}
	if (_state == AIMING)
	{
		switch (_dir)
		{
		case RIGHT:
			IMAGEMANAGER->findImage("��ũ_�Ҹ�")->frameRender(getMemDC(), tempX+10 - CAM->getX(), tempY+30 - CAM->getY(), 0, _dir);
			break;
		case LEFT:
			IMAGEMANAGER->findImage("��ũ_�Ҹ�")->frameRender(getMemDC(), tempX+110 - CAM->getX(), tempY+30 - CAM->getY(), 0, _dir);
			break;
		}
		_torqueHand[_state]->frameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir);
	}

	if (_isDebug)
	{
		//char debug[128];
		//_stprintf_s(debug, "angle :%f", utl::getAngle(_x, _y, _playerX, _playerY)*58.8);
		//TextOut(getMemDC(), _x-CAM->getX(), _y-CAM->getY(), debug, strlen(debug));
		//TextOut(getMemDC(), _x-CAM->getX(), _y-CAM->getY(), "X", strlen("X"));
		//Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
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
	_knockBackSpeed -= 0.8f;

	if(_dir == LEFT) _x += _knockBackSpeed;
	else _x -= _knockBackSpeed;

	if (_knockBackSpeed < 0)
	{
		_knockBackSpeed = 15;
		_isKnockBack = false;
	}
}

void torque::collide()
{
	//����
	for (int i = _hitBox.left+_speed+_knockBackSpeed; i >= _hitBox.left; --i)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetRValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_x = i + TORQUE_CONST::HITBOX_WIDTH / 2;
			break;
		}
	}

	//������
	for (int i = _hitBox.right-_speed-_knockBackSpeed; i <= _hitBox.right; ++i)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetRValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_x = i - TORQUE_CONST::HITBOX_WIDTH / 2;
			break;
		}
	}

	//����
	for (int i = _hitBox.top + _speed; i >= _hitBox.top; --i)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = i + TORQUE_CONST::HITBOX_HEIGHT / 2;
			break;
		}
	}
	//�Ʒ���
	for (int i = _hitBox.bottom - _gravity; i <= _hitBox.bottom; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_gravity = 0;
			_y = i - TORQUE_CONST::HITBOX_HEIGHT / 2;
			//break;
		}
		else
		{
			_gravity += 0.5;
			_y = i - TORQUE_CONST::HITBOX_HEIGHT / 2 + _gravity;
		}
	}
	//TODO : �밢�� ��� �����⸦ ������

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
	SOUNDMANAGER->play("��������", _effectVolume);
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
				EFFECTMANAGER->play("������", _vBullet[i].getX()+50, _vBullet[i].getY()+50);
				SOUNDMANAGER->play("������", _effectVolume);
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
			EFFECTMANAGER->play("������", _vBullet[i].getX() + 50, _vBullet[i].getY() + 50);
			SOUNDMANAGER->play("������", _effectVolume);
			_player->damaged(&_vBullet[i]);
			_vBullet[i].setIsActive(false);
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
