#include "stdafx.h"
#include "tortoisePhase2.h"
#include "player.h"

//hit�ڽ��� ũ��, �̹��� ��ǥ ���������� ���ڰ� �ƴ϶� WINSIZEX, _image->getWidth() ���� ����ؼ� ǥ���Ҽ� �ְ� ������.

HRESULT tortoisePhase2::init(float x, float y, int dir)
{
	enemy::init(x, y);
	_count = _index = 0;
	_dir = dir;
	if(dir == RIGHT) _angle = 0;
	else _angle = 180;
	_speed = 2.2f;
	_tortoiseImage[READY] = IGM->findImage("�ź���_������2_�غ�");
	_tortoiseImage[FLY] = IGM->findImage("�ź���_������2_����");
	_tortoiseImage[ATTACK] = IGM->findImage("�ź���_������2_���");
	_tortoiseImage[TAKE_SHIELD] = IGM->findImage("�ź���_������2_�����ݱ�");
	_tortoiseImage[OFF_SHIELD] = IGM->findImage("�ź���_������2_���ж���߸���");
	_tortoiseImage[OFF_STUN] = IGM->findImage("�ź���_������2_����");
	_tortoiseImage[OFF_FLY] = IGM->findImage("�ź���_������2_������о���");

	_delayCount = 0;

	_hitBox = RectMakeCenter(_x, _y + 60, PHASE2_CONST::HITBOX_WIDTH, PHASE2_CONST::HITBOX_HEIGHT);
	_isAttack = false;
	_attackCount = 0;
	_state = READY;

	_offSpeed = PHASE2_CONST::DEFAULT_OFF_SPEED;

	bullet blt;
	blt.init(10, 5, IGM->findImage("������1")->getWidth(), "�ź���_�Ҹ�");
	blt.setPower(3);
	for (int i = 0; i < 7; ++i)
	{
		_vBullet.push_back(blt);
	}

	_maxHp = 100;
	_hp = 100;

	_power = 3;

	_gravity = 0;

	return S_OK;
}

void tortoisePhase2::update()
{
	if (_hp <= 0)
	{
		_isActive = false;
	}
	//TODO : �ӽ�
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		damaged(_player);
	}

	_playerX = _player->getX();
	_playerY = _player->getY();
	_isActiveShield = _shield->getIsActive();

	//������ ������
	bool aniDone;
	if (READY != _state || OFF_SHIELD != _state || OFF_STUN != _state)	aniDone = frameMake(_tortoiseImage[_state], _count, _index, 7);
	else aniDone = frameMake(_tortoiseImage[_state], _count, _index, 12);

	switch (_state)
	{
	case READY:
	{
		if (aniDone)
		{
			_state = FLY;
			_gravity = 0;
		}
		else
		{
			_y -= 6 - _gravity;
			_gravity += 0.03;
		}
	}
		break;
	case FLY:						//����
		move();
		break;
	case ATTACK:					//�ѽ�
		move();
		attack();
		break;
	case TAKE_SHIELD:
		if (aniDone) takeShield();	//�����ݱ�
		break;
	case OFF_SHIELD:				//���� ����߸���
		shieldOff();
		break;
	case OFF_STUN:					//����
		if (stun()) _state = OFF_FLY;
		break;
	case OFF_FLY:					//���о��� ����
		moveOff();
		break;
	}

	//���º���
	if (_state == FLY && aniDone)
	{
		//WALK���¿��� ATTACK���°� �ǰų� WALK_SHINING���°� �ȴ�
		if (RND->getInt(4) > 1)
		{
			_state = ATTACK;
		}
		else _state = FLY;
	}

	_hitBox = RectMakeCenter(_x, _y + 60, PHASE2_CONST::HITBOX_WIDTH, PHASE2_CONST::HITBOX_HEIGHT);

	//�ǵ带 Ȱ���ϸ� ������ġ�� ����
	if (_isActiveShield)
	{
		_shield->setX(_x);
		_shield->setY(_y);
	}

	//�Ҹ�����
	Bmove();
	Bcollide();
}

void tortoisePhase2::render()
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

	//�Ҹ�����
	Brender();

	if (_isDebug)
	{
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
		_stprintf_s(_debug, "angle: %f, offSpeed: %f", _angle, _offSpeed);
		TextOut(getMemDC(), 100, 100, _debug, strlen(_debug));
		TextOut(getMemDC(), _x, _y, "X", strlen("X"));
	}
}

void tortoisePhase2::release()
{
}

void tortoisePhase2::attack()
{
	//�����߿��� �̵��� ��ӵǸ� ������ �����Ӹ� �ٲ��.
	//2��������� �ڷε� ���
	Bfire(utl::getAngle(_x, _y, _playerX, _playerY) * 58.8); //degree������ �ٽ� �ٲٱ� ���� 58.8�� ���ߴ�.

	//�Ҹ��� �� ī��Ʈ�� �ø��� 8�� ��� ���º�ȭ, count++�� Bfire�� ����
	if (_attackCount > 8)
	{
		_state = FLY;
		_attackCount = 0;
	}
}

void tortoisePhase2::move()
{
	_angle = utl::getAngle(_x, _y, _playerX, _playerY);
	_x += _speed * cosf(_angle);
	_y += _speed * -sinf(_angle);

	if (_playerX > _x)
	{
		_dir = RIGHT;
	}
	else if (_playerX < _x)
	{
		_dir = LEFT;
	}
}

void tortoisePhase2::collide()
{
	//�÷��̾�� ������ü�� �浹������
	RECT tempRc;
	if (IntersectRect(&tempRc, &_player->getHitbox(), &_hitBox))
	{
		_player->damaged(this); //this�� �ڱ��ڽ��� ����Ű�� ������
	}
}

void tortoisePhase2::shieldOff()
{
	_gravity += 0.05;
	float temp = utl::getAngle(_playerX, _playerY, _x, _y);
	if (301 < _x && _x < IMAGEMANAGER->findImage("������1")->getWidth() - 310)
	{
		_x += 3 * cosf(temp);
	}
	_y += -2 * -sinf(temp) + _gravity;

	_shield->throwed(6, temp);

	//������ü �ȼ��浹
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

bool tortoisePhase2::stun()
{
	_delayCount = (_delayCount + 1) % 90;
	if (_delayCount != 0) return false;

	return true;
}

void tortoisePhase2::moveOff()
{
	_angle = utl::getAngle(_x, _y, _shield->getX(), _shield->getY());
	//if (_offSpeed < PHASE1_CONST::MAX_OFF_SPEED) _offSpeed += 0.1f; // 2������� ���Ӿ���.
	
	_x += _offSpeed * cosf(_angle);
	_y += _offSpeed * -sinf(_angle);

	if (_shield->getX() > _x)
	{
		_dir = RIGHT;
	}
	else if (_shield->getX() < _x)
	{
		_dir = LEFT;
	}
	//�ǵ��� ������ ��������� �ǵ带 ����
	if (utl::getDistance(_x, _y, _shield->getX(), _shield->getY()) < _shield->getWidth() && _state == OFF_FLY)
	{
		_shield->setIsActive(true);
		_state = TAKE_SHIELD;
	}

	//OFF_TURN�� �ֱ� ���ؼ��� ���ϴ� ������ �����ؾ���
}

void tortoisePhase2::takeShield()
{
	//�ǵ� �ֿﶧ ���⿡ ���� �ޱ۰� �ٽ�����
	//���ӵ� �ʱ�ȭ
	if (_dir == RIGHT)
	{
		_angle = 0;
		//_offSpeed = PHASE2_CONST::DEFAULT_OFF_SPEED;
	}
	else if (_dir == LEFT)
	{
		_angle = 180;
		//_offSpeed = PHASE2_CONST::DEFAULT_OFF_SPEED;
	}
	_state = FLY;
}

void tortoisePhase2::damaged(actor * e)
{
	//e->getPower() ���� ���� �׼� ����
	//0�̸� ���ͽ� ȸ������ �������� �Ҹ�
	//�÷��̾ ���� ���ݴ������� ������ �ؾ��� �׼�

	//TODO : �ӽ�
	POINT t = { _ptMouse.x + CAM->getX() , _ptMouse.y + CAM->getY() }; //���콺��ġ�� LEVEL�� ������ǥ�� �ٲ۴�.
																	   //�ǵ尡 �ְ� ���� ���������̶� �浹������
	if (e->getPower() == 100)
	{
		_hp -= 100; //�����߰����� �ǵ������� �ѹ濡
	}
	else if (_isActiveShield) // &&	e->getPower() == 0)
	{
		if (PtInRect(&_hitBox, t)) //TODO : �ӽ�
		{
			_state = OFF_SHIELD;
			_shield->setIsActive(false);
		}
	}
	else if (!_isActiveShield)
	{
		//TODO : �ӽ�
		//�ǵ尡 ������ ������ ����
		//_hp -= e->getPower();
		_hp -= 100; //TODO : �ӽ�
	}
}

void tortoisePhase2::Bfire(float angle)
{
	//������ �ִ¹��
	_delayCount = (_delayCount + 1) % 30;
	if (_delayCount != 0) return;
	//=============

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < _vBullet.size() / 2; ++j)
		{
			if (_vBullet[i*_vBullet.size() / 2 + j].getIsActive()) continue;

			_vBullet[i*_vBullet.size() / 2 + j].setIsActive(true);
			if (i == 0) _vBullet[i*_vBullet.size() / 2 + j].setFireCenter(_x, _y - 45);
			else
			{
				if (_dir == LEFT)
					_vBullet[i*_vBullet.size() / 2 + j].setFireCenter(_x - 70, _y - 45);
				else
					_vBullet[i*_vBullet.size() / 2 + j].setFireCenter(_x + 70, _y - 45);
			}
			_vBullet[i*_vBullet.size() / 2 + j].setAngle(angle);
			_vBullet[i*_vBullet.size() / 2 + j].setDir(_dir);
			++_attackCount;
			break;
		}
	}
}

void tortoisePhase2::Bmove()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].update();
		if (_vBullet[i].getIsActive()) // isActive���ǰ˻� ���� �浹�˻��ؼ� ��� ������ �浹�ϴ� ���°� ���־���.
		{
			if (_vBullet[i].collide(_mapPixel))
			{
				EFFECTMANAGER->play("�ź���_�Ҹ�����", _vBullet[i].getX(), _vBullet[i].getY());
			}
		}
	}
}

void tortoisePhase2::Bcollide()
{
	RECT tempRc;
	//�÷��̾� ��ü�� �浹�������� ������ �־���Ѵ�.
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

void tortoisePhase2::Brender()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].render(true);
	}
}

bool tortoisePhase2::frameMake(image * bmp, int & count, int & index, int cooltime)
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

