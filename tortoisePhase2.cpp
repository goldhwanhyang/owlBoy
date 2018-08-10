#include "stdafx.h"
#include "tortoisePhase2.h"
#include "player.h"

//hit�ڽ��� ũ��, �̹��� ��ǥ ���������� ���ڰ� �ƴ϶� WINSIZEX, _image->getWidth() ���� ����ؼ� ǥ���Ҽ� �ְ� ������.

HRESULT tortoisePhase2::init(float x, float y, int dir)
{
	//CHECK �߰��̹���
	IGM->addFrameImage("�ź���_����_������2_�غ�", "Texture/Enemies/Boss1/damaged/phase2Ready_2496x684_8x2.bmp", 2496, 684, 8, 2);
	IGM->addFrameImage("�ź���_����_������2_����", "Texture/Enemies/Boss1/damaged/phase2Fly_2352x594_8x2.bmp", 2352, 594, 8, 2);
	IGM->addFrameImage("�ź���_����_������2_���", "Texture/Enemies/Boss1/damaged/phase2Shoot_2352x594_8x2.bmp", 2352, 594, 8, 2);
	IGM->addFrameImage("�ź���_����_������2_�����ݱ�", "Texture/Enemies/Boss1/damaged/phase2takeShield_2448x684_8x2.bmp", 2448, 284, 8, 2);
	IGM->addFrameImage("�ź���_����_������2_���ж���߸���", "Texture/Enemies/Boss1/damaged/phase2OffShield_636x420_2x2.bmp", 636, 420, 2, 2);
	IGM->addFrameImage("�ź���_����_������2_����", "Texture/Enemies/Boss1/damaged/phase2OffStun_636x420_2x2.bmp", 636, 420, 2, 2);
	IGM->addFrameImage("�ź���_����_������2_������о���", "Texture/Enemies/Boss1/damaged/phase2OffFly_2352x594_8x2.bmp", 2352, 594, 8, 2);

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

	_tortoiseDamagedImg[READY] = IGM->findImage("�ź���_����_������2_�غ�");
	_tortoiseDamagedImg[FLY] = IGM->findImage("�ź���_����_������2_����");
	_tortoiseDamagedImg[ATTACK] = IGM->findImage("�ź���_����_������2_���");
	_tortoiseDamagedImg[TAKE_SHIELD] = IGM->findImage("�ź���_����_������2_�����ݱ�");
	_tortoiseDamagedImg[OFF_SHIELD] = IGM->findImage("�ź���_����_������2_���ж���߸���");
	_tortoiseDamagedImg[OFF_STUN] = IGM->findImage("�ź���_����_������2_����");
	_tortoiseDamagedImg[OFF_FLY] = IGM->findImage("�ź���_����_������2_������о���");

	alphaInit();

	_delayCount = 0;

	_hitBox = RectMakeCenter(_x, _y + 60, PHASE2_CONST::HITBOX_WIDTH, PHASE2_CONST::HITBOX_HEIGHT);
	_isAttack = false;
	_attackCount = 0;
	_state = READY;

	_offSpeed = PHASE2_CONST::DEFAULT_OFF_SPEED;

	bullet blt;
	blt.init(10, 5, 5, IGM->findImage("������1")->getWidth(), "�ź���_�Ҹ�");
	for (int i = 0; i < 16; ++i)
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

	_playerX = _player->getX();
	_playerY = _player->getY();
	_isActiveShield = !_shield->getIsActive();

	stateUpdate();
	alphaUpdate();

	_hitBox = RectMakeCenter(_x, _y + 60, PHASE2_CONST::HITBOX_WIDTH, PHASE2_CONST::HITBOX_HEIGHT);

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

void tortoisePhase2::render()
{
	//��Ʈ�ڽ��� �ؽ��� ��ġ ���߱����� ���⿡ ���� �����ٲ�
	//������ frameMake�� ����ߴ��� ����Ÿ��� ����� ������ currentX , Y�� _index�� _dir�� ���� ��־���.  //_boss1Image[_state]->setFrameY(_dir);

	if (_isDebug)
	{
		IMAGEMANAGER->findImage("������1�ȼ�")->render(getMemDC(), 0, 0, CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
	}

	float tempX, tempY;
	switch (_state)
	{
	case READY:
	case FLY:
	case ATTACK:
	case OFF_SHIELD:
	case OFF_STUN:
	case OFF_FLY:
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
	else if (_isAlpha && _isActiveShield) _tortoiseDamagedImg[_state]->alphaFrameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir, _damAlphaBlue);

	//�Ҹ�����
	Brender();

	if (_isDebug)
	{
		//Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
		//_stprintf_s(_debug, "angle: %f, offSpeed: %f", _angle, _offSpeed);
		//TextOut(getMemDC(), 100, 100, _debug, strlen(_debug));
		//TextOut(getMemDC(), _x, _y, "X", strlen("X"));
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
	if (_attackCount >= 16)
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
	if ((_state != OFF_STUN) || (_state != OFF_SHIELD))
	{
		RECT tempRc;
		if (IntersectRect(&tempRc, &_player->getHitbox(), &_hitBox))
		{
			_player->damaged(this); //this�� �ڱ��ڽ��� ����Ű�� ������
			_x += 50 * cosf(utl::getAngle(_playerX, _playerY, _x, _y));
			_y += 50 * -sinf(utl::getAngle(_playerX, _playerY, _x, _y));
			_hitBox = RectMakeCenter(_x, _y + 60, PHASE2_CONST::HITBOX_WIDTH, PHASE2_CONST::HITBOX_HEIGHT);
		}
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
	if (_shield->getState() != HANG)
	{
		if (utl::getDistance(_x, _y, _shield->getX(), _shield->getY()) < _shield->getWidth()/2 && _state == OFF_FLY)
		{
			_shield->setIsActive(false);
			_state = TAKE_SHIELD;
		}
	}
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

	//POINT t = { _ptMouse.x + CAM->getX() , _ptMouse.y + CAM->getY() }; //���콺��ġ�� LEVEL�� ������ǥ�� �ٲ۴�.
																	     //�ǵ尡 �ְ� ���� ���������̶� �浹������
	_isAlpha = true;
	if (_state != READY)
	{
		if (e->getPower() == 100)
		{
			_hp -= 100; //�����߰����� �ǵ������� �ѹ濡
			_isActive = false;
		}
		else if (_isActiveShield &&	e->getPower() == 0)
		{
			_state = OFF_SHIELD;
			_shield->setIsActive(true);
		}
		else if (!_isActiveShield)
		{
			//�ǵ尡 ������ ������ ����
			_hp -= e->getPower();
			//_hp -= 100;
		}
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
				float tempAngle = _vBullet[i].getAngle() + 180;
				EFFECTMANAGER->play("�ź���_�Ҹ�����", _vBullet[i].getX(), _vBullet[i].getY(), tempAngle*0.017);
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
			float tempAngle = _vBullet[i].getAngle() + 180;
			EFFECTMANAGER->play("�ź���_�Ҹ�����", _vBullet[i].getX(), _vBullet[i].getY(), tempAngle*0.017);
			_player->damaged(&_vBullet[i]);
			_vBullet[i].setIsActive(false);
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

void tortoisePhase2::stateUpdate()
{
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
			SOUNDMANAGER->playBgm("����_������2", _soundVolume);
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
}

void tortoisePhase2::alphaInit()
{
	_damAlpha = 255;
	_damAlphaBlue = 150;
	_alphaDelay = 0;
	_alphaCount = 0;
	_isAlpha = false;
}

void tortoisePhase2::alphaUpdate()
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

