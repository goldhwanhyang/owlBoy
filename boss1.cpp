#include "stdafx.h"
#include "boss1.h"

HRESULT boss1::init(float x, float y)
{
	enemy::init(x, y);
	_count = _index = 0;
	_dir = RIGHT;
	_angle = 0;
	_speed = 2.0f;
	_speed2 = 3.4f;
	_boss1Image[TURN] = IGM->findImage("����1����");
	_boss1Image[WALK] = IGM->findImage("����1�ȱ�");
	_boss1Image[WALK_SHINING] = IGM->findImage("����1�ȱ��");
	_boss1Image[ATTACK] = IGM->findImage("����1���");
	//IGM->addImage("Null�Ҹ�", 10, 10);
	//_bullet = new bullet;

	_hitBox = RectMakeCenter(_x, _y+60, 230, 160);
	_isShield = true;
	_isAttack = false;
	_attackCount = 0;
	_state = WALK;

	return S_OK;
}

void boss1::update()
{
	enemy::update();
	//���ݰ� �ȱ� ���¸� �ٲپ��־����
	//���ݽ� �ѹ߽�� WALK���·� �ٽ� ATTACK���·� �����ư���
	bool aniDone;
	if (TURN != _state)
	{
		aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 7, _dir);
	}
	else
	{
		aniDone = frameMake(_boss1Image[_state], _count, _index, 1, 0, 12, _dir);
	}

	switch (_state)
	{
	case READY:
		break;
	case TURN:
		if (aniDone) turn();
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

	if (_state == WALK && aniDone)
	{
		//WALK���¿��� ATTACK���°� �ǰų� WALK_SHINING���°� �ȴ�
		if(RND->getInt(4) > 1) _state = ATTACK;
		else _state = WALK_SHINING;
	}
	else if (_state == WALK_SHINING && aniDone)
	{
		//WALK_SHINING���¿��� WALK���°� �ȴ�
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
		_boss1Image[_state]->frameRender(getMemDC(), _x - 135 - CAM->getX(), _y - 110 - CAM->getY(), _index, _dir);
	}
	else if (_dir == 1)
	{
		_boss1Image[_state]->frameRender(getMemDC(), _x - 120 - CAM->getX(), _y - 110 - CAM->getY(), _index, _dir);
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
	if (_dir == RIGHT)
	{
		if (_playerX > _hitBox.right)
		{
			//���������� �÷��̾ ������ getAngle(������ġ,�÷��̾���ġ)�� ���.
		}
		else
		{
			//���������� �÷��̾ ������ 50���� ���.
		}
	}
	else if (_dir == LEFT)
	{
		if (_playerX < _hitBox.left)
		{
			//���������� �÷��̾ ������ getAngle(������ġ,�÷��̾���ġ)�� ���.
		}
		else
		{
			//���������� �÷��̾ ������ 50���� ���.
		}
	}
	++_attackCount;
	//�Ҹ��� �� ī��Ʈ�� �ø��� 8�� ��� ���º�ȭ
	if (_attackCount > 8*10)
	{
		_state = WALK_SHINING;
		_attackCount = 0;
	}
}

void boss1::move()
{
	//���⿡���� �ٸ� ����
	//_x�����̸� �ϻ��°� �ɰ�, x��ǥ ������Ʈ�� �����->_angle�� 90����
	//�ϻ��°� ������ ���� �ٲٰ� �Ұ�
	if (_dir == RIGHT)
	{
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

	if (_isShield)
	{
		_x += _speed * cosf(_angle * 0.017);
	}
	else
	{
		_x += _speed2 * cosf(_angle * 0.017);
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
