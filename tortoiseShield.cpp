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
	_onGround = false;

	_maxWidth = 100;
	_maxHeight = 100;
	return S_OK;
}

void tortoiseShield::update()
{
	if (_isActive) 
	{
		move();
		collide();
		_hitBox = RectMakeCenter(_x, _y, 220, 160);
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
		_hitBox = RectMakeCenter(_x, _y + 20, SHIELD_CONST::HITBOX_WIDTH, SHIELD_CONST::HITBOX_HEIGHT);
		_index = 0; //�ǵ尡 �޷������� �ε����� 0����
		_gravity = 0;
		_onGround = false;
	}
}

void tortoiseShield::render()
{
	//if (_isActive)
	//{
		_image->frameRender(getMemDC(), _x - 95 - CAM->getX(), _y - 50 - CAM->getY(), _index, _dir);
	//}
	if (_isDebug)
	{
		TextOut(getMemDC(), 100, 120, "����", strlen("����"));
		_stprintf_s(_debug, "angle: %f, speed: %f, gravity: %f", _angle, _speed, _gravity);
		TextOut(getMemDC(), 100, 140, _debug, strlen(_debug));
		//Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
}

void tortoiseShield::release()
{
}

void tortoiseShield::damaged(actor * e)
{
	if (_onGround)
	{
		_index = 0;
	}
	//������ �ǵ带 ���������� ����
}

void tortoiseShield::move()
{
	if(!_onGround) _gravity += 0.05f;
	if (310 < _x && _x < IMAGEMANAGER->findImage("������1")->getWidth() - 310)
	{
		_x += _speed * cos(PI - _angle);
	}
	_y += _speed * -sin(_angle) + _gravity;
}

void tortoiseShield::collide()
{
	//TODO : ���Ŀ� �÷��̾ ��� �����ͱ��� �����ؾ��Ѵ�.

	//_speed����ŭ ���ӿ�����Ʈ�� ĭĭ�� ��ġ����(�̵�)�ϱ� ������ _speed�� ����(���� �ִ�� �հ� ����)��ŭ �˻��ؾ���
	//����
	for (int i = _hitBox.top + _speed; i >= _hitBox.top; i--) // ���ǵ常ŭ �˻��ؾ� �������� ����ϰ�, �����̴� �������� �˻��ؾ� ������ ������ ����.
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("������1�ȼ�")->getMemDC(), _x, i);
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
	for (int i = _hitBox.left + _speed; i >= _hitBox.left; i--)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("������1�ȼ�")->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetRValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_x = i + SHIELD_CONST::HITBOX_WIDTH / 2;
			_angle = 4.71; //270��
			break;
		}
	}

	//������
	for(int i =_hitBox.right-_speed; i <= _hitBox.right; i++)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("������1�ȼ�")->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetRValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_x = i - SHIELD_CONST::HITBOX_WIDTH / 2;
			_angle = 4.71;
			break;
		}
	}

	//�Ʒ���
	for (int i = _hitBox.bottom - _speed; i <= _hitBox.bottom; i++)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("������1�ȼ�")->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = i - SHIELD_CONST::HITBOX_HEIGHT / 2;
			_speed = 0;
			_gravity = 0;
			_onGround = true;
			CAM->setShakeInfo(10, 20); // ������ �̹����� getSX(), getSY()�� �ְ� setShakeInfo�� ��Ŷ ����
			break;
		}
	}
}

void tortoiseShield::lifted(player * _player)
{
	if (_isActive && _onGround)
	{
		_index = 4;
		_x = _player->getX();
		_y = _player->getY() + _maxHeight / 2;
	}
}
