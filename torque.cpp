#include "stdafx.h"
#include "torque.h"
#include "player.h"

HRESULT torque::init(float x, float y, int dir)
{
	//TODO : ����,���� ���¿��� �Ҹ� ����α�
	//CHECK �߰� �̹���
	IGM->addFrameImage("��ũ_�⺻", "Texture/Enemies/Torque/idle_882x258_6x2.bmp", 882, 258, 6, 2);
	IGM->addFrameImage("��ũ_����", "Texture/Enemies/Torque/ready_165x258_1x2.bmp", 165, 258, 1, 2);
	IGM->addFrameImage("��ũ_�����", "Texture/Enemies/Torque/readyOver_165x258_1x2.bmp", 165, 258, 1, 2);
	IGM->addFrameImage("��ũ_����", "Texture/Enemies/Torque/aiming_390x282_2x2.bmp", 390, 282, 2, 2);
	IGM->addFrameImage("��ũ_���ؼ�", "Texture/Enemies/Torque/aimingOver_390x282_2x2.bmp", 390, 282, 2, 2);
	IGM->addFrameImage("��ũ_���", "Texture/Enemies/Torque/shoot_1386x348_6x2.bmp", 1386, 348, 6, 2);
	IGM->addFrameImage("��ũ_����", "Texture/Enemies/Torque/damaged_456x294_2x2.bmp", 456, 294, 2, 2);

	IGM->addFrameImage("��ũ_�Ҹ�", "Texture/Enemies/Torque/bullet_69x136_1x2.bmp", 69, 136, 1, 2);
	
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

	_maxHp = 3;
	_hp = 3;

	_aimingCount = _stunCount = _attackCount = 0;
	bullet blt;
	blt.init(20, 8, IGM->findImage("������1")->getWidth(),"��ũ_�Ҹ�"); //TODO: ��Ÿ� ����� �����ϱ�
	blt.setPower(3);
	for (int i = 0; i < 1; i++)
	{
		_vBullet.push_back(blt);
	}

	return S_OK;
}

void torque::update()
{
	_playerX = _player->getX();
	_playerY = _player->getY();

	_hitBox = RectMakeCenter(_x, _y, TORQUE_CONST::HITBOX_WIDTH, TORQUE_CONST::HITBOX_HEIGHT);
	_scanRc = RectMakeCenter(_x, _y, TORQUE_CONST::HITBOX_WIDTH * 10, TORQUE_CONST::HITBOX_HEIGHT * 10);
	
	bool aniDone;
	aniDone = frameMake(_torqueImage[_state]);

	switch (_state)
	{
	case IDLE:
		search();
		turn();
		break;
	case READY:
		readyStone();
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
	//TODO : �÷��̾������ŭ
	//_hp -= e->getPower();
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

void torque::readyStone()
{
	//TODO : z������ ��(�Ҹ�)�� �׷��ְ� �� ���� ������� �׸���.
	//�׵ڿ� ����
	_state = AIMING;
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
				//TODO : �Ҹ�����Ʈ
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

bool torque::frameMake(image * bmp)
{
	++_count;
	if (_count % 12 == 0)
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
