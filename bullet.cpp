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
		if (getDistance(_fireX, _fireY, _x, _y) >= _range)
		{
			_isActive = false;
		}
		_hitBox = RectMakeCenter(_x, _y, _radius * 2, _radius * 2);

		if (_image != NULL)
		{
			++_count;
			if (_count % 7 == 0) ++_index;
			if (_index > _image->getMaxFrameX()) _index = 0;
		}
	}
}

void bullet::render(bool rotate)
{
	if (_isActive)
	{
		if (_image == NULL) Ellipse(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(),_hitBox.bottom-CAM->getY());
		else if(!rotate) _image->frameRender(getMemDC(), _x-CAM->getX(), _y-CAM->getY(), _index, _dir); //�⺻����
		else _image->rotateFrameRender(getMemDC(), _x - CAM->getX(), _y - CAM->getY(), _index, 0, _angle*0.017); //ȸ������
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
	if (!(r == 255 && g == 0 && b == 255))
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

bool bullet::collide(image * pixelImage)
{
	//���� �浹�ҋ�
	COLORREF color = GetPixel(pixelImage->getMemDC(), _x, _y);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);
	if (!(r == 255 && g == 0 && b == 255))
	{
		if (_isActive) _isActive = false;
		return true;
	}

	//��Ÿ� ���
	float distance = getDistance(_fireX, _fireY, _x, _y);
	if (_range < distance)
	{
		_isActive = false;
	}

	return false;
}
