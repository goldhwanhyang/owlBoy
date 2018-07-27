#include "stdafx.h"
#include "bullet.h"
HRESULT bullet::init(float radius, float speed, float range, const char * imageName)
{
	if (strcmp(imageName, "����") != 0) //���ڿ� ���Լ� strcmp 
		_image = IMAGEMANAGER->findImage(imageName);
	else
		_image = NULL;
	_radius = radius;
	_speed = speed;
	_range = range;
	_isActive = false;

	_x = _y = _fireX = _fireY = 0;
	_angle = 0;
	_gravity = 0;
	_count = _index = 0;

	return S_OK;
}

void bullet::update()
{
	if (_isActive)
	{
		_x += _speed * cosf(_angle * 0.017);
		_y += _speed * -sinf(_angle * 0.017) + _gravity;
		_hitBox = RectMakeCenter(_x-CAM->getX(), _y-CAM->getY(), _radius * 2, _radius * 2);

		if (_image != NULL)
		{
			++_count;
			if (_count % 7 == 0) ++_index;
			if (_index > _image->getMaxFrameX()) _index = 0;
		}
	}
}

void bullet::render()
{
	if (_isActive)
	{
		if (_image == NULL) Ellipse(getMemDC(), _hitBox);
		else _image->frameRender(getMemDC(), _x-CAM->getX(), _y-CAM->getY(), _index, _dir); //ȸ�������� �ٲ���
		//else _image->rotateframeRender(getMemDC(), _x - CAM->getX(), _y - CAM->getY(), _index, _dir, _angle);
	}
}

void bullet::release()
{
}

void bullet::collide(string pixelImageName)
{
	//���� �浹�ҋ�
	COLORREF color = GetPixel(IMAGEMANAGER->findImage(pixelImageName)->getMemDC(), _x, _y);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);
	if (r == 0 && g == 0 && b == 0)
	{
		if(_isActive) _isActive = false;
	}

	//��Ÿ� ���
	float distance = getDistance(_fireX, _fireY, _x, _y);
	if (_range < distance)
	{
		_isActive = false;
	}

	//�÷��̾�� �浹�� ������
}
