#include "stdafx.h"
#include "tortoiseShield.h"
#include "player.h"

HRESULT tortoiseShield::init(float x, float y)
{
	_x = x;
	_y = y;
	_gravity = 0;
	_z = 10;

	_image = IGM->findImage("�ź���_����������");
	_isActive = false;
	_gravity = 0;
	_index = _count = 0;

	_maxWidth = 100;
	_maxHeight = 100;
	return S_OK;
}

void tortoiseShield::update()
{
	if (_isActive) 
	{
		move();
		_hitBox = RectMakeCenter(_x, _y, SHIELD_CONST::HITBOX_WIDTH, SHIELD_CONST::HITBOX_HEIGHT);
		//hitBox ���ż����� �浹�˻� �Ʒ��� ������ ���� �������� ������Ʈ x,y��ǥ�� �ٲپ��µ� hitBox�� ���� �����ӿ� ���� �浹�˻� �ϹǷ� ������ �ȴ�.
		collide();
		//���� ������ ������, �ƽ��������̸� ������ �ȵ���.
		if (_index != _image->getMaxFrameX())
		{
			_count++;
			if (_count % 7 == 0)
			{
				_index++;
			}
		}
	}

	if (!_isActive)
	{
		_hitBox = RectMakeCenter(_x, _y + 20, SHIELD_CONST::HITBOX_WIDTH, SHIELD_CONST::HITBOX_HEIGHT+80);
		_index = 0; //�ǵ尡 �޷������� �ε����� 0����
		_gravity = 0;
	}
}

void tortoiseShield::render()
{
	_image->frameRender(getMemDC(), _x - 95 - CAM->getX(), _y - 50 - CAM->getY(), _index, _dir);

	if (_isDebug)
	{
		//TextOut(getMemDC(), 140, 120, "����", strlen("����"));
		//_stprintf_s(_debug, "angle: %f, speed: %f, gravity: %f", _angle, _speed, _gravity);
		//TextOut(getMemDC(), 140, 140, _debug, strlen(_debug));
		//Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
	if (_state == HANG)
	{
		image *temp = IMAGEMANAGER->findImage("LIFT2");
		temp->frameRender(getMemDC(), temp->getX()- temp->getFrameWidth()/2- CAM->getX(), temp->getY() - temp->getFrameHeight()/2 - CAM->getY());
	}
}

void tortoiseShield::release()
{
}

void tortoiseShield::damaged(actor * e)
{
	_index = 0;
	//������ �ǵ带 ���������� ����
}

void tortoiseShield::move()
{
	_gravity += 0.05f;
	//if (310 < _x && _x < 1920 - 310)
	{
		_x += _speed * cos(_angle);
	}
	_y += _speed * -sin(_angle) + _gravity;
}

void tortoiseShield::collide()
{
	//_speed����ŭ ���ӿ�����Ʈ�� ĭĭ�� ��ġ����(�̵�)�ϱ� ������ _speed�� ����(���� �ִ�� �հ� ����)��ŭ �˻��ؾ���
	//����
	for (int i = _y - SHIELD_CONST::HITBOX_HEIGHT/2 + _speed; i >= SHIELD_CONST::HITBOX_HEIGHT / 2; i--)
	// ���ǵ常ŭ �˻��ؾ� �������� ����ϰ�, �����̴� �������� �˻��ؾ� ������ ������ ����.
	{
		//_y - HITBOX_HEIGHT/2�� HITBOX�� top��ǥ�� ������, hitbox�� ���� ����ؼ� �˻��ϸ� ���ʰ˻翡�� _y��ǥ�� �ٲ� 
		//����,������,�Ʒ����� ���� ������ _hitBox ��ǥ�� ������ �˻��ع����Ƿ� ������ �ȴ�.
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = i + SHIELD_CONST::HITBOX_HEIGHT/2;
			_speed = 0; //�ӵ��� ���а� �������� tortoise�� �Ѱ���
			break;
		}
	}

	//����
	for (int i = _x - SHIELD_CONST::HITBOX_WIDTH / 2 + _speed; i >= _x - SHIELD_CONST::HITBOX_WIDTH / 2; i--)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetRValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_x = i + SHIELD_CONST::HITBOX_WIDTH / 2;
			_angle = 4.71; //270��
			if (_speed > 3) _speed -= 3;
			break;
		}
	}

	//������
	for(int i = _x + SHIELD_CONST::HITBOX_WIDTH / 2 -_speed; i <= _x + SHIELD_CONST::HITBOX_WIDTH / 2; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetRValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_x = i - SHIELD_CONST::HITBOX_WIDTH / 2;
			_angle = 4.71;
			if (_speed > 3) _speed -= 3;
			break;
		}
	}

	//�Ʒ���
	for (int i = _y + SHIELD_CONST::HITBOX_HEIGHT / 2 - _speed; i <= _y + SHIELD_CONST::HITBOX_HEIGHT / 2; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = i - SHIELD_CONST::HITBOX_HEIGHT / 2;
			if(_state != HANG && _speed !=0) CAM->setShakeInfo(10, 20); // ������ ��׶��� �̹����� getSX(), getSY()�� �ְ� setShakeInfo�� ��Ŷ ����
			_speed = 0;
			_gravity = 0;
			break;
		}
	}
}

void tortoiseShield::lifted(player * _player)
{
	if (_isActive)
	{
		_state = HANG;
		_gravity = 0;
		_index = 3;
		_x = _player->getX();
		_y = _player->getY() + _maxHeight / 2;
		//_hitBox = RectMakeCenter(_x, _y + 20, SHIELD_CONST::HITBOX_WIDTH, SHIELD_CONST::HITBOX_HEIGHT);
	}
}

void tortoiseShield::throwed(float speed, float angle)
{
	actor::throwed(speed, angle);
	_state = IDLE;
}

