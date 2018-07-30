#include "stdafx.h"
#include "tortoiseShield.h"

HRESULT tortoiseShield::init(float x, float y)
{
	enemy::init(x, y);

	_image = IGM->findImage("�ź���_����������");
	_isActive = true;
	_speed = 2.8f;
	_gravity = 0;
	return S_OK;
}

void tortoiseShield::update()
{

	if (!_isActive) 
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

	if (_isActive)
	{
		_hitBox = RectMakeCenter(_x, _y + 20, 220, 240);
		_index = 0; //�ǵ尡 �޷������� �ε����� 0����
		_gravity = 0;
	}
}

void tortoiseShield::render()
{
	if (!_isActive)
	{
		_image->frameRender(getMemDC(), _x - 95 - CAM->getX(), _y - 50 - CAM->getY(), _index, _dir);
	}
	if (_isDebug)
	{
		_stprintf_s(_debug, "angle: %f, speed: %f", _angle, _speed);
		TextOut(getMemDC(), 100, 120, _debug, strlen(_debug));
		//Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
}

void tortoiseShield::release()
{
}

void tortoiseShield::damaged(actor * e)
{
	//������ �ǵ带 ȸ���������� ���������� ����
}

void tortoiseShield::move()
{
	_gravity += 0.05f;
	if (310 < _x && _x < IMAGEMANAGER->findImage("������1")->getWidth() - 310)
	{
		_x += _speed * cos(PI - _angle);
	}
	_y += _speed * -sin(_angle) + _gravity;
}

void tortoiseShield::collide()
{
	/* //TODO : ���Ŀ� �÷��̾ ��� �����ͱ��� �����ؾ��Ѵ�.

	COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.top);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0)) // �������� �˻�
	{
		_y = _hitBox.top + (_hitBox.bottom - _hitBox.top) / 2;
		_angle = 3 * PI / 2;
		//break;
	}

	//�Ʒ� �˻�
	color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
	{
		_y = _hitBox.bottom - (_hitBox.bottom - _hitBox.top) / 2;
	}

	//���� �˻�
	color = GetPixel(_mapPixel->getMemDC(), _hitBox.left, _y);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0)) // ����Ÿ�� �ƴϸ� �˻� ���ٰ� �������̸� �˻�
	{
		_x = _hitBox.left + (_hitBox.right - _hitBox.left) / 2;
		_angle = 3 * PI / 2;
		//break;
	}

	//������ �˻�
	color = GetPixel(_mapPixel->getMemDC(), _hitBox.right, _y);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0))	// ����Ÿ�� �ƴϸ� �˻� ����Ÿ�� ����
										//�������� ���������� �ʷϻ��� ����ġ�� �ҷ��� 
	{
		_x = _hitBox.right - (_hitBox.right - _hitBox.left) / 2;
		_angle = 3 * PI / 2;
		//break;
	}
	*/

	//�ȼ��浹
	COLORREF color = GetPixel(IMAGEMANAGER->findImage("������1�ȼ�")->getMemDC(), _x, _hitBox.bottom);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	if (!(r == 255 && g == 0 && b == 255))
	{
		_y = _hitBox.bottom - 90;
		_speed = 0;
		_gravity = 0;
	}
}