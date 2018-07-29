#include "stdafx.h"
#include "tortoise.h"
#include "player.h"

//hit�ڽ��� ũ��, �̹��� ��ǥ ���������� ���ڰ� �ƴ϶� WINSIZEX, _image->getWidth() ���� ����ؼ� ǥ���Ҽ� �ְ� ������.

HRESULT tortoise::init(float x, float y)
{
	//  ===============�߰��� �̹���=================
	IGM->addFrameImage("����1����������", "Texture/Enemies/Boss1/bossShieldDrop_3672x366_17_2.bmp", 3672, 366, 17, 2);
	IGM->addFrameImage("����1�Ҹ�", "Texture/Enemies/Boss1/bossBullet_525x138_7x2.bmp", 525, 138, 7, 2);

	IGM->addFrameImage("����1�ȱ���о���", "Texture/Enemies/Boss1/bossOffWalk_1968x504_8x2.bmp", 1968, 504, 8, 2);
	IGM->addFrameImage("����1���ж���߸���", "Texture/Enemies/Boss1/bossOffShield_522x504_2x2.bmp", 522, 504, 2, 2);
	IGM->addFrameImage("����1������о���", "Texture/Enemies/Boss1/bossOffTurn_492x504_2x2.bmp", 492, 504, 2, 2);
	IGM->addFrameImage("����1�����ݱ�", "Texture/Enemies/Boss1/bossTakeShield_1968x648_8x2.bmp", 1968, 648, 8, 2);

	//  ============================================
	
	enemy::init(x, y);
	_count = _index = 0;
	_dir = RIGHT;
	_angle = 0;
	_speed = 2.0f;
	_tortoiseImage[TURN] = IGM->findImage("����1����");
	_tortoiseImage[WALK] = IGM->findImage("����1�ȱ�");
	_tortoiseImage[WALK_SHINING] = IGM->findImage("����1�ȱ��");
	_tortoiseImage[ATTACK] = IGM->findImage("����1���");

	_tortoiseImage[TAKE_SHIELD] = IGM->findImage("����1�����ݱ�");

	_tortoiseImage[OFF_SHIELD] = IGM->findImage("����1���ж���߸���");
	_tortoiseImage[OFF_TURN] = IGM->findImage("����1������о���");
	_tortoiseImage[OFF_WALK] = IGM->findImage("����1�ȱ���о���");

	_shieldDropImage = IGM->findImage("����1����������");

	_delayCount = 0;

	_hitBox = RectMakeCenter(_x, _y + 60, 230, 160);
	_isAttack = false;
	_attackCount = 0;
	_state = WALK;

	ZeroMemory(&_shield, sizeof(Shield));
	_shield.on = true;
	_shield.offSpeed = 3.4f;

	bullet blt;
	blt.init(10, 5,IGM->findImage("������1")->getWidth(), "����1�Ҹ�");
	for (int i = 0; i < 7; ++i)
	{
		_vBullet.push_back(blt);
	}

	return S_OK;
}

void tortoise::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		damaged(_player);
	}

	_playerX = _player->getX();
	_playerY = _player->getY();
	//������ ������
	bool aniDone;
	if (TURN != _state || OFF_SHIELD != _state || OFF_TURN != _state)	aniDone = frameMake(_tortoiseImage[_state], _count, _index, 1, 0, 7, _dir);
	else aniDone = frameMake(_tortoiseImage[_state], _count, _index, 1, 0, 12, _dir);
	
	if (!_shield.on && _shield.index != _shieldDropImage->getMaxFrameX()) //�ƽ��������̸� ������ �ȵ���.
	{
		frameMake(_shieldDropImage, _shield.count, _shield.index, 1, 0, 7, _dir);
	}

	switch (_state)
	{
	case READY:
		break;
	case TURN:					//����
		if (aniDone) turn();	//TURN���°� �Ǹ� �̹����������� ���� �ٵ�����(aniDone==true) ����ٲ�
		break;
	case WALK:					//�ȱ�
		move();
		break;
	case WALK_SHINING:			//�ȴٰ� ����
		move();
		break;
	case ATTACK:				//�ѽ�
		move();
		attack();
		break;
	case TAKE_SHIELD:			//�����ݱ�
		if (aniDone) takeShield();
		break;
	case OFF_SHIELD:			//���� ����߸���
		shieldOff();
		break;
	case OFF_TURN:				//���о��� ����
		if (aniDone) turn();
	case OFF_WALK:				//���о��� �ȱ�
		moveOff();
		break;
	}

	//���º���
	if (_state == WALK && aniDone)
	{
		//WALK���¿��� ATTACK���°� �ǰų� WALK_SHINING���°� �ȴ�
		if (RND->getInt(4) > 1)
		{
			_state = ATTACK;
		}
		else _state = WALK_SHINING;
	}
	else if (_state == WALK_SHINING && aniDone)
	{
		//WALK_SHINING���¿��� WALK���°� �ȴ�
		_state = WALK;
	}

	if (_shield.on) //�ǵ尡 �޸��� ������ġ�� ����
	{
		_shield.x = _x;
		_shield.y = _y;
	}
	_hitBox = RectMakeCenter(_x, _y + 60, 220, 160);
	_shield.hitBox = RectMakeCenter(_shield.x, _shield.y + 20, 220, 240); //�ǵ尡 �������� �޷�����

	//�Ҹ�����
	Bmove();
}

void tortoise::render()
{
	//��Ʈ�ڽ��� �ؽ��� ��ġ ���߱����� ���⿡ ���� �����ٲ�
	//������ frameMake�� ����ߴ��� ����Ÿ��� ����� ������ currentX , Y�� _index�� _dir�� ���� ��־���.  //_boss1Image[_state]->setFrameY(_dir);

	if (_isDebug)
	{
		IMAGEMANAGER->findImage("������1�ȼ�")->render(getMemDC(), 0, 0, CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
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

	if (!_shield.on)
	{
		_shieldDropImage->frameRender(getMemDC(), _shield.x - CAM->getX(), _shield.y - CAM-> getY(), _shield.index, _dir);
	}
	
	//�Ҹ�����
	Brender();

	if (_isDebug)
	{
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
		Rectangle(getMemDC(), _shield.hitBox.left - CAM->getX(), _shield.hitBox.top - CAM->getY(), _shield.hitBox.right - CAM->getX(), _shield.hitBox.bottom - CAM->getY());
		_stprintf_s(_debug, "PX: %f, X: %f", _playerX , _x);
		TextOut(getMemDC(), 100, 100, _debug, strlen(_debug));
		TextOut(getMemDC(), _x, _y, "X", strlen("X"));
	}
}

void tortoise::release()
{
}

void tortoise::attack()
{
	//�����߿��� �̵��� ��ӵǸ� ������ �����Ӹ� �ٲ��.
	if (_dir == RIGHT)
	{
		if (_playerX > _x)
		{
			Bfire(utl::getAngle(_x, _y, _playerX, _playerY) * 58.8); //degree������ �ٽ� �ٲٱ� ���� 58.8�� ���ߴ�.
			//���������� �÷��̾ ������ getAngle(������ġ,�÷��̾���ġ)�� ���.
		}
		else
		{
			Bfire(35);
			//���������� �÷��̾ ������ 35���� ���.
		}
	}
	else if (_dir == LEFT)
	{
		if (_playerX < _x)
		{
			Bfire(utl::getAngle(_x, _y, _playerX, _playerY) * 58.8); //degree������ �ٽ� �ٲٱ� ���� 58.8�� ���ߴ�.
			//���������� �÷��̾ ������ getAngle(������ġ,�÷��̾���ġ)�� ���.
		}
		else
		{
			Bfire(145);
			//���������� �÷��̾ ������ 145���� ���.
		}
	}

	//�Ҹ��� �� ī��Ʈ�� �ø��� 8�� ��� ���º�ȭ
	if (_attackCount > 8)
	{
		_state = WALK_SHINING;
		_attackCount = 0;
	}
}

void tortoise::move()
{
	_x += _speed * cosf(_angle * 0.017);
	
	if (_dir == RIGHT)
	{
		//�ϻ��°� �ɶ� index�� count�� 0�����ؾ� Trun�� �������� �۵��Ѵ�.
		if (_x > IGM->findImage("������1")->getWidth() - 310)
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

void tortoise::shieldOff()
{
	//���Ŀ� actor::throwed(_speed, _angle)�� ����ؼ� �������� ��������.
	_gravity += 0.05;
	//�ӽ�==============
	if (310 < _x && _x < IMAGEMANAGER->findImage("������1")->getWidth() - 310)
	{
		_x += 1;
	}
	if (310 < _shield.x && _shield.x < IMAGEMANAGER->findImage("������1")->getWidth() - 310)
	{
		_shield.x -= 4;
	}
	//����Ȱ��
	_y += -2 + _gravity;
	_shield.y += -2 + _gravity;
	//==================

	//������ü �ȼ��浹
	COLORREF color = GetPixel(IMAGEMANAGER->findImage("������1�ȼ�")->getMemDC(), _x, _hitBox.bottom);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	if (!(r == 255 && g == 0 && b == 255))
	{
		_y = _hitBox.bottom - 160;
		_gravity = 0;
		_state = OFF_WALK;
		_shield.y = _shield.hitBox.bottom - 160;
	}
}

void tortoise::moveOff()
{
	_angle = utl::getAngle(_x, _y, _shield.x, _shield.y);
	_shield.offSpeed += 0.1f; //�����Ѵ�.
	_x += _shield.offSpeed * cosf(_angle);
	
	if (utl::getDistance(_x,_y, _shield.x,_shield.y) < _shieldDropImage->getFrameWidth())
	{
		_shield.on = true;
		_state = TAKE_SHIELD;
	}

	//���ϴ� ������ �����ؾ���
}

void tortoise::takeShield()
{
	//�ǵ� �ֿﶧ ���⿡ ���� �ޱ۰� �ٽ�����
	if (_dir == RIGHT)
	{
		_angle = 0;
	}
	else if (_dir == LEFT)
	{
		_angle = 180;
	}
	_state = WALK;
}

void tortoise::turn()
{
	//����ٲ�
	if (_dir == RIGHT)
	{
		if(_shield.on) _state = WALK;
		else
		{
			_state = OFF_WALK;
			_shield.offSpeed = 3.4f; //���Ҷ� ���ӵ� �ʱ�ȭ
		}
		_angle = 0;
	}
	else if (_dir == LEFT)
	{
		if (_shield.on) _state = WALK;
		else
		{
			_state = OFF_WALK;
			_shield.offSpeed = 3.4f; //���Ҷ� ���ӵ� �ʱ�ȭ
		}
		_angle = 180;
	}
}

void tortoise::damaged(actor * e)
{
	//�÷��̾ ���� ���ݴ������� ������ �ؾ��� �׼�

	//�ӽ�
	POINT t = { _ptMouse.x + CAM->getX() , _ptMouse.y + CAM->getY() }; //���콺��ġ�� LEVEL�� ������ǥ�� �ٲ۴�.
	
	if (_shield.on)
	{
		if (PtInRect(&_hitBox, t) && _x > t.x) //���ʿ��� �����ʿ� �ִ� ������ ����
		{
			_state = OFF_SHIELD;
			_shield.on = false;
		}
		if (PtInRect(&_hitBox, t) && _x < t.x) //�����ʿ��� ���ʿ� �ִ� ������ ����
		{
			_state = OFF_SHIELD;
			_shield.on = false;
		}
	}
	else
	{
		if (PtInRect(&_shield.hitBox, t))
		{
			_shield.x = t.x;
			_shield.y = t.y;
		}
	}
}

void tortoise::Bfire(float angle)
{
	//������ �ִ¹��
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

void tortoise::Bmove()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].update();
		_vBullet[i].collide("������1�ȼ�");
	}
}

void tortoise::Brender()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].render();
	}
}

bool tortoise::frameMake(image * bmp, int & count, int & index, int frameY1, int frameY2, int cooltime, bool renderDir)
{
	//if (renderDir)
	{
		//���ʹ������� ����
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