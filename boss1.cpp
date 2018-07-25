#include "stdafx.h"
#include "boss1.h"

HRESULT boss1::init(float x, float y)
{
	enemy::init(x, y);
	_count = _index = 0;
	_dir = 0;
	_angle = 0;
	_speed = 2.0f;
	_speed2 = 3.4f;
	_boss1Image[WALK] = IGM->addFrameImage("����1�ȱ�", "Texture/Enemies/Boss1/bossWalk_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_boss1Image[WALK_SHINING] = IGM->addFrameImage("����1�ȱ��", "Texture/Enemies/Boss1/bossWalkShining_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_boss1Image[ATTACK] = IGM->addFrameImage("����1���", "Texture/Enemies/Boss1/bossShoot_1968x504_8x2.bmp", 1968, 504, 8, 2);
	IGM->addImage("Null�Ҹ�", 10, 10);
	_bullet = new bullet;
	_bullet->init("Null�Ҹ�",5,1920);

	_hitBox = RectMakeCenter(_x, _y+60, 230, 160);
	_isShield = true;
	_isAttack = false;
	_state = WALK;

	return S_OK;
}

void boss1::update()
{
	enemy::update();
	//���ݰ� �ȱ� ���¸� �ٲپ��־����
	//���ݽ� �ѹ߽�� WALK���·� �ٽ� ATTACK���·� �����ư���
	if (KEYMANAGER->isOnceKeyDown('G'))
	{
		if (_state == ATTACK)
		{
			_state = WALK;
		}
		else if (_state == WALK)
		{
			_state = ATTACK;
		}
	}

	switch (_state)
	{
	case READY:
		break;
	case WALK:
		move();
		break;
	case WALK_SHINING:
		move();
		break;
	case ATTACK:
		move();
		attack();
		break;
	case TAKE_SHIELD:
		break;
	case OFF_SHIELD:
		break;
	case OFF_WALK:
		break;
	}
	bool aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 7, _dir);

	if (_state == WALK && aniDone)
	{
		_state = WALK_SHINING;
	}
	else if (_state == WALK_SHINING && aniDone)
	{
		_state = WALK;
	}
	_hitBox = RectMakeCenter(_x, _y + 60, 220, 160);
	_shieldHitBox = RectMakeCenter(_x, _y+20, 220, 240);
}

void boss1::render()
{
	//��Ʈ�ڽ��� �ؽ��� ��ġ ���߱����� ���⿡ ���� �����ٲ�
	//������ frameMake�� ����ߴ��� ����Ÿ��� ����� ������ currentX , Y�� _index�� _dir�� ���� ��־���.  //_boss1Image[_state]->setFrameY(_dir);
	if (_dir == 0)
	{
		_boss1Image[_state]->frameRender(getMemDC(), _x - 135, _y - 110, _index, _dir);
	}
	else if (_dir == 1)
	{
		_boss1Image[_state]->frameRender(getMemDC(), _x - 120, _y - 110, _index, _dir);
	}

	if (_isDebug)
	{
		RectangleLine(getMemDC(), _hitBox);
		RectangleLine(getMemDC(), _shieldHitBox);
		wsprintf(_debug, "%d", _index);
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
	//WALK 8�� �����Ӱ� ATTACK�� 1�� �������� ����������
	//���������� �÷��̾ ������ 50���� ���.
	//���������� �÷��̾ ������ getAngle(������ġ,�÷��̾���ġ)�� ���.
}

void boss1::move()
{
	//����ٲ�
	if (_x > IGM->findI("������1")->getWidth() - 310 && _dir == 0)
	{
		_dir = 1;
		_angle = 180;
	}
	else if (_x < 310 && _dir == 1)
	{
		_dir = 0;
		_angle = 0;
	}

	if (_isShield)
	{
		if (_dir == 0) //���������� ����
		{
			_x += _speed * cosf(_angle * 0.017);
		}
		else if (_dir == 1) //�������� ����
		{
			_x += _speed * cosf(_angle * 0.017);
		}
	}
	else
	{
		if (_dir == 0) //���������� ����
		{
			_x += _speed2 * cosf(_angle * 0.017);
		}
		else if (_dir == 1) //�������� ����
		{
			_x += _speed2 * cosf(_angle * 0.017);
		}
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
