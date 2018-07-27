#include "stdafx.h"
#include "boss1.h"
#include "player.h"

HRESULT boss1::init(float x, float y)
{
	//  ===============�߰��� �̹���=================
	IGM->addFrameImage("����1����������", "Texture/Enemies/Boss1/bossShieldDrop_3672x366_17_2.bmp", 3672, 366, 17, 2);
	IGM->addFrameImage("����1�Ҹ�", "Texture/Enemies/Boss1/bossBullet_525x138_7x2.bmp", 525, 138, 7, 2);
	IGM->addFrameImage("����1�ȱ���о���", "Texture/Enemies/Boss1/bossOffWalk_1968x504_8x2.bmp", 1968, 504, 8, 2);
	IGM->addFrameImage("����1���ж���߸���", "Texture/Enemies/Boss1/bossOffShield_522x504_2x2.bmp", 522, 504, 2, 2);

	//  ============================================
	
	enemy::init(x, y);
	_count = _index = 0;
	_dir = RIGHT;
	_angle = 0;
	_speed = 2.0f;
	_boss1Image[TURN] = IGM->findImage("����1����");
	_boss1Image[WALK] = IGM->findImage("����1�ȱ�");
	_boss1Image[WALK_SHINING] = IGM->findImage("����1�ȱ��");
	_boss1Image[ATTACK] = IGM->findImage("����1���");
	_boss1Image[OFF_SHIELD] = IGM->findImage("����1���ж���߸���");
	_boss1Image[OFF_WALK] = IGM->findImage("����1�ȱ���о���");
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

void boss1::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		damaged(_player);
	}

	_playerX = _player->getX();
	_playerY = _player->getY();
	//������ ������
	bool aniDone;
	if (TURN != _state || OFF_SHIELD != _state)	aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 7, _dir);
	else aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 12, _dir);
	
	if (!_shield.on && _shield.index != _shieldDropImage->getMaxFrameX()) //�ƽ��������̸� ������ �ȵ���.
	{
		frameMake(_shieldDropImage, _shield.count, _shield.index, 1, 0, 7, _dir);
	}

	switch (_state)
	{
	case READY:
		break;
	case TURN:					//ȸ��
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
		break;
	case OFF_SHIELD:			//���� ����߸���
		shieldOff();
		break;
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
	_hitBox = RectMakeCenter(_x - CAM->getX(), _y + 60 - CAM->getY(), 220, 160);
	_shield.hitBox = RectMakeCenter(_shield.x - CAM->getX(), _shield.y + 20 - CAM->getY(), 220, 240); //�ǵ尡 �������� �޷�����

	//�Ҹ�����
	Bmove();
}

void boss1::render()
{
	//��Ʈ�ڽ��� �ؽ��� ��ġ ���߱����� ���⿡ ���� �����ٲ�
	//������ frameMake�� ����ߴ��� ����Ÿ��� ����� ������ currentX , Y�� _index�� _dir�� ���� ��־���.  //_boss1Image[_state]->setFrameY(_dir);
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
	
	//�Ҹ�����
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

void boss1::move()
{
	if (_dir == RIGHT)
	{
		//�ϻ��°� �ɶ� index�� count�� 0�����ؾ� Trun�� �������� �۵��Ѵ�.
		if (_x > IGM->findI("������1")->getWidth() - 310)
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
	//���Ŀ� actor::throwed(_speed, _angle)�� ����ؼ� �������� ��������.
	
	//�ӽ�
	_x += 1;
	_shield.x -= 1;

	COLORREF color = GetPixel(IGM->findImage("������1�ȼ�")->getMemDC(), _x, _hitBox.bottom);
}

void boss1::moveOff()
{
	_angle = utl::getAngle(_x, _y, _shield.x, _shield.y);
	_x += _shield.offSpeed * cosf(_angle);
}

void boss1::damaged(actor * e)
{
	//�÷��̾ ���� ���ݴ������� ������ �ؾ��� �׼�

	//�ӽ�
	if (PtInRect(&_hitBox, _ptMouse) && _x > _ptMouse.x) //���ʿ��� �����ʿ� �ִ� ������ ����
	{
		_state = OFF_SHIELD;
		_shield.on = false;
	}
	if (PtInRect(&_hitBox, _ptMouse) && _x < _ptMouse.x) //�����ʿ��� ���ʿ� �ִ� ������ ����
	{
		_state = OFF_SHIELD;
		_shield.on = false;
	}
}

void boss1::turn()
{
	//����ٲ�
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

void boss1::Bmove()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].update();
		_vBullet[i].collide("������1�ȼ�");
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