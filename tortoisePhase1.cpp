#include "stdafx.h"
#include "tortoisePhase1.h"
#include "player.h"

//hit�ڽ��� ũ��, �̹��� ��ǥ ���������� ���ڰ� �ƴ϶� WINSIZEX, _image->getWidth() ���� ����ؼ� ǥ���Ҽ� �ְ� ������.

HRESULT tortoisePhase1::init(float x, float y, int dir)
{
	enemy::init(x, y, dir);
	_count = _index = 0;
	_dir = RIGHT;
	_angle = 0;
	_speed = 2.0f;
	_tortoiseImage[READY] = IMAGEMANAGER->findImage("�ź���_������1_����");
	_tortoiseImage[TURN] = IMAGEMANAGER->findImage("�ź���_������1_����");
	_tortoiseImage[WALK] = IMAGEMANAGER->findImage("�ź���_������1_�ȱ�");
	_tortoiseImage[WALK_SHINING] = IMAGEMANAGER->findImage("�ź���_������1_�ȱ��");
	_tortoiseImage[ATTACK] = IMAGEMANAGER->findImage("�ź���_������1_���");

	_tortoiseImage[TAKE_SHIELD] = IMAGEMANAGER->findImage("�ź���_������1_�����ݱ�");

	_tortoiseImage[OFF_SHIELD] = IMAGEMANAGER->findImage("�ź���_������1_���ж���߸���");
	_tortoiseImage[OFF_STUN] = IMAGEMANAGER->findImage("�ź���_������1_����");
	_tortoiseImage[OFF_TURN] = IMAGEMANAGER->findImage("�ź���_������1_������о���");
	_tortoiseImage[OFF_WALK] = IMAGEMANAGER->findImage("�ź���_������1_�ȱ���о���");

	//���ع���
	_tortoiseDamagedImg[READY] = _tortoiseImage[READY];
	_tortoiseDamagedImg[TURN] = IMAGEMANAGER->findImage("�ź���_����_������1_����");
	_tortoiseDamagedImg[WALK] = IMAGEMANAGER->findImage("�ź���_����_������1_�ȱ�");
	_tortoiseDamagedImg[WALK_SHINING] = IMAGEMANAGER->findImage("�ź���_����_������1_�ȱ��");
	_tortoiseDamagedImg[ATTACK] = IMAGEMANAGER->findImage("�ź���_����_������1_���");

	_tortoiseDamagedImg[TAKE_SHIELD] = IMAGEMANAGER->findImage("�ź���_����_������1_�����ݱ�");

	_tortoiseDamagedImg[OFF_SHIELD] = IMAGEMANAGER->findImage("�ź���_����_������1_���ж���߸���");
	_tortoiseDamagedImg[OFF_STUN] = IMAGEMANAGER->findImage("�ź���_����_������1_����");
	_tortoiseDamagedImg[OFF_TURN] = IMAGEMANAGER->findImage("�ź���_����_������1_������о���");
	_tortoiseDamagedImg[OFF_WALK] = IMAGEMANAGER->findImage("�ź���_����_������1_�ȱ���о���");

	alphaInit();

	_delayCount = 0;

	_hitBox = RectMakeCenter(_x, _y + 60, PHASE1_CONST::HITBOX_WIDTH, PHASE1_CONST::HITBOX_HEIGHT);
	_isAttack = false;
	_attackCount = 0;
	_attPerCount = 0;

	_state = READY;

	_offSpeed = PHASE1_CONST::DEFAULT_OFF_SPEED;

	bullet blt;
	blt.init(10, 5, 5, 1920, "�ź���_�Ҹ�");
	for (int i = 0; i < 16; ++i)
	{
		_vBullet.push_back(blt);
	}

	_maxHp = 100;
	_hp = 100;

	_power = 3;

	_isStandby = false;

	return S_OK;
}

void tortoisePhase1::update()
{
	_playerX = _player->getX();
	_playerY = _player->getY();
	_isActiveShield = !_shield->getIsActive();

	stateUpdate();
	alphaUpdate();

	_hitBox = RectMakeCenter(_x, _y + 60, PHASE1_CONST::HITBOX_WIDTH, PHASE1_CONST::HITBOX_HEIGHT);

	collide();

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

void tortoisePhase1::render()
{
	//��Ʈ�ڽ��� �ؽ��� ��ġ ���߱����� ���⿡ ���� �����ٲ�
	//������ frameMake�� ����ߴ��� ����Ÿ��� ����� ������ currentX , Y�� _index�� _dir�� ���� ��־���.  //_boss1Image[_state]->setFrameY(_dir);

	float tempX, tempY;
	switch (_state)
	{
	case READY:
		tempX = _x - 135;
		tempY = _y - 110;
		break;
	case TURN:
	case WALK:
	case WALK_SHINING:
	case ATTACK:
	case OFF_SHIELD:
	case OFF_STUN:
	case OFF_TURN:
	case OFF_WALK:
		if (_dir == RIGHT)
		{
			tempX = _x - 135;
			tempY = _y - 110;
		}
		else
		{
			tempX = _x - 120;
			tempY = _y - 110;
		}
		break;
	case TAKE_SHIELD:
		if (_dir == RIGHT)
		{
			tempX = _x - 135;
			tempY = _y - 180;
		}
		else
		{
			tempX = _x - 120;
			tempY = _y - 180;
		}
		break;
	}

	_tortoiseImage[_state]->frameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir);
	if (_isAlpha && !_isActiveShield) _tortoiseDamagedImg[_state]->alphaFrameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir, _damAlpha);
	else if(_isAlpha && _isActiveShield) _tortoiseDamagedImg[_state]->alphaFrameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir, _damAlphaBlue);
	
	//�Ҹ�����
	Brender();

	if (_isDebug)
	{
		//char debug[64];
		//Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
		//_stprintf_s(debug, "angle: %f, hp: %d", _angle, _hp);
		//TextOut(getMemDC(), 100, 100, debug, strlen(debug));
		//TextOut(getMemDC(), _x, _y, "X", strlen("X"));
	}
}

void tortoisePhase1::release()
{
}

void tortoisePhase1::attack()
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

	//�Ҹ��� �� ī��Ʈ�� �ø��� 8*2�� ��� ���º�ȭ
	if (_attackCount >= 16)
	{
		_state = WALK_SHINING;
		_attackCount = 0;
	}
}

void tortoisePhase1::move()
{
	_x += _speed * cosf(_angle * 0.017);
	
	if (_dir == RIGHT)
	{
		//�ϻ��°� �ɶ� index�� count�� 0�����ؾ� Trun�� �������� �۵��Ѵ�.
		if (_x > 1920 - 310)
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

void tortoisePhase1::turn()
{
	//����ٲ�
	if (_dir == RIGHT)
	{
		if (_isActiveShield) _state = WALK;
		//else
		//{
		//	_state = OFF_WALK;
		//	_shield.offSpeed = DEFAULT_OFF_SPEED; //���Ҷ� ���ӵ� �ʱ�ȭ
		//}
		_angle = 0;
		_attackCount = 0;
	}
	else if (_dir == LEFT)
	{
		if (_isActiveShield) _state = WALK;
		//else
		//{
		//	_state = OFF_WALK;
		//	_shield.offSpeed = DEFAULT_OFF_SPEED; //���Ҷ� ���ӵ� �ʱ�ȭ
		//}
		_angle = 180;
		_attackCount = 0;
	}
}

void tortoisePhase1::collide()
{
	//�÷��̾�� ������ü�� �浹������
	if ((_state != OFF_STUN) || (_state != OFF_SHIELD))
	{
		RECT tempRc;
		if (IntersectRect(&tempRc, &_player->getHitbox(), &_hitBox))
		{
			_player->damaged(this); //this�� �ڱ��ڽ��� ����Ű�� ������
		}
	}
}

void tortoisePhase1::shieldOff()
{
	_gravity += 0.05;
	float temp = utl::getAngle(_playerX, _playerY, _x, _y);
	if (301 < _x && _x < 1920 - 310)
	{
		_x += 3 * cosf(temp);
	}
	_y += 4 * -sinf(temp) + _gravity;

	_shield->throwed(8, PI-temp);

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

bool tortoisePhase1::stun()
{
	_delayCount = (_delayCount + 1) % 90;
	if (_delayCount != 0) return false;

	 return true;
}

void tortoisePhase1::moveOff()
{
	_angle = utl::getAngle(_x, _y, _shield->getX(), _shield->getY());
	_angle *= 58.8; // degree������ �����ϱ� ���� 58.8�� ���ߴ�.
	if(_offSpeed < PHASE1_CONST::MAX_OFF_SPEED) _offSpeed += 0.04f; // �����Ѵ�.
	_x += _offSpeed * cosf(_angle * 0.017); // cosf�� �ֱ����� �ٽ� radian���� ����
	//�ޱ۰� ���濡 ���� ����ũ ����
	if (_shield->getX() > _x)
	{
		_dir = RIGHT;
	}
	else if (_shield->getX() < _x)
	{
		_dir = LEFT;
	}
	//�ǵ��� ������ ��������� �ǵ带 ����
	if (_shield->getState() != HANG)
	{
		if (utl::getDistance(_x,_y, _shield->getX(),_shield->getY()) < _shield->getWidth()/2 && _state == OFF_WALK)
		{
			_shield->setIsActive(false);
			_state = TAKE_SHIELD;
		}
	}

	//OFF_TURN�� �ֱ� ���ؼ��� ���ϴ� ������ �����ؾ���
}

void tortoisePhase1::takeShield()
{
	//�ǵ������ ü����0�̵Ǹ� �ǵ带 �ݰ� ����� �Ѿ��
	if (_hp <= 0)
	{
		_isActive = false;
	}
	else
	{
		//�ǵ� �ֿﶧ ���⿡ ���� �ޱ۰� �ٽ�����
		//���ӵ� �ʱ�ȭ
		if (_dir == RIGHT)
		{
			_angle = 0;
			_offSpeed = PHASE1_CONST::DEFAULT_OFF_SPEED;
		}
		else if (_dir == LEFT)
		{
			_angle = 180;
			_offSpeed = PHASE1_CONST::DEFAULT_OFF_SPEED;
		}
		_state = WALK;
	}
}

void tortoisePhase1::damaged(actor * e)
{
	if (_state != READY)
	{
		//CHECK ���ͽ��� ���ݰ� �Ե��� ������ �����ϴ� ��� -> �������� üũ
		//e->getPower() ���� ���� �׼� ����
		//0�̸� ���ͽ� ȸ������ �������� �Ҹ�
		//�÷��̾ ���� ���ݴ������� ������ �ؾ��� �׼�
		_isAlpha = true;
		//�ǵ尡 �ְ� ���� ���������̶� �浹������
		if (e->getPower() == 100)
		{
			_hp -= 100; //�����߰����� �ǵ������� �ѹ濡
			_isActive = false; //�ǵ带 �ݴµ��۾��� ������ �ѱ�
		}
		else if (_isActiveShield && e->getPower() == 0)
		{
			_state = OFF_SHIELD;
			_shield->setIsActive(true);
		}
		else if (!_isActiveShield)
		{
			//TODO : �ǵ������ ȸ������ ������ �ڷ� �������� ���ϵ�, ������ ������ �ȼ��浹 ��� �۵�����
			//if (e->getPower() == 0)
			//{
			//	_gravity += 0.05;
			//	float temp = utl::getAngle(_playerX, _playerY, _x, _y);
			//	if (301 < _x && _x < 1920 - 310)
			//	{
			//		_x += 3 * cosf(temp);
			//	}
			//	_y += 4 * -sinf(temp) + _gravity;

			//	//������ü �ȼ��浹
			//	COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
			//	int r = GetRValue(color);
			//	int g = GetGValue(color);
			//	int b = GetBValue(color);

			//	if (!(r == 255 && g == 0 && b == 255))
			//	{
			//		_y = _hitBox.bottom - 160;
			//		_gravity = 0;
			//		_state = OFF_STUN;
			//	}
			//}
			_hp -= e->getPower();
		}
	}
}

void tortoisePhase1::Bfire(float angle)
{
	//������ �ִ¹��
	_delayCount = (_delayCount + 1) % 30;
	if (_delayCount != 0) return;
	//=============

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < _vBullet.size()/2; ++j)
		{
			if (_vBullet[i*_vBullet.size()/2 + j].getIsActive()) continue;

			_vBullet[i*_vBullet.size()/2 + j].setIsActive(true);
			if(i == 0) _vBullet[i*_vBullet.size()/2 + j].setFireCenter(_x, _y - 45);
			else
			{
				if(_dir == LEFT)
					_vBullet[i*_vBullet.size() / 2 + j].setFireCenter(_x - 70, _y - 45);
				else
					_vBullet[i*_vBullet.size() / 2 + j].setFireCenter(_x + 70, _y - 45);
			}
			_vBullet[i*_vBullet.size()/2 + j].setAngle(angle);
			_vBullet[i*_vBullet.size()/2 + j].setDir(_dir);
			++_attackCount;
			break;
		}
	}
}

void tortoisePhase1::Bmove()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].update();
		if (_vBullet[i].getIsActive()) // isActive���ǰ˻� ���� �浹�˻��ؼ� ��� ������ �浹�ϴ� ���°� ���־���.
		{
			if (_vBullet[i].collide(_mapPixel))
			{
				float tempAngle = _vBullet[i].getAngle() + 180;
				EFFECTMANAGER->play("�ź���_�Ҹ�����", _vBullet[i].getX(), _vBullet[i].getY(), tempAngle*0.017);
			}
		}
	}
}

void tortoisePhase1::Bcollide()
{
	RECT tempRc;
	//�÷��̾� ��ü�� �浹�������� ������ �־���Ѵ�.
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		if (IntersectRect(&tempRc, &_player->getHitbox(), &_vBullet[i].getHitbox()) && _vBullet[i].getIsActive())
		{
			float tempAngle = _vBullet[i].getAngle() + 180;
			EFFECTMANAGER->play("�ź���_�Ҹ�����", _vBullet[i].getX(), _vBullet[i].getY(), tempAngle*0.017);
			_player->damaged(&_vBullet[i]);
			_vBullet[i].setIsActive(false);
			break;
		}
	}
}

void tortoisePhase1::Brender()
{
	for (int i = 0; i < _vBullet.size(); ++i)
	{
		_vBullet[i].render(true);
	}
}

bool tortoisePhase1::attPercent(int num, int* count)
{
	int seed = num / 3;
	if (RND->getInt(100) < seed * (*count))
	{
		*count = 0;
		return true;
	}
	else
	{
		++*count;
		return false;
	}
}

void tortoisePhase1::stateUpdate()
{

	//������ ������
	bool aniDone = false;
	if (READY == _state)
	{
		if(_isStandby) aniDone = frameMake(_tortoiseImage[_state], _count, _index, 40);
		//TODO : �ð����� ���� ���⵵ ����
	}
	else if (TURN != _state || OFF_SHIELD != _state || OFF_TURN != _state || OFF_STUN != _state) aniDone = frameMake(_tortoiseImage[_state], _count, _index, 7);
	else aniDone = frameMake(_tortoiseImage[_state], _count, _index, 12);

	switch (_state)
	{
	case READY:
		if (aniDone) 
		{
			SOUNDMANAGER->playBgm("����_������1",_soundVolume);
			_state = WALK;
		}
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
	case OFF_STUN:				//����
		if (stun()) _state = OFF_WALK;
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
		if (attPercent(50, &_attPerCount))
		{
			_state = ATTACK;
		}
		else if (_state != ATTACK) _state = WALK_SHINING;
	}
	else if (_state == WALK_SHINING && aniDone)
	{
		//WALK_SHINING���¿��� WALK���°� �ȴ�
		_state = WALK;
	}
}

void tortoisePhase1::alphaInit()
{
	_damAlpha = 255;
	_damAlphaBlue = 150;
	_alphaDelay = 0;
	_alphaCount = 0;
	_isAlpha = false;
}

void tortoisePhase1::alphaUpdate()
{
	if (_isAlpha)
	{
		_alphaDelay = (_alphaDelay + 1) % 5;
		if (_alphaDelay == 0)
		{
			_damAlpha = -_damAlpha;
			_damAlphaBlue = -_damAlphaBlue;
			++_alphaCount;
			if (_alphaCount >= 2)
			{
				_isAlpha = false;
				_alphaCount = 0;
			}
		}
	}
}

bool tortoisePhase1::frameMake(image * bmp, int & count, int & index, int cooltime)
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